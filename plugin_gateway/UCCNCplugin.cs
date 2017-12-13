using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Reflection;

namespace Plugins {
  public class UCCNCplugin { // Class name must be UCCNCplugin to work!
    public class CppDll {
      static class Kernel32 {
        [DllImport("kernel32.dll")]
        public static extern IntPtr LoadLibrary(string dllToLoad);

        [DllImport("kernel32.dll")]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

        [DllImport("kernel32.dll")]
        public static extern bool FreeLibrary(IntPtr hModule);
      }

      public CppDll() {
          string assemblyPath = Assembly.GetExecutingAssembly().Location;
          string assemblyName = assemblyPath.Substring(assemblyPath.LastIndexOf('/') + 1);
          string cppDllName = assemblyName.Substring(0, assemblyName.LastIndexOf('_')) + ".dll";
          string absoluteDllPath = assemblyPath.Substring(0, assemblyPath.LastIndexOf('/')) +
              @"/cpp/" + cppDllName;

          path = absoluteDllPath;
      }

      ~CppDll() {
        Unload();
      }

      public bool Load() {
        if (pDll != IntPtr.Zero)
          return false;

        pDll = Kernel32.LoadLibrary(path);

        if (pDll == IntPtr.Zero) {
          MessageBox.Show(String.Format("Failed to load {0}!", path), "Error on loading cpp dll");

          return false;
        }

        onFirstCycle = (OnFirstCycle_t)Marshal.GetDelegateForFunctionPointer(
          Kernel32.GetProcAddress(pDll, "onFirstCycle"), typeof(OnFirstCycle_t));

        onTick = (OnTick_t)Marshal.GetDelegateForFunctionPointer(
            Kernel32.GetProcAddress(pDll, "onTick"), typeof(OnTick_t));

        onShutdown = (OnShutdown_t)Marshal.GetDelegateForFunctionPointer(
            Kernel32.GetProcAddress(pDll, "onShutdown"), typeof(OnShutdown_t));

        buttonpress_event = (Buttonpress_event_t)Marshal.GetDelegateForFunctionPointer(
            Kernel32.GetProcAddress(pDll, "buttonpress_event"), typeof(Buttonpress_event_t));

        textfieldclick_event = (Textfieldclick_event_t)Marshal.GetDelegateForFunctionPointer(
            Kernel32.GetProcAddress(pDll, "textfieldclick_event"), typeof(Textfieldclick_event_t));

        textfieldtexttyped_event = (Textfieldtexttyped_event_t)Marshal.GetDelegateForFunctionPointer(
            Kernel32.GetProcAddress(pDll, "textfieldtexttyped_event"), typeof(Textfieldtexttyped_event_t));

        getproperties_event = (Getproperties_event_t)Marshal.GetDelegateForFunctionPointer(
            Kernel32.GetProcAddress(pDll, "getproperties_event"), typeof(Getproperties_event_t));

        setCallBacks = (SetCallBacks_t)Marshal.GetDelegateForFunctionPointer(
            Kernel32.GetProcAddress(pDll, "setCallBacks"), typeof(SetCallBacks_t));

        return true;
      }

      public bool Unload() {
        bool success = Kernel32.FreeLibrary(pDll);
        pDll = IntPtr.Zero;

        return success;
      }

      public bool IsLoaded() {
        return pDll != IntPtr.Zero;
      }

      private string path;
      private IntPtr pDll = IntPtr.Zero;

      public OnFirstCycle_t onFirstCycle;
      public OnTick_t onTick;
      public OnShutdown_t onShutdown;
      public Buttonpress_event_t buttonpress_event;
      public Textfieldclick_event_t textfieldclick_event;
      public Textfieldtexttyped_event_t textfieldtexttyped_event;
      public Getproperties_event_t getproperties_event;
      public SetCallBacks_t setCallBacks;

      [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
      public delegate bool OnFirstCycle_t();

      [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
      public delegate bool OnTick_t();

      [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
      public delegate bool OnShutdown_t();

      [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
      public delegate void Buttonpress_event_t(int buttonnumber, bool onscreen);

      [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
      public delegate void Textfieldclick_event_t(int labelnumber, bool Ismainscreen);

      [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
      public delegate void Textfieldtexttyped_event_t(int labelnumber, bool Ismainscreen, StringBuilder text);

      [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
      public delegate void Getproperties_event_t(StringBuilder author, StringBuilder pluginName,
          StringBuilder pluginVersion);

      [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
      public delegate void SetCallBacks_t(PluginInterfaceEntry pInterface);
    }

    // Do loading and unloading of dll in a safe state to avoid unloading while code in dll is executed.
    // Else this meight result in a crash:

    enum DllDelayedOperation {
      None,
      Load,
      Unload
    };

    DllDelayedOperation delayedDllOperation;

    public void loadDllAsync() {
      delayedDllOperation = DllDelayedOperation.Load;
    }

    public void unloadDllAsync() {
      delayedDllOperation = DllDelayedOperation.Unload;
    }

    public unsafe void unsafeStrCpy(byte* pDst, int dstBufLen, String src) {
      // TOOD: how to do the string copy right!?
      int len = src.Length < dstBufLen - 1 ? src.Length : dstBufLen - 1;

      for (int i = 0; i < len; i++)
        pDst[i] = (byte)src[i];

      pDst[len] = (byte)'\0';
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate bool IsMovingCallBack();
    private unsafe bool IsMovingHandler() {
      return UC.IsMoving();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public unsafe delegate void GetFieldCallBack(byte* pResult, int resultBufLen, bool isAS3, int fieldnumber);
    private unsafe void GetFieldHandler(byte* pField, int fieldBufLen, bool isAS3, int fieldnumber) {
      string result = UC.Getfield(isAS3, fieldnumber);
      unsafeStrCpy(pField, fieldBufLen, result);
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int GetFieldIntCallBack(bool isAS3, int fieldnumber);
    private int GetFieldIntHandler(bool isAS3, int fieldnumber) {
      return UC.Getfieldint(isAS3, fieldnumber);
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetFieldDoubleCallBack(bool isAS3, int fieldnumber);
    private double GetFieldDoubleHandler(bool isAS3, int fieldnumber) {
      return UC.Getfielddouble(isAS3, fieldnumber);
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate bool GetLedCallBack(int ledNumber);
    private bool GetLedHandler(int ledNumber) {
      return UC.GetLED(ledNumber);
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetXposCallBack();
    private double GetXposHandler() {
      return UC.GetXpos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetYposCallBack();
    private double GetYposHandler() {
      return UC.GetYpos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetZposCallBack();
    private double GetZposHandler() {
      return UC.GetZpos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetAposCallBack();
    private double GetAposHandler() {
      return UC.GetApos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetBposCallBack();
    private double GetBposHandler() {
      return UC.GetBpos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate double GetCposCallBack();
    private double GetCposHandler() {
      return UC.GetCpos();
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public unsafe delegate void GetGetgcodefilenameCallBack(byte* pFileName, int fileNameBufLen);
    private unsafe void GetgcodefilenameHandler(byte* pFileName, int fileNameBufLen) {
      string fileName = UC.Getgcodefilename();
      unsafeStrCpy(pFileName, fileNameBufLen, fileName);
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct PluginInterfaceEntry {
      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetFieldCallBack pGetField;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetFieldIntCallBack pGetFieldInt;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetFieldDoubleCallBack pGetFieldDouble;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetLedCallBack pGetLed;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public IsMovingCallBack pIsMoving;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetXposCallBack pGetXpos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetYposCallBack pGetYpos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetZposCallBack pGetZpos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetAposCallBack pGetApos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetBposCallBack pGetBpos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetCposCallBack pGetCpos;

      [MarshalAs(UnmanagedType.FunctionPtr)]
      public GetGetgcodefilenameCallBack pGetGcodeFileName;
    }

    // Use instance variable if PluginInterfaceEntry to ensure the delegates don't get garbage collected:
    private PluginInterfaceEntry uc_callbacks;
    public Plugininterface.Entry UC;
    PluginForm pluginForm;
    public bool isFirstCycle = true;
    public bool loopstop = false;
    public CppDll cppDll;

    public unsafe UCCNCplugin() {
      cppDll = new CppDll();
      delayedDllOperation = DllDelayedOperation.None;

      uc_callbacks = new PluginInterfaceEntry();
      uc_callbacks.pGetField = new GetFieldCallBack(GetFieldHandler);
      uc_callbacks.pGetFieldInt = new GetFieldIntCallBack(GetFieldIntHandler);
      uc_callbacks.pGetFieldDouble = new GetFieldDoubleCallBack(GetFieldDoubleHandler);
      uc_callbacks.pGetLed = new GetLedCallBack(GetLedHandler);
      uc_callbacks.pIsMoving = new IsMovingCallBack(IsMovingHandler);
      uc_callbacks.pGetXpos = new GetXposCallBack(GetXposHandler);
      uc_callbacks.pGetYpos = new GetYposCallBack(GetYposHandler);
      uc_callbacks.pGetZpos = new GetZposCallBack(GetZposHandler);
      uc_callbacks.pGetApos = new GetAposCallBack(GetAposHandler);
      uc_callbacks.pGetBpos = new GetBposCallBack(GetBposHandler);
      uc_callbacks.pGetCpos = new GetCposCallBack(GetCposHandler);
      uc_callbacks.pGetGcodeFileName = new GetGetgcodefilenameCallBack(GetgcodefilenameHandler);

      // Do not load dll asynchronously at this point or Getproperties_event() call will never reach cpp dll:
      cppDll.Load();
      cppDll.setCallBacks(uc_callbacks);
    }

    // Called when the plugin is initialised.
    // The parameter is the Plugin interface object which contains all functions prototypes for calls and callbacks.
    public void Init_event(Plugininterface.Entry UC) {
      this.UC = UC;
      pluginForm = new PluginForm(this);

      String value = UC.Readkey("Plugins_enabled", "msgflo.dll", "False");
      
      if (value.ToLower() == "true")
        cppDll.Load();
    }

    // Called when the plugin is loaded, the author of the plugin should set the details of the plugin here.
    public Plugininterface.Entry.Pluginproperties Getproperties_event(Plugininterface.Entry.Pluginproperties Properties) {
      StringBuilder author = new StringBuilder(256);
      StringBuilder pluginName = new StringBuilder(256);
      StringBuilder pluginVersion = new StringBuilder(256);

      author.Append("---");
      pluginName.Append("---");
      pluginVersion.Append("---");

      if (cppDll.IsLoaded()) {
        cppDll.getproperties_event(author, pluginName, pluginVersion);
        cppDll.Unload();
      }
           
      Properties.author = author.ToString();
      Properties.pluginname = pluginName.ToString(); ;
      Properties.pluginversion = pluginVersion.ToString();
      return Properties;
    }

    // Called from UCCNC when the user presses the Configuration button in the Plugin configuration menu.
    // Typically the plugin configuration window is shown to the user.

    public void Configure_event() {
      ConfigForm cform = new ConfigForm();
      cform.ShowDialog();
    }

    // Called from UCCNC when the plugin is loaded and started.
    public void Startup_event() {
      if (pluginForm.IsDisposed)
        pluginForm = new PluginForm(this);

      pluginForm.Show();
    }

    // Called when the Pluginshowup(string Pluginfilename); function is executed in the UCCNC.

    public void Showup_event() {
      if (pluginForm.IsDisposed)
        pluginForm = new PluginForm(this);

      pluginForm.Show();
      pluginForm.BringToFront();
    }

    // Called when the UCCNC software is closing.
    public void Shutdown_event() {
      try {
        loopstop = true;

        if (cppDll.IsLoaded())
          cppDll.onShutdown();

        pluginForm.Close();
      }
      catch (Exception) {
        MessageBox.Show("Exception in c++ plugin!", "Error in Shutdown_event");
      }
    }

    public void processDelayedDllOperation() {
      switch(delayedDllOperation) {
        case DllDelayedOperation.Load:
          if (!cppDll.Load())
            MessageBox.Show("Loading cpp dll failed!");

          cppDll.setCallBacks(uc_callbacks);
          isFirstCycle = true;
          break;

        case DllDelayedOperation.Unload:
          if(!cppDll.Unload())
            MessageBox.Show("Unloading cpp dll failed!");

          break;
      }

      delayedDllOperation = DllDelayedOperation.None;
    }

    // Called in a loop with a 25Hz interval.
    public void Loop_event()  {
      if (loopstop)
        return;

      processDelayedDllOperation();

      if (!cppDll.IsLoaded())
        return;

      if (pluginForm == null || pluginForm.IsDisposed)
        return;

      if (isFirstCycle) {
        isFirstCycle = false;
        cppDll.onFirstCycle();
      }

      try {
        cppDll.onTick();
      }
      catch (Exception e) {
        MessageBox.Show(String.Format("Exception in c++ plugin!\n {0}", e.StackTrace),
            "Error in Loop_event");
      }
    }

    // This is a direct function call addressed to this plugin dll
    // The function can be called by macros or by another plugin
    // The passed parameter is an object and the return value is also an object

    public object Informplugin_event(object Message) {
      if (!(pluginForm == null || pluginForm.IsDisposed)) {
        if (Message is string) {
          string receivedstr = Message as string;
          MessageBox.Show(this.pluginForm, "Informplugin message received by UCCNC cpp plugin! Message was: "
              + receivedstr);
        }
      }

      string returnstr = "Return string by UCCNC cpp plugin";
      return (object)returnstr;
    }

    // This is a function call made to all plugin dll files
    // The function can be called by macros or by another plugin
    // The passed parameter is an object and there is no return value

    public void Informplugins_event(object Message) {
      if (!(pluginForm == null || pluginForm.IsDisposed)) {
        string receivedstr = Message as string;
        MessageBox.Show(this.pluginForm, "Informplugins message received by UCCNC cpp plugin! Message was: "
            + receivedstr);
      }
    }

    // Called when the user presses a button on the UCCNC GUI or if a Callbutton function is executed.
    // The int buttonnumber parameter is the ID of the caller button.
    // The bool onscreen parameter is true if the button was pressed on the GUI and is false if the Callbutton
    // function was called.

    public void Buttonpress_event(int buttonnumber, bool onscreen) {
      if(cppDll.IsLoaded())
        cppDll.buttonpress_event(buttonnumber, onscreen);
    }

    // Called when the user clicks and enters a Textfield on the screen
    // The labelnumber parameter is the ID of the accessed Textfield
    // The bool Ismainscreen parameter is true is the Textfield is on the main screen and false if it is on the
    // jog screen

    public void Textfieldclick_event(int labelnumber, bool Ismainscreen) {
      if (cppDll.IsLoaded())
        cppDll.textfieldclick_event(labelnumber, Ismainscreen);
    }

    // Called when the user enters text into the Textfield and it gets validated
    // The labelnumber parameter is the ID of the accessed Textfield
    // The bool Ismainscreen parameter is true is the Textfield is on the main screen and false if it is on the jog screen.
    // The text parameter is the text entered and validated by the user

    public void Textfieldtexttyped_event(int labelnumber, bool Ismainscreen, string text) {
      StringBuilder sbText = new StringBuilder(text);

      if (cppDll.IsLoaded())
        cppDll.textfieldtexttyped_event(labelnumber, Ismainscreen, sbText);
    }
  }
}
