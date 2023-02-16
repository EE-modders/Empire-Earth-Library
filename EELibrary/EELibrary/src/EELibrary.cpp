#include "pch.h"
#include <EELibrary.h>
#include <Logger.h>
#include <Memory.h>
#include <Utils.h>
#include <sstream>
#include <exceptions/core/CoreInitException.h>
#include <exceptions/ModManagerException.h>

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
	Logger::Init("Library");

	Logger::Trace("Initialized Logger");
	Logger::Info("============================================================");
	Logger::Info("Initialized EELibrary v{}.{}.{}", EELIBRARY_VERSION_MAJOR, EELIBRARY_VERSION_MINOR, EELIBRARY_VERSION_PATCH);
	Logger::Info("Created by: EnergyCube");
	Logger::Info("============================================================");

	Logger::Trace("Initializing Memory...");
	_memory = std::make_unique<eelib::memory::Memory>();
	_memory->Initialize();
	Logger::Trace("Initialized Memory");

	Logger::Trace("Initializing ModManager...");
	_modManager = std::make_unique<eelib::mod::ModManager>();
	Logger::Trace("Initialized ModManager");
}

EELibrary::~EELibrary()
{
	Logger::Trace("Uninitializing Memory...");
	_memory->Uninitialize();
	_memory.reset();
	Logger::Trace("Uninitialized Memory");
	
	Logger::Trace("Uninitializing Logger...");
	Logger::Uninit();
}

void EELibrary::Init(HMODULE hModule)
{
	if (_hinst != NULL)
		throw CoreInitException("EELibrary already initialized");
	_hinst = hModule;
	if (LockLibraryIntoProcessMem(_hinst, &_hself) != ERROR_SUCCESS)
		throw CoreInitException("Failed to lock library into process memory");
	
	if (!FolderExist(_modPath))
		return;

	std::vector<std::filesystem::path> modPaths;
	for (const auto& entry : std::filesystem::directory_iterator(_modPath))
		if (entry.is_regular_file() && entry.path().extension() == ".dll")
			modPaths.push_back(entry.path());

	for (const auto& path : modPaths)
	{
		mod::Mod* mod = nullptr;
		try {
			Logger::Info("Loading mod {}...", path.filename().string());
			mod = _modManager->LoadMod(path.string().c_str());
			Logger::Info("Loaded mod {} v{}", mod->GetName(), mod->GetVersion().GetString());
			
			Logger::Info("Initializing mod {}...", mod->GetName());
			_modManager->InitMod(mod);
			Logger::Info("Initialized mod {}", mod->GetName());
			
			Logger::Info("Starting mod {}...", mod->GetName());
			_modManager->StartMod(mod);
			Logger::Info("Started mod {}", mod->GetName());
		}
		catch (mod::ModManagerException& e) {
			Logger::Error("Failed to load mod {}: {}", path.filename().string(), e.what());
			MessageBoxA(NULL, e.what(), "EE Library", MB_OK);
			if (mod != nullptr && _modManager->IsModLoaded(mod->GetPath()))
				_modManager->UnloadMod(mod);
			continue;
		}
	}
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
	Logger::Trace("Registering main hook");
	if (_memory->HookFunction(_memory->Game_StartAddr, pHook, (LPVOID*)&_memory->Game_StartOrg) != EELIBRARY_OK)
		return FALSE;
	Logger::Trace("Registered main hook");
	return TRUE;
}

BOOL EELibrary::RegisterMainEndHook(LPVOID pHook)
{
	Logger::Trace("Registering main end hook");
	if (_memory->HookFunction(_memory->LLE_UShutdownAddr, pHook, (LPVOID*)&_memory->LLE_UShutdownOrg) != EELIBRARY_OK)
		return FALSE;
	Logger::Trace("Registered main end hook");
	return TRUE;
}