using System;
using System.Net;           // IPEndPoint Ŭ����
using System.Net.Sockets;   // Socket Ŭ����

namespace TcpPort
{
    /// <summary>
    /// Ȱ��ȭ�� ��Ʈ��ȣ �˾Ƴ��� Ŭ����
    /// </summary>
    public class Port
    {
        public Socket connect = null;  // ����
        String server_ip = null;
        Form1 frm = null;
        int start, end;

        /// <summary>
        ///  Port Ŭ���� ������
        /// </summary>
        /// <param name="server_ip">�˻��� ���� IP</param>
        /// <param name="frm">ȣ���� ��</param>
        /// <param name="start">���� ��Ʈ ��ȣ</param>
        /// <param name="end">�� ��Ʈ ��ȣ</param>
        public Port(String server_ip, Form1 frm, int start, int end)
        {
            this.server_ip = server_ip;
            this.frm = frm;
            this.start = start;  // �˻��� ���� ��Ʈ ��ȣ
            this.end = end;       // �˻��� �� ��Ʈ ��ȣ
        }

        /// <summary>
        /// ��Ʈ �˻� ����
        /// </summary>
        /// <param name="server_ip">������ ���� ������</param>
        /// <param name="message">������ �޽���</param>
        public void Connect()
        {
            try
            {
                connect = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                frm.AddString_Info(start + "~" + end + ":��Ʈ �˻� ���� !");
                for (int port = this.start; port < this.end; port++)
                {
                    IPEndPoint E_IP = new IPEndPoint(IPAddress.Parse(this.server_ip), port);
                    try
                    {
                        connect.Connect(E_IP);  // ���� �õ�				
                        frm.AddString(port + " ���� ����!");
                    }
                    catch { /* ������ �����ϸ�*/ }
                }
                frm.AddString_Info(start + "~" + end + ":��Ʈ �˻� �Ϸ� !");
            }
            catch
            {
                frm.AddString_Info(start + "~" + end + ":��Ʈ �˻� ���� �߻� !");                
            }
            finally
            {
                if (connect != null) connect.Close();
            }           
        }
    }
}