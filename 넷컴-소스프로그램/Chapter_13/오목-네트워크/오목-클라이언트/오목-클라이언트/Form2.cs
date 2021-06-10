using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;

namespace 오목_클라이언트
{
    public partial class Form2 : Form
    {
        Form1 f1;
        public Form2()
        {
            InitializeComponent();
        }
        public Form2(Form1 frm)
        {
            InitializeComponent();
            f1 = frm;

        }

        private void button1_Click(object sender, EventArgs e)
        {
            f1.client = new TcpClient(textBox1.Text, int.Parse(textBox2.Text));
            MessageBox.Show("연결");
            Close();
        }
    }
}
