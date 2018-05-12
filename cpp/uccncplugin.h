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

#ifndef _UCCNCPLUGIN_H
#define _UCCNCPLUGIN_H

#include <windows.h>
#include <string>
#include "singleton.h"
#include "uccnc_types.h"
#include "debug.h"

using namespace std;

// These function definitions do only exist so they can be used in decltype.
// None of these functions is meant to be implemented:

extern "C" {
  bool   __cdecl _isMoving();
  void   __cdecl _getField(char* pResult, int resultBufLen, bool isAS3, UccncField field);
  int    __cdecl _getFieldInt(bool isAS3, UccncField field);
  double __cdecl _getFieldDouble(bool isAS3, UccncField field);
  bool   __cdecl _GetLed(UccncLed led);
  double __cdecl _getXpos();
  double __cdecl _getYpos();
  double __cdecl _getZpos();
  double __cdecl _getApos();
  double __cdecl _getBpos();
  double __cdecl _getCpos();
  void   __cdecl _getGcodeFileName(char* pFileName, int fileNameBufLen);
  void   __cdecl _code(char* pCode);
}

struct PluginInterfaceEntry {
  decltype(_getField)*         getField;
  decltype(_getFieldInt)*      getFieldInt;
  decltype(_getFieldDouble)*   getFieldDouble;
  decltype(_GetLed)*           getLed;
  decltype(_isMoving)*         isMoving;
  decltype(_getXpos)*          getXpos;
  decltype(_getYpos)*          getYpos;
  decltype(_getZpos)*          getZpos;
  decltype(_getApos)*          getApos;
  decltype(_getBpos)*          getBpos;
  decltype(_getCpos)*          getCpos;
  decltype(_getGcodeFileName)* getGcodeFileName;
  decltype(_code)*             code;
};

class UccncPlugin : public FactorySingleton<UccncPlugin> {
public:
  static bool create(); // Implement this method in your plugins .cpp file.

  // Example:
  //
  //----------------------------------------
  // class YourPlugin : public UccncPlugin {
  // ...
  // }
  // ---------------------------------------
  //
  // ---------------------------------------
  // bool UccncPlugin::create() {
  //   return UccncPlugin::_create<YourPlugin>();
  // }
  // ---------------------------------------

  void debugSetDebugConsoleTitle(int numPluginsLoaded) {
    char pBuf[256];
    sprintf_s(pBuf, sizeof(pBuf), "UCCNC C++ plugin debug console. Loaded plugins: %d\n", numPluginsLoaded);
    SetConsoleTitle(pBuf);
  }

  UccncPlugin(const string& author, const string& pluginName, const string& pluginVersion) {
#ifdef _DEBUG
    int numPluginsLoaded = attachDebugConsole();

    // Disable close button of console windows to prevent closing UCCNC accidentally:
    HWND hwnd = GetConsoleWindow();
    HMENU hmenu = GetSystemMenu(hwnd, FALSE);
    EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

    debugSetDebugConsoleTitle(numPluginsLoaded);
    dbg("Build: " __DATE__ ", " __TIME__ "\n");
#endif // _DEBUG

    trace();

    author_ = author;
    pluginName_ = pluginName;
    pluginVersion_ = pluginVersion;
  }

  virtual ~UccncPlugin() = 0 {
    trace();

#ifdef _DEBUG
    int numPluginsLoaded = detachDebugConsole();
    debugSetDebugConsoleTitle(numPluginsLoaded);
#endif // _DEBUG
  }

  void getPropertiesEvent(char* pAuthor, char* pPluginName, char* pPluginVersion) {
    trace();

    strcpy_s(pAuthor, 256, author_.c_str());
    strcpy_s(pPluginName, 256, pluginName_.c_str());
    strcpy_s(pPluginVersion, 256, pluginVersion_.c_str());
  }

  virtual void onFirstCycle() = 0;
  virtual void onTick() = 0;
  virtual void onShutdown() = 0;
  virtual void buttonPressEvent(UccncButton button, bool onScreen) = 0;
  virtual void textFieldClickEvent(UccncField label, bool isMainScreen) = 0;
  virtual void textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText) = 0;

  void setCallBacks(PluginInterfaceEntry uc) {
    trace();
    UC = uc;
  }

  PluginInterfaceEntry UC{ 0 };

private:
  string author_;
  string pluginName_;
  string pluginVersion_;
};

#endif // _UCCNCPLUGIN_H

