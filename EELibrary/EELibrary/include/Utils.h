#pragma once

#include <ctime>

namespace eelib
{
    static long long FileSize(const wchar_t* name)
    {
        WIN32_FILE_ATTRIBUTE_DATA fad {};
        if (!GetFileAttributesEx(name, GetFileExInfoStandard, &fad))
            return -1; // error condition, could call GetLastError to find out more
        LARGE_INTEGER size;
        size.HighPart = fad.nFileSizeHigh;
        size.LowPart = fad.nFileSizeLow;
        return size.QuadPart;
    }

	static bool FileExist(LPCWSTR szPath)
	{
		DWORD dwAttrib = GetFileAttributes(szPath);

		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	static bool FolderExist(LPCWSTR szPath)
	{
		DWORD dwAttrib = GetFileAttributes(szPath);

		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

    static const std::string FormatDate(std::string format)
    {
        time_t now = time(nullptr);
        struct tm tstruct {};
        char buf[256];
        localtime_s(&tstruct, &now);
        strftime(buf, sizeof(buf), format.c_str(), &tstruct);
        return buf;
    }
}