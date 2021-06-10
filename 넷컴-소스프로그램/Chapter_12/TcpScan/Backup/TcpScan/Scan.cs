using System;
using System.Net;
using System.Net.Sockets;
namespace TcpScan
{
    /// <summary>
    /// 가짜 포트를 열어 해킹 시도 IP를 추적하는 클래스
    /// </summary>
    public class Scan
    {
        TcpListener listener = null; // TCP 서버
        Form1 frm = null;
        int port;

        public Scan(Form1 frm, int port)
        {
            this.frm = frm;
            this.port = port;
        }

        // 특정 포트를 열어놓고 감시
        public void ScanPort()
        {
            try
            {
                listener = new TcpListener(IPAddress.Any, port);
                listener.Start();  // TCP 리스너 작동
                frm.AddString(port + " 서버 포트 실행!");
            }
            catch { frm.AddString(port + " 서버 포트 열기 실패!"); }

            while (true)
            {
                Socket socket = listener.AcceptSocket();

                if (socket.Connected)
                { // 클라이언트가 접속하면...
                    frm.AddString(port + "에 " + socket.RemoteEndPoint.ToString() + "접속 시도!");
                }
            }
        }
    }
}
