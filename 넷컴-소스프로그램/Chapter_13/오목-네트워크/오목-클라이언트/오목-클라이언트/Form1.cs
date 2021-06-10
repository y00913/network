using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace 오목_클라이언트
{
    public partial class Form1 : Form
    {
        enum SITE { none, black, white };

        SITE[,] m_Board = new SITE[20, 20];
        bool m_bTurn;

        private Pen pen;
        private Brush whitebrush, blackbrush;
        int ax, ay;
        StreamWriter writer;
        StreamReader reader;
        Thread t;
        Form2 dlg;
        public TcpClient client; // 연결 소켓

        public Form1()
        {
            InitializeComponent();
            m_bTurn = false;

            pen = new Pen(Color.Black);
            whitebrush = new SolidBrush(Color.White);
            blackbrush = new SolidBrush(Color.Black);
        }


        private void 종료ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (reader != null) reader.Close();
            if (writer != null) writer.Close();
            if (t.IsAlive) t.Abort();
            Close();
        }

        private void Form1_MouseDown(object sender, MouseEventArgs e)
        {
            if (m_bTurn == true)//흑만 둠
            {
                if (e.Button != MouseButtons.Left)
                    return;

                ax = e.X / 20;
                ay = e.Y / 20;

                if ((ax < 0) || (ax >= 19) || (ay < 0) || (ay >= 19)) return;
                if (m_Board[ax, ay] != SITE.none) return;
                m_Board[ax, ay] = SITE.black;
                DrawSite(ax, ay, m_Board[ax, ay]);

                writer.WriteLine(ax);
                writer.WriteLine(ay);
                writer.Flush();
                //dist();
                m_bTurn = false;
            }
        }

        public void Receive()
        {
            while (true)
            {
                try
                {
                    ax = int.Parse(reader.ReadLine());
                    ay = int.Parse(reader.ReadLine());
                    m_Board[ax, ay] = SITE.white;
                    DrawSite(ax, ay, m_Board[ax, ay]);

                    m_bTurn = true;
                }
                catch (Exception ex)
                {

                }
            }
        }

        private void 연결ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            dlg = new Form2(this);
            dlg.ShowDialog();

            NetworkStream ns = client.GetStream();
            m_bTurn = true;
            writer = new StreamWriter(ns);
            reader = new StreamReader(ns);
            t = new Thread(new ThreadStart(Receive));
            t.Start();

        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics g = this.CreateGraphics();

            for (int x = 0; x < 19; x++)   // 가로줄 그음
            {
                g.DrawLine(pen, 10, 34 + x * 20, 370, 34 + x * 20);
            }
            for (int x = 0; x < 19; x++)   // 세로줄 그음
            {
                g.DrawLine(pen, 10 + x * 20, 34, 10 + x * 20, 394);
            }

            for (int x = 0; x < 19; x++)   // 바둑돌 그림
                for (int y = 0; y < 19; y++)
                {
                    DrawSite(x, y, m_Board[x, y]);
                }
        }
        void DrawSite(int x, int y, SITE dol)
        {
            Graphics g = this.CreateGraphics();
            if (dol != SITE.none)
            {
                if (dol == SITE.black)
                {
                    g.FillEllipse(blackbrush, x * 20 + 2, y * 20 + 2, 16, 16);
                }
                else
                    g.FillEllipse(whitebrush, x * 20 + 2, y * 20 + 2, 16, 16);
            }

        }

    }
}
