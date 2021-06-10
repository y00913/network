using System;
using System.Collections.Generic;
using System.Text;

using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO; 

namespace ChatClient
{
    // NetWork 클래스
    public class Network
    {
        Form1 wnd = null;
        Thread th = null;

        private TcpClient client = null;
        private NetworkStream stream = null;
        private StreamReader reader = null;
        private StreamWriter writer = null;

        // 생성자 채팅 창
        public Network(Form1 wnd)
        {
            this.wnd = wnd;
        }

        // 채팅 서버와 연결 시도 , string 연결할 서버 아이피 , return 연결 유무
        public bool Connect(string ip)
        {
	        try	{
		        client = new TcpClient(ip, 7007);

		        wnd.Add_MSG(ip+ "서버에 접속 성공...");
		        stream = client.GetStream();
		        reader = new StreamReader(stream);
		        writer = new StreamWriter(stream);

		        th = new Thread(new ThreadStart(Receive));
		        th.Start(); 

		        return true;
	        }
	        catch(Exception ex){
		        wnd.Add_MSG(ex.Message);
                return false;
	        }
	    }
        // 채팅 서버와 연결 끊음
        public void DisConnect()
        {
            try
            {
                if (client != null)
                {
                    if (reader != null)
                        reader.Close();
                    if (writer != null)
                        writer.Close();

                    if (stream != null)
                        stream.Close();

                    client.Close();  // 클라이언트 접속 끊음
                    if(th.IsAlive)
                        th.Abort();      // 스레드 종료
                }
                wnd.Add_MSG("접속 종료");
            }
            catch (Exception ex)
            {
                wnd.Add_MSG(ex.Message);
            }
        }

        // 접속된 상대방 데이터 수신
        public void Receive(){
            string msg = null;
		    try{
		        do{
			            msg = reader.ReadLine();
			            wnd.Add_MSG(msg);
                }while( msg != null);
		    }
		    catch(Exception ex){
		         wnd.Add_MSG(ex.Message);
		    }
	     }

        // 접속한 상대방에 데이터 전송
        public void Send(string msg)
        {
            try
            {
                writer.WriteLine(msg);
                writer.Flush();
            }
            catch (Exception ex)
            {
                wnd.Add_MSG(ex.Message);
            }
        }

        public string Get_MyIP()
        {
            IPHostEntry host = Dns.Resolve(Dns.GetHostName());
            string myip = host.AddressList[0].ToString();
            return myip;
        }
    }
}
