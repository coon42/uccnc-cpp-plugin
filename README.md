# C++ binding plugin for UCCNC

### Description ###
- This UCCNC plugin plugins allows implementing plugins in C++ instead of using any .NET language.
- The .NET assembly which is loaded by UCCNC only acts as a gateway between C++ dll and .NET plugin interface
- The assembly loads the C++ dll after it got loaded by UCCNC

### How to use ###
- Open **uccnc-cpp-plugin.sln** and compile it using Visual Studio 2017
- copy **example.dll** to **Plugins/cpp** directory (create **cpp** subdirectory, if it does not exist yet)
- copy **plugin_gateway.dll** to **Plugins** directory
- rename **plugin_gateway.dll** to **example_gateway.dll**
  - The assembly will load a cpp plugin depending of its name. If it is called **plugin_gateway.dll** it will load **plugin.dll** from cpp subdirectory. If it is called **example_gateway.dll** it will load **example.dll** etc.

### Features ###
- Dynamic reload - Unload and reload the plugin without restarting UCCNC
- Use modern C++ without any feature limitations
- Debug using printf and an attached console
  - UCCNC has a copyprotection which prevents you attaching the visual studio debugger to UCCNC. This makes it hard to debug your plugin. When compiling the C++ dll in debug configuration a console is attached to UCCNC so you can debug using printf.

### Reasons for using C++ ###
- The author has very few knowledge about C# / .NET but strong knowledge in C++
- The author was too lazy to learn C# just to write a plugin
- UCCNC is written in .NET 2.0. Therefore any plugin (the assembly) has also to be written in .NET 2.0. Assemblies written in higher .NET versions cannot be used. Unfortunately .NET 2.0 is so old that many libraries (like MQTT) are not available for it.
