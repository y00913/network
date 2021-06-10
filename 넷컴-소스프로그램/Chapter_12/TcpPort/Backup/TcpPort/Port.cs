using System;
using System.Net;           // IPEndPoint 클래스
using System.Net.Sockets;   // Socket 클래스

namespace TcpPort
{
    /// <summary>
    /// 활성화된 포트번호 알아내는 클래스
    /// </summary>
    public class Port
    {
        public Socket connect = null;  // 소켓
        String server_ip = null;
        Form1 frm = null;
        int start, end;

        /// <summary>
        ///  Port 클래스 생성자
        /// </summary>
        /// <param name="server_ip">검색할 서버 IP</param>
        /// <param name="frm">호출한 폼</param>
        /// <param name="start">시작 포트 번호</param>
        /// <param name="end">끝 포트 번호</param>
        public Port(String server_ip, Form1 frm, int start, int end)
        {
            this.server_ip = server_ip;
            this.frm = frm;
            this.start = start;  // 검색할 시작 포트 번호
            this.end = end;       // 검색할 끝 포트 번호
        }

        /// <summary>
        /// 포트 검사 수행
        /// </summary>
        /// <param name="server_ip">연결할 서버 아이피</param>
        /// <param name="message">전송할 메시지</param>
        public void Connect()
        {
            try
            {
                connect = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                frm.AddString_Info(start + "~" + end + ":포트 검사 시작 !");
                for (int port = this.start; port < this.end; port++)
                {
                    IPEndPoint E_IP = new IPEndPoint(IPAddress.Parse(this.server_ip), port);
                    try
                    {
                        connect.Connect(E_IP);  // 연결 시도				
                        frm.AddString(port + " 열려 있음!");
                    }
                    catch { /* 연결이 실패하면*/ }
                }
                frm.AddString_Info(start + "~" + end + ":포트 검사 완료 !");
            }
            catch
            {
                frm.AddString_Info(start + "~" + end + ":포트 검사 문제 발생 !");                
            }
            finally
            {
                if (connect != null) connect.Close();
            }           
        }
    }
}