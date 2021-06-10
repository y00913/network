using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace 채팅서버
{
    public partial class Form1 : Form
    {
        private Network net = null; //채팅을 처리하는 Network 클래스 변수 선언
        private Thread server_th = null; //채팅 서버 스레드 선언

        public Form1()
        {
            InitializeComponent();
            net = new Network(this); // Network 클래스 객체 생성	
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            server_th = new Thread(new ThreadStart(net.ServerStart));//net는 Network 클래스 객체
            server_th.Start(); // 채팅 서버 시작
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
  
                    net.ServerStop(); //채팅 서버 실행 중지

                    if (server_th.IsAlive) //ServerStart 스레드를 종료
                        server_th.Abort();
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message); //예외 메시지 출력
            }			
        }
        public void Add_MSG(string msg)
        {
            txt_info.AppendText(msg + "\r\n"); // 채팅 문자열 출력
            txt_info.ScrollToCaret(); // txt_info 텍스트 박스 자동 스크롤	
            txt_input.Focus(); //txt_input 텍스트 박스에 초점 맞춤
        }

        private void txt_input_KeyDown(object sender, KeyEventArgs e)
        {
            // 엔터키가 눌리면 문자열 메시지가 상대방에게 전송
            if (e.KeyCode == Keys.Enter)
            {
                string msg = txt_input.Text.Trim();
                //Add_MSG("[" + my_ip + "] " + msg);
                Add_MSG("[ 본인] " + msg);
                net.Send(msg);
                txt_input.Text = "";
                txt_input.Focus();
            }
        }

    }
}
