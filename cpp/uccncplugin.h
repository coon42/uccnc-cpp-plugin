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
}

struct PluginInterfaceEntry {
  decltype(_getField)*       pGetField;
  decltype(_getFieldInt)*    pGetFieldInt;
  decltype(_getFieldDouble)* pGetFieldDouble;
  decltype(_GetLed)*         pGetLed;
  decltype(_isMoving)*       pIsMoving;
  decltype(_getXpos)*        pCgetXpos;
  decltype(_getYpos)*        pCgetYpos;
  decltype(_getZpos)*        pCgetZpos;
  decltype(_getApos)*        pCgetApos;
  decltype(_getBpos)*        pCgetBpos;
  decltype(_getCpos)*        pCgetCpos;
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

  UccncPlugin(const string& author, const string& pluginName, const string& pluginVersion) {
#ifdef _DEBUG
    attachDebugConsole();

    // Disable close button of console windows to prevent closing UCCNC accidentally:
    HWND hwnd = GetConsoleWindow();
    HMENU hmenu = GetSystemMenu(hwnd, FALSE);
    EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

    SetConsoleTitle("UCCNC C++ plugin debug console. (Build: " __DATE__ ", " __TIME__ ")\n");
#endif // _DEBUG

    trace();

    author_ = author;
    pluginName_ = pluginName;
    pluginVersion_ = pluginVersion;
  }

  virtual ~UccncPlugin() = 0 {
    trace();

#ifdef _DEBUG
    detachDebugConsole();
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

protected:
  PluginInterfaceEntry UC{ 0 };

private:
  string author_;
  string pluginName_;
  string pluginVersion_;
};

#endif // _UCCNCPLUGIN_H

