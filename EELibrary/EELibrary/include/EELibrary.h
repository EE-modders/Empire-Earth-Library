#pragma once

#ifdef EELIBRARY_EXPORTS
#define EELIBRARY_API __declspec(dllexport)
#else
#define EELIBRARY_API __declspec(dllimport)
#endif

#define EELIBRARY_OK 0
#define EELIBRARY_ERROR 1

