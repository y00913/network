using System;
using System.Net;
using System.Net.Sockets;
namespace TcpScan
{
    /// <summary>
    /// ��¥ ��Ʈ�� ���� ��ŷ �õ� IP�� �����ϴ� Ŭ����
    /// </summary>
    public class Scan
    {
        TcpListener listener = null; // TCP ����
        Form1 frm = null;
        int port;

        public Scan(Form1 frm, int port)
        {
            this.frm = frm;
            this.port = port;
        }

        // Ư�� ��Ʈ�� ������� ����
        public void ScanPort()
        {
            try
            {
                listener = new TcpListener(IPAddress.Any, port);
                listener.Start();  // TCP ������ �۵�
                frm.AddString(port + " ���� ��Ʈ ����!");
            }
            catch { frm.AddString(port + " ���� ��Ʈ ���� ����!"); }

            while (true)
            {
                Socket socket = listener.AcceptSocket();

                if (socket.Connected)
                { // Ŭ���̾�Ʈ�� �����ϸ�...
                    frm.AddString(port + "�� " + socket.RemoteEndPoint.ToString() + "���� �õ�!");
                }
            }
        }
    }
}
