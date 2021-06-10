namespace ChatServer
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
            this.button_Start = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // text_IP
            // 
            this.text_IP.Location = new System.Drawing.Point(6, 50);
            this.text_IP.Name = "text_IP";
            this.text_IP.Size = new System.Drawing.Size(174, 21);
            this.text_IP.TabIndex = 0;
            // 
            // text_Port
            // 
            this.text_Port.Location = new System.Drawing.Point(206, 50);
            this.text_Port.Name = "text_Port";
            this.text_Port.Size = new System.Drawing.Size(74, 21);
            this.text_Port.TabIndex = 1;
            this.text_Port.Text = "7007";
            // 
            // button_Start
            // 
            this.button_Start.Location = new System.Drawing.Point(8, 103);
            this.button_Start.Name = "button_Start";
            this.button_Start.Size = new System.Drawing.Size(271, 26);
            this.button_Start.TabIndex = 4;
            this.button_Start.Text = "채팅서버 시작";
            this.button_Start.UseVisualStyleBackColor = true;
            this.button_Start.Click += new System.EventHandler(this.button_Start_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "서버 주소";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(206, 18);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "포트 번호";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 145);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button_Start);
            this.Controls.Add(this.text_Port);
            this.Controls.Add(this.text_IP);
            this.Name = "Form1";
            this.Text = "채팅 서버";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox text_IP;
        private System.Windows.Forms.TextBox text_Port;
        private System.Windows.Forms.Button button_Start;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}

