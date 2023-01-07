#pragma once

// Fix for Platform Toolset: Windows XP (v141_xp)
#if _MSC_VER >= 1900 // idk if this is the right version lol
typedef struct IUnknown IUnknown;
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <Windows.h>