#include <windows.h>
#include "uccncplugin.h"

static UccncPlugin* _pUccncPlugin = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      _pUccncPlugin = UccncPlugin::create();
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      if (_pUccncPlugin) {
        delete _pUccncPlugin;

        _pUccncPlugin = nullptr;
      }

      break;
  }

  return TRUE;
}

extern "C" {
  __declspec(dllexport)
  void __cdecl setCallBacks(PluginInterfaceEntry uc) {
    return _pUccncPlugin->setCallBacks(uc);
  }

  __declspec(dllexport)
  void __cdecl onFirstCycle() {
    return _pUccncPlugin->onFirstCycle();
  }

  __declspec(dllexport)
  void __cdecl onTick() {
    return _pUccncPlugin->onTick();
  }

  __declspec(dllexport)
  void __cdecl onShutdown() {
    return _pUccncPlugin->onShutdown();
  }

  __declspec(dllexport)
  void __cdecl buttonpress_event(UccncButton button, bool onScreen) {
    _pUccncPlugin->buttonPressEvent(button, onScreen);
  }

  __declspec(dllexport)
  void __cdecl textfieldclick_event(UccncField label, bool isMainScreen) {
    _pUccncPlugin->textFieldClickEvent(label, isMainScreen);
  }

  __declspec(dllexport)
  void __cdecl textfieldtexttyped_event(UccncField label, bool isMainScreen, const char* pText) {
    _pUccncPlugin->textFieldTextTypedEvent(label, isMainScreen, pText);
  }

  __declspec(dllexport)
  void __cdecl getproperties_event(char* pAuthor, char* pPluginName, char* pPluginVersion) {
    _pUccncPlugin->getPropertiesEvent(pAuthor, pPluginName, pPluginVersion);
  }
}
