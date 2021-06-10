namespace ChatClient
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
            this.text_IP = new System.Windows.Forms.TextBox();
            this.text_Port = new System.Windows.Forms.TextBox();
            this.text_Name = new System.Windows.Forms.TextBox();
            this.text_Info = new System.Windows.Forms.TextBox();
            this.text_Input = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.button_Connect = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // text_IP
            // 
            this.text_IP.Location = new System.Drawing.Point(5, 35);
            this.text_IP.Name = "text_IP";
            this.text_IP.Size = new System.Drawing.Size(225, 21);
            this.text_IP.TabIndex = 0;
            // 
            // text_Port
            // 
            this.text_Port.Location = new System.Drawing.Point(236, 35);
            this.text_Port.Name = "text_Port";
            this.text_Port.Size = new System.Drawing.Size(85, 21);
            this.text_Port.TabIndex = 1;
            this.text_Port.Text = "7007";
            // 
            // text_Name
            // 
            this.text_Name.Location = new System.Drawing.Point(54, 72);
            this.text_Name.Name = "text_Name";
            this.text_Name.Size = new System.Drawing.Size(176, 21);
            this.text_Name.TabIndex = 2;
            // 
            // text_Info
            // 
            this.text_Info.Location = new System.Drawing.Point(5, 108);
            this.text_Info.Multiline = true;
            this.text_Info.Name = "text_Info";
            this.text_Info.ReadOnly = true;
            this.text_Info.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.text_Info.Size = new System.Drawing.Size(316, 224);
            this.text_Info.TabIndex = 3;
            // 
            // text_Input
            // 
            this.text_Input.Location = new System.Drawing.Point(5, 364);
            this.text_Input.Name = "text_Input";
            this.text_Input.Size = new System.Drawing.Size(316, 21);
            this.text_Input.TabIndex = 4;
            this.text_Input.KeyDown += new System.Windows.Forms.KeyEventHandler(this.text_Input_KeyDown);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "서버 IP";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(234, 10);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "포트 번호";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 349);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "메시지 입력";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 81);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 12);
            this.label4.TabIndex = 8;
            this.label4.Text = "이름";
            // 
            // button_Connect
            // 
            this.button_Connect.Location = new System.Drawing.Point(239, 72);
            this.button_Connect.Name = "button_Connect";
            this.button_Connect.Size = new System.Drawing.Size(81, 20);
            this.button_Connect.TabIndex = 9;
            this.button_Connect.Text = "연결";
            this.button_Connect.UseVisualStyleBackColor = true;
            this.button_Connect.Click += new System.EventHandler(this.button_Connect_Click);
            // 
            // Form1
            // 
            this.ClientSize = new System.Drawing.Size(326, 388);
            this.Controls.Add(this.button_Connect);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.text_Input);
            this.Controls.Add(this.text_Info);
            this.Controls.Add(this.text_Name);
            this.Controls.Add(this.text_Port);
            this.Controls.Add(this.text_IP);
            this.Name = "Form1";
            this.Text = "채팅 클라이언트";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox text_IP;
        private System.Windows.Forms.TextBox text_Port;
        private System.Windows.Forms.TextBox text_Name;
        private System.Windows.Forms.TextBox text_Info;
        private System.Windows.Forms.TextBox text_Input;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button_Connect;
    }
}

