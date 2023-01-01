// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Memory.h"
#include <MinHook.h>

bool bRun = true;
HANDLE hThread = NULL;
LPVOID EEMain_original;


// Create ThreadLoop
DWORD WINAPI ThreadLoop(LPVOID memory)
{
	//eelib::memory::Memory* mem = static_cast<eelib::memory::Memory*>(memory);
	MessageBox(NULL, L"Hello World!", L"Hello World!", MB_OK);
	while (bRun)
	{
		Sleep(1000); // Keep thread alive
	}
	MessageBox(NULL, L"ThreadLoop has ended", L"ThreadLoop", MB_OK);
	return 0;
}

void __fastcall GameMain(int regecx)
{
	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadLoop, NULL, NULL, NULL);

	// dxm do that, idk why it seems to be a manual trampoline
	// maybe he didn't used minhook back then and still have that trampoline active :V ?
	/*__asm
	{
		MOV ECX, regecx
		CALL EEMain_original
	}*/
}

bool Detach()
{
	bRun = false;
	MH_Uninitialize();
	if (WaitForSingleObject(hThread, 4242) == WAIT_TIMEOUT)
		TerminateThread(hThread, 0);
	if (hThread != NULL)
		CloseHandle(hThread);
	return true;
}

HMODULE g_hinst;
HMODULE g_Self;

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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH:
		{
			g_hinst = hModule;
			Sleep(5000);
			try
			{
				LockLibraryIntoProcessMem(hModule, &g_Self);
				eelib::memory::Memory mem;
				if (mem.Initialize() == eelib::memory::Unknown)
					return FALSE;
				if (mem.HookFunction(mem.mainHook, &GameMain, &EEMain_original) != EELIBRARY_OK)
					return FALSE;
			}
			catch (...)
			{
				MessageBox(NULL, L"Exception", L"Exception", MB_OK);
				return FALSE;
			}
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		{
			break;
		}
    }
    return TRUE;
}

