using System;
using System.Collections.Generic;
using System.Text;

using System.Collections;
using System.IO;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;

namespace ChatServer
{
    delegate void BroadCast(object sender);

    class Server
    {
        TcpListener listener;  // 접속 전용 소켓
        ClientGroup member; // 접속한 클라이언트 객체들을 저장
        Thread th;

        bool stop;  // 정지 조건 flag
        int port;

        // Server 생성자
        public Server(int port)
        {
            this.port = port; // 포트 번호           
        }

        // 클라이언트 요구를 처리
        public void Accept()
        {
            try
            {
                listener = new TcpListener(IPAddress.Any, port);
                listener.Start();  // 클라이언트 접속 받는 것을 시작
            }
            catch
            {
                return;
            }

            while (true)
            {
                // 클라이언트  접속을 대기 중
                Socket socket = listener.AcceptSocket();

                if (socket.Connected)
                { // 클라이언트가 접속하면...
                    // Client 개체를 생성하고, ClientGroup에 추가
                    member.Add(new Client(socket));
                    //IPEndPoint ip = (IPEndPoint)socket.RemoteEndPoint;
                    //BroadCast_MSG(ip.Address + " 가 서버에 참석");
                }
            }
        }

        // 채팅 서버 시작  
        public void Start()
        {
            member = new ClientGroup(); // ClientGroup 개체 생성
            //클라이언트 접속 받는 메서드를 스레드로 생성
            th = new Thread(new ThreadStart(Accept));
            th.Start();

            stop = true;
        }

        //  채팅 서버 중지
        public void Stop()
        {
            if (stop)
            { // stop 플래그를 검사해서
                listener.Stop();  // Listener 중지
                th.Abort();   // Accept 스레드 중지
                member.Dispose(); // Client 객체들 제거
                stop = false; // stop 플래그를 false로
            }
        }

        //BroadCast_MSG 메시지
        public void BroadCast_MSG(string msg)
        {
            try
            {
                member.Send_All(msg); //전체 방송 메시지 보내기				
            }
            catch (Exception ex)
            {
                member.Send_All(ex);  //예외 메시지를 방송
            }
        }
    }

    // 클라이언트 메시지 저장 큐
    class Message
    {
        Queue Message_queue = new Queue();

        // 메시지를 큐에 추가하는 메서드
        public void Enqueue(string Message)
        {
            // 동시에 두 개의 쓰레드가 사용할 수 없도록 동기화를 사용
            lock (Message_queue)
            {
                Message_queue.Enqueue(Message);
            }
        }

        // 메시지를 가져옴
        public string Dequeue()
        {
            // 동시에 두 개의 쓰레드가 사용할 수 없도록 락을 사용함
            lock (Message_queue)
            {
                if (Message_queue.Count != 0)
                    return Message_queue.Dequeue().ToString();
                return null; //메시지가 없음
            }
        }
    }

    // 서버가 연결된 클라이언트에게 메시지를 전달하기 위한 정보를 갖는 객체
    class Client
    {
        //public int Client_Num; 	// 클라이언트 식별번호
        public Message msg_queue; // 메시지 큐

        StreamReader read;    	// 입력 스트림
        StreamWriter write;   	// 출력 스트림
        NetworkStream stream;  	// 스트림
        Socket socket;  		// 소켓

        Thread th;   		// 읽어오기 스레드
        bool bConnect = false;  	// 연결상태 체크 프래그
        public event BroadCast Send_Event;  //이벤트 Send_Event을 BroadCast 델리게이트로 선언

        /*public override string ToString()
        {  //ToString 메서드 오버라이드
            return String.Format("Client[{0}] : ", Client_Num);
        }*/

        public Client(Socket socket)
        { //생성자
            this.socket = socket;
            if (this.socket.Connected)
            {// 연결된다면
                stream = new NetworkStream(socket);
                read = new StreamReader(stream);  // 스트림 읽기
                write = new StreamWriter(stream); //  스트림 보내기

                // 메시지 수신 메서드를 스레드로 생성
                th = new Thread(new ThreadStart(Receive));
                th.Start();

                bConnect = true; //연결
            }
        }

        public void Close()
        {
            if (bConnect)
            { // 리소스 해제
                th.Abort();
                read.Close();
                write.Close();
                stream.Close();
                socket.Close();

                bConnect = false;// 연결 해제
            }
        }

        // 클라이언트가 보낸 메시지를 수신하는 메서드
        public void Receive()
        {
            try
            {
                while (true)
                {
                    string message = read.ReadLine();

                    if (message != null)
                    {
                        msg_queue.Enqueue(message); // 메시지 큐에 저장
                        Send_Event(this); // 자신을 제외한 모든 클라이언트에게 전송
                    }
                }
            }
            catch
            {
                Close();
            }
        }

        // 메시지 송신
        public void Send(string msg)
        {
            write.WriteLine(msg);
            write.Flush();
        }
    }

    // 서버에 연결된 클라이언트들을 저장한 클래스
    class ClientGroup
    {
        ArrayList member = new ArrayList(); // 클라이언트 리스트
        //int Client_Num = 0;
        Message msg_queue = new Message(); // 메시지 큐

        // 클라이언트를 리스트에 추가
        public void Add(Client client)
        {
            //client.Client_Num = Client_Num++;
            client.msg_queue = msg_queue;
            client.Send_Event += new BroadCast(Send_All);// 이벤트에 Send_All 메서드 등록

            member.Add(client);
        }

        //  클라이언트에게 메시지 보내기
        public void Send_All(object sender)
        {
            string msg = msg_queue.Dequeue();

            foreach (Client client in member)
            {
                if (sender != client)
                    client.Send(msg);
            }
        }

        // 현재 연결된  Client 객체들을 제거
        public void Dispose()
        {
            foreach (Client client in member)
            {
                client.Close();
            }
        }
    }
}
