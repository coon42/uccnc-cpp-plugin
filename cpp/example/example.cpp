#include "example.h"

ExamplePlugin::ExamplePlugin() : UccncPlugin(AUTHOR, PLUGIN_NAME, VERSION) {
  trace();
}

void ExamplePlugin::onFirstCycle() {
  trace();
}

void ExamplePlugin::onTick() {
  trace();
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
