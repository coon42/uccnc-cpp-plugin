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

#include "example.h"

ExamplePlugin::ExamplePlugin() : UccncPlugin(AUTHOR, PLUGIN_NAME, VERSION) {
  trace();
}

void ExamplePlugin::onFirstCycle() {
  trace();
}

void ExamplePlugin::onTick() {
  // trace();

  static int _cnt = 0;

  if (_cnt++ == 5) {
    char pBuf[256];
    UC.getGcodeFileName(pBuf, sizeof(pBuf));

    dbg("Gcode file name: %s\n", pBuf);
    dbg("Coordinates: \n"
      "X=%f, Y=%f, Z=%f\n"
      "A=%f, B=%f, C=%f\n",
      UC.getXpos(), UC.getYpos(), UC.getZpos(),
      UC.getApos(), UC.getBpos(), UC.getCpos());

    _cnt = 0;
  }
}

void ExamplePlugin::onShutdown() {
  trace();
}

void ExamplePlugin::buttonPressEvent(UccncButton button, bool onScreen) {
  trace();
}

void ExamplePlugin::textFieldClickEvent(UccncField label, bool isMainScreen) {
  trace();
}

void ExamplePlugin::textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText) {
  trace();
}

bool UccncPlugin::create() {
  return UccncPlugin::_create<ExamplePlugin>();
}
