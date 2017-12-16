using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace Plugins {
  public partial class PluginForm : Form {
    private Plugininterface.Entry UC;
    UCCNCplugin Pluginmain;

    public PluginForm(UCCNCplugin Pluginmain) {
      this.UC = Pluginmain.UC;
      this.Pluginmain = Pluginmain;
      InitializeComponent();
    }

    private void PluginForm_Load(object sender, EventArgs e) {
      // CheckForIllegalCrossThreadCalls = false;
      setButtons();
    }

    private void setButtons() {
      if (Pluginmain.cppDll.IsLoaded()) {
        button_load.Enabled = false;
        button_unload.Enabled = true;
      }
      else {
        button_load.Enabled = true;
        button_unload.Enabled = false;
      }
    }

    private void button_load_Click(object sender, EventArgs e) {
      Pluginmain.loadDllAsync();
      Pluginmain.isFirstCycle = true;

      // TODO: enable / disable button, when loading was successful:
      button_load.Enabled = false;
      button_unload.Enabled = true;
    }

    private void button_unload_Click(object sender, EventArgs e) {
      Pluginmain.unloadDllAsync();

      // TODO: enable / disable button, when unloading was successful:
      button_load.Enabled = true;
      button_unload.Enabled = false;
    }
  }
}
