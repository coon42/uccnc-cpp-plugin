# Native C++ binding plugin for UCCNC

### Description ###
- This UCCNC plugin allows implementing plugins in native C++ instead of using any .NET language.
- The .NET assembly which is loaded by UCCNC only acts as a proxy between C++ dll and UCCNC .NET plugin interface
- The assembly loads the C++ dll after it got loaded by UCCNC

### How to use ###
- open **uccnc-cpp-plugin.sln** and compile it using Visual Studio 2017
- switch to UCCNC installation directory (Usually **C:\UCCNC\Plugins**)
- create cpp directory (if it does not exist already)
- copy **example.dll** from **Debug** directory to **C:\UCCNC\Plugins\cpp**
- copy **plugin_proxy.dll** to **C:\UCCNC\Plugins** directory
- rename **plugin_proxy.dll** to **example_proxy.dll**
  - The assembly will load a cpp plugin depending of its name. If it is called **plugin_proxy.dll** it will load **plugin.dll** from cpp subdirectory. If it is called **example_proxy.dll** it will load **example.dll** etc.

### Features ###
- Use modern C++ without any feature limitations
- Dynamic reload - Unload and reload the C++ plugin without restarting UCCNC
  - This allows updating the plugin without restarting UCCNC every time
- Debug console for printf debugging
  - UCCNC has a copy protection which prevents you attaching the Visual Studio debugger to UCCNC. This makes it hard to debug your plugin. When compiling the C++ dll in debug configuration a console is attached to UCCNC so you can debug using printf.

### Reasons for using C++ ###
- The author has very few knowledge about C# / .NET but strong knowledge in C++
- The author was too lazy to learn C# just to write a plugin
- UCCNC is written in .NET 2.0. Therefore any plugin (the assembly) has also to be written in .NET 2.0. Assemblies written in higher .NET versions cannot be used. Unfortunately .NET 2.0 is so old that many libraries (like MQTT) are not available for it.
