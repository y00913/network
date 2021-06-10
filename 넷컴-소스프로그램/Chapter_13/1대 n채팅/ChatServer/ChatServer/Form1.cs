using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace ChatServer
{
    public partial class Form1 : Form
    {
        Server chat;
        public Form1()
        {
            InitializeComponent();
            text_IP.Text = Get_MyIP().ToString();
        }

   

        private void button_Start_Click(object sender, EventArgs e)
        {
            if (button_Start.Text == "채팅서버 시작")
            {
                chat = new Server(Convert.ToInt16(text_Port.Text));//Server 클래스 개체 생성
                chat.Start();  // 채팅 서버 시작

                button_Start.Text = "채팅서버 종료";
                MessageBox.Show("서버가 시작됨!");
            }
            else
            {
                chat.Stop();  // 채팅 서버 종료
                button_Start.Text = "채팅서버 시작"; //버튼 텍스트 바꾸기
                MessageBox.Show("서버를 종료하였습니다.");
            }
        }

        public string Get_MyIP()
        {
            IPHostEntry host = Dns.Resolve(Dns.GetHostName());
            string myip = host.AddressList[0].ToString();
            return myip;
        }
    }
}