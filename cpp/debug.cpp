#include <windows.h>
#include <fstream>

static FILE* _pDebugStream{ nullptr };

static bool _isConsoleAttached() {
  if (HWND hWnd = GetConsoleWindow())
    return true;

  return false;
}

static int adjustConsoleCount(int ammount) {
  int numConsoles;
  char pBuf[256];
  GetEnvironmentVariable("num_cpp_plugins", pBuf, sizeof(pBuf));

  if (GetLastError() == ERROR_ENVVAR_NOT_FOUND)
    numConsoles = ammount > 0 ? 1 : 0;
  else
    numConsoles = atoi(pBuf) + ammount;

  _itoa_s(numConsoles, pBuf, sizeof(pBuf), 10);
  SetEnvironmentVariable("num_cpp_plugins", pBuf);

  return numConsoles;
}

static int increaseConsoleCount() {
  return adjustConsoleCount(+1);
}

static int decreaseConsoleCount() {
  return adjustConsoleCount(-1);
}

int attachDebugConsole() {
  if (!_isConsoleAttached()) {
    AllocConsole();

    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 500;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    freopen_s(&_pDebugStream, "CONOUT$", "w", stdout);
  }

  return increaseConsoleCount();
}

int detachDebugConsole() {
  int numConsolesLeft = decreaseConsoleCount();

  if (!numConsolesLeft) {
    // TODO: set stdout to some empty stream, which does not crash when calling printf after this!?
    if (_pDebugStream)
      fclose(_pDebugStream);

    FreeConsole();
  }

  return numConsolesLeft;
}
