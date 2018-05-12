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
