#include "pch.h"
#include "mod/ModManager.h"

using namespace eelib::mod;

ModManager::ModManager(void)
{
    _modManagerPimpl = new ModManagerPimpl();
}

ModManager::~ModManager(void)
{
    delete _modManagerPimpl;
}

ModManager& ModManager::Instance()
{
    static ModManager modManager;
    return modManager;
}

Mod* ModManager::LoadMod(const wchar_t* modPath)
{
    Mod* mod = NULL;
    ModManagerPimpl::ModMap::iterator iter = _modManagerPimpl->m_Mods.find(modPath);
    char* modPathChar = new char[wcslen(modPath) + 1];
	wcstombs_s(NULL, modPathChar, wcslen(modPath) + 1, modPath, wcslen(modPath) + 1);

    if (iter != _modManagerPimpl->m_Mods.end())
        throw std::exception(("Mod " + std::string(modPathChar) + " already loaded").c_str());
    // or we could return mod = iter->second;

    HMODULE hModule = LoadLibraryW(modPath);
    if (hModule == NULL)
        throw std::exception(("Could not load library " + std::string(modPathChar)).c_str());
    fnCreateMod CreateMod = (fnCreateMod)GetProcAddress(hModule, "CreateMod");
    if (CreateMod == NULL)
    {
        FreeLibrary(hModule);
        throw std::exception(("Could not find symbol \"CreateMod\" in " + std::string(modPathChar)).c_str());
    }

    mod = CreateMod();

    if (mod == NULL)
    {
        FreeLibrary(hModule);
        throw std::exception(("Could not load mod from " + std::string(modPathChar)).c_str());
    }
    mod->SetPath(modPath);
    _modManagerPimpl->m_Mods.insert(ModManagerPimpl::ModMap::value_type(modPath, mod));
    _modManagerPimpl->m_Libs.insert(ModManagerPimpl::LibraryMap::value_type(modPath, hModule));
    return mod;
}

bool ModManager::IsModLoaded(const wchar_t* modName) const
{
    ModManagerPimpl::ModMap::iterator iter = _modManagerPimpl->m_Mods.find(modName);
    return iter != _modManagerPimpl->m_Mods.end();
}

void ModManager::UnloadMod(Mod*& mod)
{
    if (mod == NULL)
        throw std::exception("Mod is NULL");
    ModManagerPimpl::LibraryMap::iterator iter = _modManagerPimpl->m_Libs.find(mod->GetPath());
    if (iter == _modManagerPimpl->m_Libs.end())
        throw std::exception("Trying to unload mod that is already unloaded or has never been loaded.");

    _modManagerPimpl->m_Mods.erase(mod->GetPath());

    HMODULE hModule = iter->second;
    fnDestroyMod DestroyMod = (fnDestroyMod)GetProcAddress(hModule, "DestroyMod");
    if (DestroyMod == NULL)
        throw std::exception("Unable to find symbol \"DestroyMod\" in library");
    DestroyMod();
    FreeLibrary(hModule);
    _modManagerPimpl->m_Libs.erase(iter);
    mod = NULL;
}

void ModManager::InitMod(Mod* mod)
{
    if (mod == NULL)
        throw std::exception("Mod is NULL");
    ModManagerPimpl::ThreadMap::iterator iter = _modManagerPimpl->m_Threads.find(mod->GetPath());
    mod->SetEnabled(mod->OnInit());
}

void ModManager::StartMod(Mod* mod)
{
    if (mod == NULL)
        throw std::exception("Mod is NULL");
    if (!mod->IsEnabled())
        throw std::exception("Mod is not enabled");
    ModManagerPimpl::ThreadMap::iterator iter = _modManagerPimpl->m_Threads.find(mod->GetPath());
    if (iter != _modManagerPimpl->m_Threads.end())
        throw std::exception("Mod already started");
    std::thread thread = std::thread(&Mod::OnStart, mod);
    _modManagerPimpl->m_Threads.insert(ModManagerPimpl::ThreadMap::value_type(mod->GetPath(), std::move(thread)));
}

void ModManager::StopMod(Mod* mod)
{
    if (mod == NULL)
        throw std::exception("Mod is NULL");
    ModManagerPimpl::ThreadMap::iterator iter = _modManagerPimpl->m_Threads.find(mod->GetPath());
    if (iter == _modManagerPimpl->m_Threads.end())
        throw std::exception("Mod not started");
    if (!mod->OnStop())
        throw std::exception("Mod failed to disable");
    _modManagerPimpl->m_Threads.at(mod->GetPath()).join();
    _modManagerPimpl->m_Threads.erase(iter);
}