#pragma once

#pragma comment(lib, "../Debug/EELibrary.lib")
#include <mod/Mod.h>

#define MOD_NAME ModTemplate
#define MOD_NAME_STRING "Mod Template"

/*
        Sleep is required to prevent the thread from using too much CPU
        Feel free to change the sleep time to your liking (in milliseconds)
        But don't set it too low, or you will get a lot of CPU usage :V

        When the mod get unloaded the Library give 4s to the thread to exit
*/
#define MOD_UPDATE_INTERVAL 250

#define MOD_VERSION_MAJOR 1
#define MOD_VERSION_MINOR 0
#define MOD_VERSION_PATCH 0

#define MOD_VERSION_STRING STRINGIFY(MOD_VERSION_MAJOR) "." STRINGIFY(MOD_VERSION_MINOR) "." STRINGIFY(MOD_VERSION_PATCH

#ifdef MODTEMPLATE_EXPORTS
#define MODTEMPLATE_API __declspec(dllexport)
#else
#define MODTEMPLATE_API __declspec(dllimport)
#endif

/*
        You should not need to modify anything below this line.
        =======================================================
*/

#define MOD_VERSION ((MOD_VERSION_MAJOR << 16) | (MOD_VERSION_MINOR << 8) | MOD_VERSION_PATCH)
#if MOD_VERSION_MAJOR > 255 || MOD_VERSION_MINOR > 255 || MOD_VERSION_PATCH > 255
#error "MOD_VERSION_MAJOR, MOD_VERSION_MINOR and MOD_VERSION_PATCH must be less than 256."
#endif

extern "C" MODTEMPLATE_API int GetLibraryVersion();
extern "C" MODTEMPLATE_API eelib::mod::Mod* CreateMod(void);
extern "C" MODTEMPLATE_API void DestroyMod(void);
