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
