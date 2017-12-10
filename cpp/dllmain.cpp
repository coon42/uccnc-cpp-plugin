#include <windows.h>
#include "uccncplugin.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      if(!UccncPlugin::create())
        dbg("Failed creating plugin instance!\n");
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      if (!UccncPlugin::_destroy())
        dbg("Failed destroying plugin instance!\n");
      break;
  }

  return TRUE;
}

extern "C" {
  __declspec(dllexport)
  void __cdecl setCallBacks(PluginInterfaceEntry uc) {
    return UccncPlugin::_instance()->setCallBacks(uc);
  }

  __declspec(dllexport)
  void __cdecl onFirstCycle() {
    return UccncPlugin::_instance()->onFirstCycle();
  }

  __declspec(dllexport)
  void __cdecl onTick() {
    return UccncPlugin::_instance()->onTick();
  }

  __declspec(dllexport)
  void __cdecl onShutdown() {
    return UccncPlugin::_instance()->onShutdown();
  }

  __declspec(dllexport)
  void __cdecl buttonpress_event(UccncButton button, bool onScreen) {
    return UccncPlugin::_instance()->buttonPressEvent(button, onScreen);
  }

  __declspec(dllexport)
  void __cdecl textfieldclick_event(UccncField label, bool isMainScreen) {
    return UccncPlugin::_instance()->textFieldClickEvent(label, isMainScreen);
  }

  __declspec(dllexport)
  void __cdecl textfieldtexttyped_event(UccncField label, bool isMainScreen, const char* pText) {
    return UccncPlugin::_instance()->textFieldTextTypedEvent(label, isMainScreen, pText);
  }

  __declspec(dllexport)
  void __cdecl getproperties_event(char* pAuthor, char* pPluginName, char* pPluginVersion) {
    return UccncPlugin::_instance()->getPropertiesEvent(pAuthor, pPluginName, pPluginVersion);
  }
}
