/*
BSD 2-Clause License

Copyright (c) 2018, Stephan Thiele
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
