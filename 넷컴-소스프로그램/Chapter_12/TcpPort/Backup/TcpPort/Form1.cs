using System;
using System.Data;
using System.Windows.Forms;

using System.Threading;     // 스레드 사용

namespace TcpPort
{
    public partial class Form1 : Form
    {
        // 1~65635 포트를 순차적으로 검사하면 속도가 느리므로
        // 256개의 스레드를 발생시켜 검사 속도를 빠르게 만들기 위해서 선언
        Port[] port = new Port[256];
        Thread[] t = new Thread[256];

        public Form1()
        {
            InitializeComponent();
            Form.CheckForIllegalCrossThreadCalls = false; // 크로스 스레드 예외 off
        }

        /// <summary>
        ///  txt_out에 문자열 추가
        /// </summary>
        /// <param name="str">추가할 문자열</param>
        public void AddString(string str)
        {
            lock(txt_out)
            {
                    this.txt_out.AppendText("\r\n **" + str);
            }
        }

        /// <summary>
        /// txt_info에 문자열 추가
        /// </summary>
        /// <param name="str"></param>
        public void AddString_Info(string str)
        {
            lock (txt_info)
            {
                this.txt_info.AppendText("\r\n **" + str);
            }
        }


        private void btn_search_Click(object sender, EventArgs e)
        {
            // 검사중에 버튼을 클릭할때 예외 처리
            if (t[255] != null)
            {
                if (t[255].IsAlive)
                {
                    MessageBox.Show("현재 포트 번호 검사중입니다. 잠시만 기다리세요", "에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

            String server_ip = txt_ip.Text.Trim(); // 서버 아이피 가져옴

            for (int i = 0; i < 256; i++)
            {
                port[i] = new Port(server_ip, this, i * 256, (i * 256 + 255));
                t[i] = new Thread(new ThreadStart(port[i].Connect));
                t[i].Start();
            }
        }

        /// <summary>
      	/// 프로그램이 종료될때  발생하는 이벤트
        /// 현재 활성화된 스레드 종료
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            for (int i = 0; i < 256; i++)
            {
                try
                {
                    if (t[i].IsAlive)
                    {
                        t[i].Abort(); // 현재 활성화된 스레드 종료
                    }
                }
                catch { }
            }
        } 

    }
}