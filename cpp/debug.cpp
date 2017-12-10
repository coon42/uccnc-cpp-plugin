#include <windows.h>
#include <fstream>

static bool _isConsoleAttached = false;
static FILE* _pDebugStream{ nullptr };

void attachDebugConsole() {
  if (!_isConsoleAttached) {
    AllocConsole();

    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 500;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    freopen_s(&_pDebugStream, "CONOUT$", "w", stdout);

    _isConsoleAttached = true;
  }
}

void detachDebugConsole() {
  if (_isConsoleAttached) {
    fclose(_pDebugStream);

    // TODO: set stdout to some empty stream, which does not crash when calling printf after this!?

    FreeConsole();

    _isConsoleAttached = false;
  }
}
