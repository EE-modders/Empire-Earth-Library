#include "pch.h"
#include "mod/ModManager.h"

using namespace eelib::mod;

ModManager::ModManager()
{
    _modManagerPimpl = new ModManagerPimpl();
}

ModManager::~ModManager()
{
    delete _modManagerPimpl;
}

Mod* ModManager::LoadMod(const wchar_t* modPath)
{
    Mod* mod = nullptr;

    auto iter = _modManagerPimpl->m_Mods.find(modPath);
    char* modPathChar = new char[wcslen(modPath) + 1];
	wcstombs_s(NULL, modPathChar, wcslen(modPath) + 1, modPath, wcslen(modPath) + 1);

    if (iter != _modManagerPimpl->m_Mods.end())
        throw std::exception(("Mod " + std::string(modPathChar) + " already loaded").c_str());
    // or we could return mod = iter->second;

    HMODULE hModule = LoadLibraryW(modPath);
    if (hModule == NULL)
        throw std::exception(("Could not load library " + std::string(modPathChar)).c_str());

	eelib::mod::Mod::fnGetLibraryVersion GetLibVersion = (eelib::mod::Mod::fnGetLibraryVersion)GetProcAddress(hModule, "GetLibraryVersion");
    if (GetLibVersion == NULL)
    {
		FreeLibrary(hModule);
		throw std::exception(("Could not find GetLibraryVersion in " + std::string(modPathChar)).c_str());
    }

    int modApiVersion = GetLibVersion();

    if (modApiVersion < EELIBRARY_LAST_COMPATIBLE_VERSION)
    {
		FreeLibrary(hModule);
		throw std::exception(("Mod " + std::string(modPathChar) + " is not compatible with this version of EE Library").c_str());
    }
    
    eelib::mod::Mod::fnCreateMod CreateMod = (eelib::mod::Mod::fnCreateMod)GetProcAddress(hModule, "CreateMod");
    if (CreateMod == NULL)
    {
        FreeLibrary(hModule);
        throw std::exception(("Could not find symbol \"CreateMod\" in " + std::string(modPathChar)).c_str());
    }

    mod = CreateMod();

    if (mod == nullptr)
    {
        FreeLibrary(hModule);
        throw std::exception(("Could not load mod from " + std::string(modPathChar)).c_str());
    }
    mod->SetPath(modPath);
	mod->SetInitialized(false);
    mod->SetRunning(false);
    _modManagerPimpl->m_Mods.insert(ModManagerPimpl::ModMap::value_type(modPath, mod));
	_modManagerPimpl->m_Libs.insert(ModManagerPimpl::LibraryMap::value_type(modPath, std::move(hModule)));
    return mod;
}

bool ModManager::IsModLoaded(const wchar_t* modName) const
{
    auto iter = _modManagerPimpl->m_Mods.find(modName);
    return iter != _modManagerPimpl->m_Mods.end();
}

void ModManager::UnloadMod(Mod* mod)
{
    if (mod == nullptr)
        throw std::exception("Mod is NULL");

    auto iter = _modManagerPimpl->m_Libs.find(mod->GetPath());
    if (iter == _modManagerPimpl->m_Libs.end())
        throw std::exception("Trying to unload mod that is already unloaded or has never been loaded.");

    if (mod->IsRunning())
        StopMod(mod);

   _modManagerPimpl->m_Mods.erase(mod->GetPath());

    HMODULE hModule = iter->second;
    mod::Mod::fnDestroyMod DestroyMod = (mod::Mod::fnDestroyMod)GetProcAddress(hModule, "DestroyMod");
    if (DestroyMod == NULL)
        throw std::exception("Unable to find symbol \"DestroyMod\" in library");
	DestroyMod();
    FreeLibrary(hModule);
    _modManagerPimpl->m_Libs.erase(iter);
    mod = nullptr;
	hModule = NULL;
}

void ModManager::UnloadAllMods()
{
	ModManagerPimpl::ModMap::iterator iter = _modManagerPimpl->m_Mods.begin();
	while (iter != _modManagerPimpl->m_Mods.end())
	{
		Mod* mod = iter->second;
		UnloadMod(mod);
		iter = _modManagerPimpl->m_Mods.begin();
	}
}

void ModManager::InitMod(Mod* mod)
{
    if (mod == nullptr)
        throw std::exception("Mod is NULL");
	if (!IsModLoaded(mod->GetPath()))
		throw std::exception("Mod is not loaded");
	if (mod->IsInitialized())
		throw std::exception("Mod is already initialized");
	mod->SetInitialized(mod->OnStart());
}

void ModManager::StartMod(Mod* mod)
{
    if (mod == nullptr)
        throw std::exception("Mod is NULL");
	if (!IsModLoaded(mod->GetPath()))
		throw std::exception("Mod is not loaded");
	if (!mod->IsInitialized())
		throw std::exception("Mod is not initialized");
    
    auto iter = _modManagerPimpl->m_Threads.find(mod->GetPath());
    if (iter != _modManagerPimpl->m_Threads.end())
        throw std::exception("Mod already started");

    // don't work for some reasons: std::thread thread = std::thread(&Mod::OnStart, mod);
	// let's use CreateThread instead...
	// Fist create a lambda function that will call the OnStart method of the mod
    LPTHREAD_START_ROUTINE threadStartRoutine = [](LPVOID lpParameter) -> DWORD
	{
		Mod* mod = reinterpret_cast<Mod*>(lpParameter);
		if (mod == NULL)
			throw std::exception("Mod cast failed in thread start routine");
        mod->SetRunning(true);
        mod->OnUpdate();
        mod->SetRunning(false);
		return 0;
	};
	HANDLE hThread = CreateThread(NULL, 0, threadStartRoutine, mod, 0, NULL);
	if (hThread == NULL)
		throw std::exception("Could not create thread");
	SetThreadPriority(hThread, THREAD_PRIORITY_BELOW_NORMAL);
    _modManagerPimpl->m_Threads.insert(ModManagerPimpl::ThreadMap::value_type(mod->GetPath(), hThread));
}

void ModManager::StopMod(Mod* mod)
{
    if (mod == nullptr)
        throw std::exception("Mod is NULL");
	if (!mod->IsInitialized())
		throw std::exception("Mod is not initialized");
	if (!mod->IsRunning())
		throw std::exception("Mod is already stopped");
    auto iter = _modManagerPimpl->m_Threads.find(mod->GetPath());
    if (iter == _modManagerPimpl->m_Threads.end())
        throw std::exception("Mod not started");
	HANDLE hThread = iter->second;
	if (hThread == NULL)
		throw std::exception("Mod thread is NULL");

    mod->SetRunning(false);
    if (WaitForSingleObject(hThread, 4242) == WAIT_TIMEOUT)
        TerminateThread(hThread, 0); // too bad
	CloseHandle(hThread);
	hThread = NULL;

    if (!mod->OnStop())
        throw std::exception("Mod failed to disable");
    
	_modManagerPimpl->m_Threads.erase(iter);
}
