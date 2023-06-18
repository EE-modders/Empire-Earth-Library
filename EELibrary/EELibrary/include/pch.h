// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#define new new (_CLIENT_BLOCK, __FILE__, __LINE__)

// Disable 4251
#define SAFE_EXPORT_START __pragma(warning(push)) __pragma(warning(disable : 4251))
#define SAFE_EXPORT_END __pragma(warning(pop))

#endif // PCH_H
