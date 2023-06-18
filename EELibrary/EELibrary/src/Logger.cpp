#include "pch.h"

#include "Logger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace eelib {
std::shared_ptr<spdlog::logger> Logger::_libLogger;

void Logger::Init(const char* name)
{
    if (_libLogger != nullptr)
        return;
    if (spdlog::get(name) != nullptr)
        return;

#ifdef _DEBUG
    if (GetConsoleWindow() == NULL)
        AllocConsole();

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleTitleA("EE Library Debug Console");

    // freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    // freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    // freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);

    // Create thread to force console to be topmost
    // TODO: Hook lol
    CreateThread(
        NULL, NULL, [](LPVOID) -> DWORD {
            bool visible = false;

            while (true) {
                if (GetAsyncKeyState(VK_F5) & 1)
                    visible = !visible;

                if (!visible) {
                    HWND consoleWindow = GetConsoleWindow();
                    if (consoleWindow != NULL) {
                        // Set the console as FUllscreen overlay and topmost and semi-transparent
                        SetWindowLong(consoleWindow, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
                        SetLayeredWindowAttributes(consoleWindow, 0, static_cast<BYTE>(0.70 * 255), LWA_ALPHA);
                        SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, NULL);
                        SetWindowPos(consoleWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
                    }
                } else {
                    ShowWindow(GetConsoleWindow(), SW_HIDE);
                }
                Sleep(500);
            }
            return 0;
        },
        NULL, NULL, NULL);
#endif

    _libLogger = spdlog::stdout_color_mt(name, spdlog::color_mode::automatic);
    spdlog::set_default_logger(_libLogger);
    _libLogger->set_pattern("[%H:%M:%S] [%n] [%^%L%$] [thread %t] %v");
    _libLogger->set_level(spdlog::level::trace);
    _libLogger->sinks().push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("Mods/Library.log", (1024 * 1024) * 5, 3, false));
}

void Logger::Uninit()
{
    _libLogger->flush();
#ifdef _DEBUG
    if (GetConsoleWindow() != NULL)
        FreeConsole();
#endif
}

}