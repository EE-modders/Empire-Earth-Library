// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "EELibrary.h"
#include <MinHook.h>
#include <dinput.h>
#include <iostream>

// #pragma comment(lib, "../Low-Level Engine.lib")

bool bRun = true;
HANDLE hThread = NULL;

BOOL Detach(BOOL processTerminationAsked)
{
    bRun = false;

    // Only wait for the thread to exit if we terminated the process ourselves
    // Because when lpReserved != NULL, the DLL is being unloaded due to a process termination
    // And at that point, the thread is already dead because... Windows ?
    if (!processTerminationAsked && hThread != NULL) {
        if (WaitForSingleObject(hThread, 4242) == WAIT_TIMEOUT)
            TerminateThread(hThread, 0); // too bad
        CloseHandle(hThread);
    }

    if (processTerminationAsked && instance != nullptr) {
        instance->Exit();
        instance.reset();
    }
    return TRUE;
}

DWORD CALLBACK ThreadLoop(LPVOID memory)
{
    while (bRun) {
        instance->Run();
        Sleep(1000);
    }
    return 0;
}

LINK_HOOK(void, Game_Start)
{
    hThread = CreateThread(NULL, NULL, ThreadLoop, NULL, NULL, NULL);
    instance->GetMemory()->Game_StartOrg();
}

LINK_HOOK(void, LLE_UShutdown, bool coUninitialize, bool exit)
{
    Detach(FALSE);
    instance->Exit();
    instance->GetMemory()->LLE_UShutdownOrg(coUninitialize, exit);
    instance.reset();
    // maybe interesting? _CrtDumpMemoryLeaks();
}

// At 005c142d
// The game compare
// CMP byte ptr [EBP + -0x1],0x0
// A valid campain is loaded if the value is 0x1
// And the game initialize the value to 0x0
// So we just need to set the value to 0x1 before the comparison
// and the game will think the campain is valid!
LPVOID CampainValidityCheckOrg = nullptr;

void __declspec(naked) CampainValidityCheckHookFn()
{
    __asm
    {
		mov byte ptr[ebp - 0x1], 0x1
		jmp[CampainValidityCheckOrg]
    }
}

/*
auto CampainValidityCheckAddr = (LPVOID)(0x001c142d + instance->GetMemory()->moduleGameAddress);

if (instance->GetMemory()->HookFunction(CampainValidityCheckAddr, CampainValidityCheckHookFn, &CampainValidityCheckOrg) != EELIBRARY_OK)
        throw std::exception("Failed to hook CampainValidityCheckAddr");
*/

BOOL Attach(HMODULE hModule)
{
    DisableThreadLibraryCalls(hModule);

    if (instance != nullptr)
        return FALSE;

    try {
        instance = std::make_unique<eelib::EELibrary>();
        instance->Init(hModule);

        if (!instance->RegisterMainHook(Game_StartHookFn))
            throw std::exception("Failed to register main hook");
        if (!instance->RegisterMainEndHook(LLE_UShutdownHookFn))
            throw std::exception("Failed to register dll unload hook");

    } catch (const std::exception& e) {
        // Show topmost message box with error message
        MessageBoxA(NULL, e.what(), "EE Library", MB_ICONERROR | MB_TOPMOST);
        instance.reset();
        return FALSE;
    }
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
#ifdef _DEBUG
        // wait for debugger to attach
        while (!IsDebuggerPresent() && !GetAsyncKeyState(VK_F5))
            Sleep(100);
#endif // _DEBUG
        Attach(hModule);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH: {
        Detach(lpReserved != nullptr);
        break;
    }
    default:
        break;
    }
    return TRUE;
}
