#pragma once

// temp for testing (need to be defined in vcxproj)
#pragma comment(lib, "gdiplus.lib")
#include <objidl.h>
// MS didn't checked GDI+ headers is compatible with /permissive- flag
#pragma warning(disable : 4596)
#include <gdiplus.h>
#pragma warning(default : 4596)

/*
// Use GDI+ to draw text on screen
void DrawTextCentered(Gdiplus::Graphics* graphics, const wchar_t* text, RECT rect)
{
        Gdiplus::FontFamily fontFamily(L"Arial");
        Gdiplus::Font font(&fontFamily, 16, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
}

void DrawBlackSemiTransparentBackground(Gdiplus::Graphics& graphics, RECT rect)
{
        // Create a solid brush.
        Gdiplus::SolidBrush blackBrush(Gdiplus::Color(128, 0, 0, 0));

        // Fill rectangle to screen.
        graphics.FillRectangle(&blackBrush, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

void DrawImage(Gdiplus::Graphics& graphics, Gdiplus::Image& bitmap, RECT rect)
{
        graphics.DrawImage(&bitmap, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

DWORD WINAPI ProgressTextLoop(LPVOID memory)
{
        // Use GDI+ to display loading text
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        RECT rect;
        GetWindowRect(GetDesktopWindow(), &rect);

        // We don't have a window handle yet, so we need to create our own on top of the game window
        HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, L"STATIC", L"Loading", WS_POPUP, 0, 0, rect.right, rect.bottom, NULL, NULL, g_hinst, NULL);
        ShowWindow(hWnd, SW_SHOW);

        Gdiplus::Graphics graphics(hWnd);
        Gdiplus::Image bitmap(L"test.png");
        // Draw the text
        while (!bInit)
        {
                //DrawBlackSemiTransparentBackground(graphics, rect);
                DrawImage(graphics, bitmap, rect);
                DrawTextCentered(&graphics, progress, rect);
                UpdateWindow(hWnd);
                Sleep(100);
        }
        // close GDI+, window, and graphics, Image objects
        Gdiplus::GdiplusShutdown(gdiplusToken);
        DestroyWindow(hWnd);
        return 0;
}
*/