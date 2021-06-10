using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace TimeServer
{
    public partial class Form1 : Form
    {
	    public NetworkStream stream;
	    public const int port = 7000;// 서버 포트 번호
        TcpListener listener = null;
	    StreamWriter strWriter = null;
	    Thread th = null;

        public Form1()
        {
            InitializeComponent();
        }

        public void Accept()
        {
          //IPEndPoint ipep = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 7000);
            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 7000);
            //listener = new TcpListener(port);
            listener = new TcpListener(ipep);
            listener.Start(); //
            button1.Text = "타임 서버 종료";
            while (true)
            {
                Socket client = listener.AcceptSocket(); // 접속한 클라이언트와 통신할 소켓 생성

                if (client.Connected) // 클라이언트가 접속했을 경우
                {
                    stream = new NetworkStream(client);
                    strWriter = new StreamWriter(stream);
                    strWriter.WriteLine(DateTime.Now);// 현재 시각을 전송
                    strWriter.Flush();
                    stream.Close();
                    strWriter.Close();
                }
                client.Close();
            }
        }

        public void Stop()
        {
            listener.Stop();
            button1.Text = "타임 서버 시작";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (button1.Text == "타임 서버 시작")
            {
                th = new Thread(new ThreadStart(Accept));
                th.Start(); //
            }
            else
            {
                this.Stop();
                th.Abort();
            }
        }
    }
}