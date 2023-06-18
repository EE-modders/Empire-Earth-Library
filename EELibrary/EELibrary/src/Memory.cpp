#include "pch.h"

#include "Memory.h"
#include <MinHook.h>

using namespace eelib::memory;

void Memory::Initialize()
{
    moduleGameAddress = (DWORD)GetModuleHandle(NULL);
    moduleLLEAddress = (DWORD)GetModuleHandle(L"Low-Level Engine");
    moduleTCPProtoAddress = (DWORD)GetModuleHandle(L"TCP Protocol"); // don't work

    if (moduleGameAddress == NULL || moduleLLEAddress == NULL /*|| moduleTCPProtoAddress == NULL*/)
        throw std::exception("Failed to get mandatory module addresses");

    moduleDX7DispAddress = (DWORD)GetModuleHandle(L"DX7HRDisplay");
    moduleDX7TnLDispAddress = (DWORD)GetModuleHandle(L"DX7HRTnLDisplay");

    if (moduleDX7DispAddress == NULL && moduleDX7TnLDispAddress == NULL)
        throw std::exception("Failed to get at least one of the display modules");

    BYTE clientCode = *(BYTE*)(moduleGameAddress + clientCodeAddress);
    if (clientCode != _clientCodeEE && clientCode != _clientCodeAoC)
        throw std::exception("Failed to determine game type");

    if (MH_Initialize() != MH_OK)
        throw std::exception("Failed to initialize MinHook");

    if (clientCode == 0xE8)
        InitializeEE();
    else if (clientCode == 0x55)
        InitializeAoC();
}

void Memory::Uninitialize()
{
    if (MH_Uninitialize() != MH_OK)
        throw std::exception("Failed to uninitialize MinHook");
}

void Memory::InitializeEE()
{
    _gameType = EE;
    REGISTER_HOOK_ADDR(Game_Start, moduleGameAddress + 0x001360ca) // 0x002603c4
    REGISTER_HOOK_ADDR(LLE_UShutdown, moduleLLEAddress + 0x00080634)
}

void Memory::InitializeAoC()
{
    _gameType = AoC;
    REGISTER_HOOK_ADDR(Game_Start, 0x00148514)
}

int Memory::HookFunction(LPVOID orgAddress, LPVOID hookFn, LPVOID* orgFn)
{
    DWORD oldProtect;
    bool isExecutable = IsExecutableAddress(orgAddress);

    if (!isExecutable) // If the address is not executable, we need to make it so
        VirtualProtect(orgAddress, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    if (MH_CreateHook(orgAddress, hookFn, orgFn) != MH_OK)
        return EELIBRARY_ERROR;
    if (MH_EnableHook(orgAddress) != MH_OK)
        return EELIBRARY_ERROR;
    if (!isExecutable) // If the address was not executable, we need to restore
                       // the old protection
        VirtualProtect(orgAddress, 5, oldProtect, &oldProtect);
    return EELIBRARY_OK;
}

int Memory::UnhookFunction(LPVOID orgAddress)
{
    if (MH_DisableHook(orgAddress) != MH_OK)
        return EELIBRARY_ERROR;
    return EELIBRARY_OK;
}

int Memory::HookFunctionByApi(LPCWSTR pszModule, LPCSTR pszProcName,
    LPVOID pDetour, LPVOID* ppOriginal)
{
    MH_STATUS status;
    status = MH_CreateHookApi(pszModule, pszProcName, pDetour, ppOriginal);
    if (status != MH_OK)
        return EELIBRARY_ERROR;
    status = MH_EnableHook(MH_ALL_HOOKS);
    if (status != MH_OK)
        return EELIBRARY_ERROR;
    return EELIBRARY_OK;
}

bool Memory::IsExecutableAddress(LPVOID pAddress)
{
    DWORD flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
    MEMORY_BASIC_INFORMATION mi;

    VirtualQuery(pAddress, &mi, sizeof(mi));

    return (mi.State == MEM_COMMIT && (mi.Protect & flags));
}
