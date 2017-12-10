#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include "../uccncplugin.h"

#define AUTHOR      "coon@c-base.org"
#define PLUGIN_NAME "Example C++ plugin"
#define VERSION     "1.0000"

class ExamplePlugin : public UccncPlugin {
public:
  ExamplePlugin();

private:
  virtual void onFirstCycle();
  virtual void onTick();
  virtual void onShutdown();
  virtual void buttonPressEvent(UccncButton button, bool onScreen);
  virtual void textFieldClickEvent(UccncField label, bool isMainScreen);
  virtual void textFieldTextTypedEvent(UccncField label, bool isMainScreen, const char* pText);
};

#endif // !_EXAMPLE_H
