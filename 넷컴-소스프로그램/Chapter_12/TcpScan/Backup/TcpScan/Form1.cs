using System;
using System.Threading;
using System.Windows.Forms;

namespace TcpScan
{
    public partial class Form1 : Form
    {
        private static int[] ports = { 1, 2, 3, 7777, 65380 };
        private Thread [] th;

        delegate void SetTextCallback(string msg);

        public Form1()
        {
            InitializeComponent();
        }

        public void AddString(string str)
        {
            try
            {
                if (this.txt_info.InvokeRequired) // 컨트롤 요청이 들어올 경우
                {   // 델리게이트를 이용해 Add_MSG 메서드를 다시 호출
                    SetTextCallback d = new SetTextCallback(AddString);
                    this.Invoke(d, new object[] { str });
                }
                else
                {   // 컨트롤 접근이 가능할 경우, 다음 구문 처리
                    this.txt_info.AppendText("\r\n" + str);
                 }
            }
            catch { } // 멀티 스레드 환경에서 뜻하지 않은 예외가 발생할 경우 처리
        }

        private void btn_start_Click(object sender, EventArgs e)
        {
            th = new Thread[ports.Length];

            for (int i = 0; i < ports.Length; i++)
            {
                Scan scan = new Scan(this, ports[i]);
                th[i] = new Thread(new ThreadStart(scan.ScanPort));
                th[i].Name = i.ToString();
                th[i].IsBackground = true;
                th[i].Start();
            }
            btn_start.Enabled = false;
        }

        private void btn_exit_Click(object sender, EventArgs e)
        {
            if (!btn_start.Enabled)  // 포트 스캔이 실행중이라면
            {
                for (int i = 0; i < ports.Length; i++)
                {
                    try
                    {   
                        if (th[i].IsAlive) th[i].Abort();
                    }
                    catch { }
                }
            }
            this.Close();
        }
    }
}