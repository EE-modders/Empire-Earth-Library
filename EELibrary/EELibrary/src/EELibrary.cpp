#include "pch.h"
#include "EELibrary.h"
#include "Memory.h"
#include "Utils.h"
#include <sstream>

using namespace eelib;

int LockLibraryIntoProcessMem(HMODULE dllHandle, HMODULE* localDllHandle)
{
	if (localDllHandle == NULL)
		return ERROR_INVALID_PARAMETER;
	*localDllHandle = NULL;
	TCHAR DllPath[MAX_PATH];
	if (GetModuleFileName(dllHandle, DllPath, MAX_PATH) == NULL)
		return GetLastError();
	*localDllHandle = LoadLibrary(DllPath);
	if (*localDllHandle == NULL)
		return GetLastError();
	return ERROR_SUCCESS;
}

EELibrary::EELibrary()
{
	_memory = std::make_unique<eelib::memory::Memory>();
	_memory->Initialize();
}

EELibrary::~EELibrary()
{
	_memory->Uninitialize();
	_memory.reset();
}

BOOL EELibrary::Init(HMODULE hModule)
{
	if (_hinst != NULL)
		return FALSE;
	_hinst = hModule;
	if (LockLibraryIntoProcessMem(_hinst, &_hself) != ERROR_SUCCESS)
		throw std::exception("Failed to lock library into process memory");
	_modManager = std::make_unique<eelib::mod::ModManager>();

	//return;
	// load mods
	if (!FolderExist(_modPath))
		return TRUE; // No mods to load, what a shame
	for (const auto& entry : std::filesystem::directory_iterator(_modPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".dll")
		{
			mod::Mod* mod = nullptr;
			try {
				mod = _modManager->LoadMod(entry.path().c_str());
				_modManager->InitMod(mod);
				_modManager->StartMod(mod);
			}
			catch (std::exception& e) {
				MessageBoxA(NULL, e.what(), "EE Library", MB_OK);
				if (mod != nullptr && _modManager->IsModLoaded(mod->GetPath()))
					_modManager->UnloadMod(mod);
				continue;
			}
		}
	}
	return TRUE;
}

void EELibrary::Run()
{
}

void EELibrary::Exit()
{
	_modManager->UnloadAllMods();
	_modManager.reset();
	// We don't uninitalize Memory here because we need it
	// to call the original LLE UShutdown function
	// We will destroy the object after that function is called
	// which is totally fine because everything is already unloaded
}

std::unique_ptr<memory::Memory>& EELibrary::GetMemory()
{
	return _memory;
}

BOOL EELibrary::RegisterMainHook(LPVOID pHook)
{
	if (_memory->HookFunction(_memory->Game_StartAddr, pHook, (LPVOID*)&_memory->Game_StartOrg) != EELIBRARY_OK)
		return FALSE;
	return TRUE;
}

BOOL EELibrary::RegisterMainEndHook(LPVOID pHook)
{
	if (_memory->HookFunction(_memory->LLE_UShutdownAddr, pHook, (LPVOID*)&_memory->LLE_UShutdownOrg) != EELIBRARY_OK)
		return FALSE;
	return TRUE;
}