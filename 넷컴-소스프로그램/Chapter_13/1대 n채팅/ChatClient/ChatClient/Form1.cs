using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.Net;        // 네트워크
using System.Net.Sockets;  //TCP 소켓
using System.Threading;   // 쓰레드 사용
using System.Diagnostics; //네트워크에서 이벤트 처리

namespace ChatClient
{
    public partial class Form1 : Form
    {
        private Network net = null;	//Network 클래스 개체
        private string my_ip = null; //자신의 아이피를 저장해두는 개체

        public Form1()
        {
            InitializeComponent();
            this.net = new Network(this);
            //my_ip = net.Get_MyIP();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        private void button_Connect_Click(object sender, EventArgs e)
        {
            if (button_Connect.Text == "연결")
            {
                string ip = text_IP.Text.Trim();
                if (ip == "")
                {
                    MessageBox.Show("아이피 번호를 입력하세요!");
                    return;
                }

                if (!net.Connect(ip))
                {
                    MessageBox.Show("서버 아이피 번호가 틀리거나 서버가 작동중이지 않습니다.");
                }
                else
                {
                    button_Connect.Text = "접속중...";   // 접속이 성공하면
                    net.Send(text_Name.Text + " 님이 참여 하였습니다.");
                }
            }
            else
            {     // 서버에 종료 메시지 전송
                net.Send(text_Name.Text + "  접속 종료");
                net.DisConnect();
                button_Connect.Text = "연결";
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                if (button_Connect.Text == "접속중...")
                {
                    net.DisConnect();
                }
            }  
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void text_Input_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string msg = "[" + text_Name.Text + "]"+ text_Input.Text.Trim();
                Add_MSG(msg);// 자신의 채팅 창에 메시지 추가
                net.Send(msg); // 서버에 메시지 전달
                text_Input.Text = "";
                text_Input.Focus();
            }
        }

        public void Add_MSG(string msg)
        {
            text_Info.AppendText(msg + "\r\n");     //채팅 문자열 출력
            text_Info.ScrollToCaret();             //txt_info 텍스트 박스 자동 스크롤
            text_Input.Focus();                     //txt_input 텍스트박스에 초점 맞춤
        }
    }
}