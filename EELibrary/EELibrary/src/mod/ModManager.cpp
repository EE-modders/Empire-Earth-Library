#include "pch.h"

#include "Logger.h"
#include "mod/ModManager.h"
#include <exceptions/ModManagerException.h>

using namespace eelib::mod;

ModManager::ModManager()
{
    _implem = new Pimpl();
}

ModManager::~ModManager()
{
    if (_implem->m_Mods.size() > 0) {
        Logger::Error("ModManager is being destroyed while mods are still loaded!");
    }
    delete _implem;
}

Mod* ModManager::LoadMod(const std::string& modPath)
{
    Mod* mod = nullptr;
    auto iter = _implem->m_Mods.find(modPath);

    if (iter != _implem->m_Mods.end())
        throw ModManagerException(("Mod " + modPath + " already loaded").c_str());

    Logger::Trace("Loading mod {} 1/6...", modPath);
    HMODULE hModule = LoadLibraryA(modPath.c_str());
    if (hModule == NULL)
        throw ModManagerException(("Could not load library " + modPath).c_str());

    Logger::Trace("Loading mod {} 2/6...", modPath);
    eelib::mod::Mod::fnGetLibraryVersion GetLibVersion = (eelib::mod::Mod::fnGetLibraryVersion)GetProcAddress(hModule, "GetLibraryVersion");
    if (GetLibVersion == NULL) {
        FreeLibrary(hModule);
        throw ModManagerException(("Could not find GetLibraryVersion in " + modPath).c_str());
    }
    int modApiVersion = GetLibVersion();

    Logger::Trace("Loading mod {} 3/6...", modPath);
    if (modApiVersion < EELIBRARY_LAST_COMPATIBLE_VERSION) {
        FreeLibrary(hModule);
        throw ModManagerException(("Mod " + modPath + " is not compatible with this version of EE Library").c_str());
    }

    Logger::Trace("Loading mod {} 4/6...", modPath);
    eelib::mod::Mod::fnCreateMod CreateMod = (eelib::mod::Mod::fnCreateMod)GetProcAddress(hModule, "CreateMod");
    if (CreateMod == NULL) {
        FreeLibrary(hModule);
        throw ModManagerException(("Could not find symbol \"CreateMod\" in " + modPath).c_str());
    }
    mod = CreateMod();

    Logger::Trace("Loading mod {} 5/6...", modPath);
    if (mod == nullptr) {
        FreeLibrary(hModule);
        throw ModManagerException(("Could not load mod from " + modPath).c_str());
    }
    mod->SetPath(modPath.c_str());
    mod->SetInitialized(false);
    mod->SetRunning(false);
    Logger::Trace("Loading mod {} 6/6...", modPath);
    _implem->m_Mods.insert(Pimpl::ModMap::value_type(modPath, mod));
    _implem->m_Libs.insert(Pimpl::LibraryMap::value_type(modPath, std::move(hModule)));
    return mod;
}

bool ModManager::IsModLoaded(const char* modName) const
{
    auto iter = _implem->m_Mods.find(modName);
    return iter != _implem->m_Mods.end();
}

void ModManager::UnloadMod(Mod* mod)
{
    if (mod == nullptr)
        throw ModManagerException("Mod is NULL");

    auto iter = _implem->m_Libs.find(mod->GetPath());
    if (iter == _implem->m_Libs.end())
        throw ModManagerException("Trying to unload mod that is already unloaded or has never been loaded.");

    if (mod->IsRunning())
        StopMod(mod);

    _implem->m_Mods.erase(mod->GetPath());

    HMODULE hModule = iter->second;
    mod::Mod::fnDestroyMod DestroyMod = (mod::Mod::fnDestroyMod)GetProcAddress(hModule, "DestroyMod");
    if (DestroyMod == NULL)
        throw ModManagerException("Unable to find symbol \"DestroyMod\" in library");
    DestroyMod();
    FreeLibrary(hModule);
    _implem->m_Libs.erase(iter);
    mod = nullptr;
    hModule = NULL;
}

void ModManager::UnloadAllMods()
{
    Pimpl::ModMap::iterator iter = _implem->m_Mods.begin();
    while (iter != _implem->m_Mods.end()) {
        Mod* mod = iter->second;
        UnloadMod(mod);
        iter = _implem->m_Mods.begin();
    }
}

void ModManager::InitMod(Mod* mod)
{
    if (mod == nullptr)
        throw ModManagerException("Mod is NULL");
    if (!IsModLoaded(mod->GetPath()))
        throw ModManagerException("Mod is not loaded");
    if (mod->IsInitialized())
        throw ModManagerException("Mod is already initialized");
    mod->SetInitialized(mod->OnStart());
}

void ModManager::StartMod(Mod* mod)
{
    if (mod == nullptr)
        throw ModManagerException("Mod is NULL");
    if (!IsModLoaded(mod->GetPath()))
        throw ModManagerException("Mod is not loaded");
    if (!mod->IsInitialized())
        throw ModManagerException("Mod is not initialized");

    auto iter = _implem->m_Threads.find(mod->GetPath());
    if (iter != _implem->m_Threads.end())
        throw ModManagerException("Mod already started");

    // don't work for some reasons: std::thread thread = std::thread(&Mod::OnStart, mod);
    // let's use CreateThread instead...
    // Fist create a lambda function that will call the OnStart method of the mod
    LPTHREAD_START_ROUTINE threadStartRoutine = [](LPVOID lpParameter) -> DWORD {
        Mod* mod = reinterpret_cast<Mod*>(lpParameter);
        if (mod == NULL)
            throw ModManagerException("Mod cast failed in thread start routine");
        mod->SetRunning(true);
        mod->OnUpdate();
        mod->SetRunning(false);
        return 0;
    };
    HANDLE hThread = CreateThread(NULL, 0, threadStartRoutine, mod, 0, NULL);
    if (hThread == NULL)
        throw ModManagerException("Could not create thread");
    SetThreadPriority(hThread, THREAD_PRIORITY_BELOW_NORMAL);
    _implem->m_Threads.insert(Pimpl::ThreadMap::value_type(mod->GetPath(), hThread));
}

void ModManager::StopMod(Mod* mod)
{
    if (mod == nullptr)
        throw ModManagerException("Mod is NULL");
    if (!mod->IsInitialized())
        throw ModManagerException("Mod is not initialized");
    if (!mod->IsRunning())
        throw ModManagerException("Mod is already stopped");
    auto iter = _implem->m_Threads.find(mod->GetPath());
    if (iter == _implem->m_Threads.end())
        throw ModManagerException("Mod not started");
    HANDLE hThread = iter->second;
    if (hThread == NULL)
        throw ModManagerException("Mod thread is NULL");

    mod->SetRunning(false);
    if (WaitForSingleObject(hThread, 4242) == WAIT_TIMEOUT)
        TerminateThread(hThread, 0); // too bad
    CloseHandle(hThread);
    hThread = NULL;

    if (!mod->OnStop())
        throw ModManagerException("Mod failed to disable");

    _implem->m_Threads.erase(iter);
}
