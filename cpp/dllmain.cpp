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
