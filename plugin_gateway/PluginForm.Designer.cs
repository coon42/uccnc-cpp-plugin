namespace Plugins
{
    partial class PluginForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
      this.button_load = new System.Windows.Forms.Button();
      this.button_unload = new System.Windows.Forms.Button();
      this.SuspendLayout();
      //
      // button_load
      //
      this.button_load.Enabled = false;
      this.button_load.Location = new System.Drawing.Point(12, 12);
      this.button_load.Name = "button_load";
      this.button_load.Size = new System.Drawing.Size(75, 23);
      this.button_load.TabIndex = 0;
      this.button_load.Text = "Load";
      this.button_load.UseVisualStyleBackColor = true;
      this.button_load.Click += new System.EventHandler(this.button_load_Click);
      //
      // button_unload
      //
      this.button_unload.Enabled = false;
      this.button_unload.Location = new System.Drawing.Point(12, 41);
      this.button_unload.Name = "button_unload";
      this.button_unload.Size = new System.Drawing.Size(75, 23);
      this.button_unload.TabIndex = 1;
      this.button_unload.Text = "Unload";
      this.button_unload.UseVisualStyleBackColor = true;
      this.button_unload.Click += new System.EventHandler(this.button_unload_Click);
      //
      // PluginForm
      //
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(301, 235);
      this.Controls.Add(this.button_unload);
      this.Controls.Add(this.button_load);
      this.Name = "PluginForm";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
      this.Text = "cpp plugin config";
      this.TopMost = true;
      this.Load += new System.EventHandler(this.PluginForm_Load);
      this.ResumeLayout(false);

        }

    #endregion

    private System.Windows.Forms.Button button_load;
    private System.Windows.Forms.Button button_unload;
  }
}