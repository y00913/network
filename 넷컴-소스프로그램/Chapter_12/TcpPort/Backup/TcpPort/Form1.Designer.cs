namespace TcpPort
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
            this.txt_info = new System.Windows.Forms.TextBox();
            this.txt_out = new System.Windows.Forms.TextBox();
            this.btn_search = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.txt_ip = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // txt_info
            // 
            this.txt_info.Location = new System.Drawing.Point(196, 84);
            this.txt_info.Multiline = true;
            this.txt_info.Name = "txt_info";
            this.txt_info.Size = new System.Drawing.Size(224, 112);
            this.txt_info.TabIndex = 9;
            this.txt_info.Text = "*** 검색 포트 진행 사항 ***";
            // 
            // txt_out
            // 
            this.txt_out.Location = new System.Drawing.Point(12, 84);
            this.txt_out.Multiline = true;
            this.txt_out.Name = "txt_out";
            this.txt_out.Size = new System.Drawing.Size(176, 112);
            this.txt_out.TabIndex = 8;
            this.txt_out.Text = "*** 활성화된 포트 정보 ***";
            // 
            // btn_search
            // 
            this.btn_search.Location = new System.Drawing.Point(236, 44);
            this.btn_search.Name = "btn_search";
            this.btn_search.Size = new System.Drawing.Size(88, 23);
            this.btn_search.TabIndex = 7;
            this.btn_search.Text = "검출";
            this.btn_search.Click += new System.EventHandler(this.btn_search_Click);
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label1.ForeColor = System.Drawing.SystemColors.HotTrack;
            this.label1.Location = new System.Drawing.Point(12, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(200, 23);
            this.label1.TabIndex = 6;
            this.label1.Text = "활성화된 포트번호 검출";
            // 
            // txt_ip
            // 
            this.txt_ip.Location = new System.Drawing.Point(20, 44);
            this.txt_ip.Name = "txt_ip";
            this.txt_ip.Size = new System.Drawing.Size(200, 21);
            this.txt_ip.TabIndex = 5;
            this.txt_ip.Text = "127.0.0.1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(429, 215);
            this.Controls.Add(this.txt_info);
            this.Controls.Add(this.txt_out);
            this.Controls.Add(this.btn_search);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txt_ip);
            this.Name = "Form1";
            this.Text = "포트 검출";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txt_info;
        private System.Windows.Forms.TextBox txt_out;
        private System.Windows.Forms.Button btn_search;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txt_ip;
    }
}

