namespace TimeClient
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
            this.textIP = new System.Windows.Forms.TextBox();
            this.textDate = new System.Windows.Forms.TextBox();
            this.textTime = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // textIP
            // 
            this.textIP.Location = new System.Drawing.Point(65, 22);
            this.textIP.Name = "textIP";
            this.textIP.Size = new System.Drawing.Size(223, 21);
            this.textIP.TabIndex = 0;
            // 
            // textDate
            // 
            this.textDate.Location = new System.Drawing.Point(65, 135);
            this.textDate.Name = "textDate";
            this.textDate.Size = new System.Drawing.Size(223, 21);
            this.textDate.TabIndex = 1;
            // 
            // textTime
            // 
            this.textTime.Location = new System.Drawing.Point(65, 182);
            this.textTime.Name = "textTime";
            this.textTime.Size = new System.Drawing.Size(223, 21);
            this.textTime.TabIndex = 2;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(116, 71);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(123, 24);
            this.button1.TabIndex = 3;
            this.button1.Text = "서버에 연결";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "서버 주소";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 135);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "현재 날짜";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 182);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "현재 시각";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 225);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textTime);
            this.Controls.Add(this.textDate);
            this.Controls.Add(this.textIP);
            this.Name = "Form1";
            this.Text = "타임 클라이언트";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textIP;
        private System.Windows.Forms.TextBox textDate;
        private System.Windows.Forms.TextBox textTime;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
    }
}

