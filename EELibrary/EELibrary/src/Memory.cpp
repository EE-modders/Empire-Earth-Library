#include "pch.h"
#include "Memory.h"
#include <MinHook.h>

using namespace eelib::memory;

GameType Memory::Initialize()
{
	_gameType = Unknown;
	gameAddress = (DWORD) GetModuleHandle(NULL);
	lleAddress = (DWORD) GetModuleHandle(L"Low-Level Engine");

	if (gameAddress == NULL || lleAddress == NULL)
		return _gameType;

	BYTE clientCode = *(BYTE*) (gameAddress + clientCodeAddress);

	if (clientCode == 0xE8)
	{
		InitializeEE();
		_gameType = EE;
	}
	else if (clientCode == 0x55)
	{
		InitializeAoC();
		_gameType = AoC;
	}

	MH_Initialize();
	
	return _gameType;
}

void Memory::Uninitialize()
{
	MH_Uninitialize();
}

void Memory::InitializeEE()
{
	mainHook = LPVOID((DWORD)gameAddress + 0x0013BD15);
}

void Memory::InitializeAoC()
{
	mainHook = LPVOID(gameAddress + 0x00148514);
}

#define PAGE_EXECUTE_FLAGS \
    (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
BOOL IsExecutableAddress(LPVOID pAddress)
{
	MEMORY_BASIC_INFORMATION mi;
	VirtualQuery(pAddress, &mi, sizeof(mi));

	return (mi.State == MEM_COMMIT && (mi.Protect & PAGE_EXECUTE_FLAGS));
}

int Memory::HookFunction(LPVOID hookAddress, LPVOID hookFunction, LPVOID* originalFunction)
{
	DWORD oldProtect;
	bool isExecutable = IsExecutableAddress(hookAddress);
	if (!isExecutable) // If the address is not executable, we need to make it so
		VirtualProtect(hookAddress, 5, PAGE_EXECUTE, &oldProtect);
	if (MH_CreateHook(hookAddress, hookFunction, originalFunction) != MH_OK)
		return EELIBRARY_ERROR;
	if (MH_EnableHook(hookAddress) != MH_OK)
		return EELIBRARY_ERROR;
	if (!isExecutable) // If the address was not executable, we need to restore the old protection
		VirtualProtect(hookAddress, 5, oldProtect, &oldProtect);
	return EELIBRARY_OK;
}

int Memory::UnhookFunction(LPVOID hookAddress)
{
	if (MH_DisableHook(hookAddress) != MH_OK)
		return EELIBRARY_ERROR;
	return EELIBRARY_OK;
}
