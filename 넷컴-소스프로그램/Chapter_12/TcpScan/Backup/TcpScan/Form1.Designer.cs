namespace TcpScan
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.btn_start = new System.Windows.Forms.Button();
            this.txt_info = new System.Windows.Forms.TextBox();
            this.btn_exit = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btn_start
            // 
            this.btn_start.Location = new System.Drawing.Point(28, 244);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(136, 23);
            this.btn_start.TabIndex = 5;
            this.btn_start.Text = "서버 감시 시작";
            this.btn_start.Click += new System.EventHandler(this.btn_start_Click);
            // 
            // txt_info
            // 
            this.txt_info.Location = new System.Drawing.Point(12, 12);
            this.txt_info.Multiline = true;
            this.txt_info.Name = "txt_info";
            this.txt_info.ReadOnly = true;
            this.txt_info.Size = new System.Drawing.Size(328, 224);
            this.txt_info.TabIndex = 4;
            // 
            // btn_exit
            // 
            this.btn_exit.Location = new System.Drawing.Point(196, 244);
            this.btn_exit.Name = "btn_exit";
            this.btn_exit.Size = new System.Drawing.Size(128, 23);
            this.btn_exit.TabIndex = 3;
            this.btn_exit.Text = "프로그램 종료";
            this.btn_exit.Click += new System.EventHandler(this.btn_exit_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(353, 278);
            this.Controls.Add(this.btn_start);
            this.Controls.Add(this.txt_info);
            this.Controls.Add(this.btn_exit);
            this.Name = "Form1";
            this.Text = "서버 감시";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_start;
        private System.Windows.Forms.TextBox txt_info;
        private System.Windows.Forms.Button btn_exit;
    }
}

