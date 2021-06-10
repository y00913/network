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

namespace TimeClient
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            TcpClient client = new TcpClient(textIP.Text.Trim(), 7000); // 서버에 연결
            NetworkStream stream = client.GetStream(); // NetworkStream 객체 생상
            StreamReader strReader = new StreamReader(stream);
            DateTime data = DateTime.Parse(strReader.ReadLine()); // 서버로부터 데이터 수신
            textTime.Text = data.ToLongTimeString().ToString(); // 시간 출력
            textDate.Text = data.ToLongDateString().ToString(); // 날자 출력
            
            // close
            client.Close();
            stream.Close();
            strReader.Close();
        }
    }
}