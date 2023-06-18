/*

        You should not need to modify anything in this file.

*/

#include "pch.h"

#include "ModTemplate.h"
#include <cassert>

static MOD_NAME* g_ModInstance = NULL;

extern "C" MODTEMPLATE_API int GetLibraryVersion()
{
    return EELIBRARY_VERSION;
}

extern "C" MODTEMPLATE_API eelib::mod::Mod* CreateMod(void)
{
    assert(g_ModInstance == NULL);
    g_ModInstance = new MOD_NAME();
    return g_ModInstance;
}

extern "C" MODTEMPLATE_API void DestroyMod()
{
    assert(g_ModInstance != NULL);
    delete g_ModInstance;
    g_ModInstance = NULL;
}

/*
    Historically, Empire Earth mods DLLs took advantage of DllMain
    to launch a thread and modify the game during execution.
    However you don't have to do this, as Empire Earth Library takes
    care of this step and calls your OnInit, OnStart and OnStop functions.
    It is therefore extremely important not to put anything here.
*/
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
