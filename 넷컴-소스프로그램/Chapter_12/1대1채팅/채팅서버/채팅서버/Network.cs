using System;
using System.Collections.Generic;
using System.Text;
using System.Net;  //네트워크 처리
using System.Net.Sockets; // 소켓 처리
using System.Threading; // 스레드 처리


namespace 채팅서버
{
    class Network
    {
            
           Form1 wnd = null; // 채팅 창 인스턴스 변수
           Socket server = null; //채팅 서버로 사용할 소켓
           Socket client = null; //채팅 클라이언트로 사용할 소켓(접속용)
	       Thread th = null; //스레드 처리

	 public Network(Form1 wnd)	{			
	      this.wnd = wnd;			
	 }

           // 채팅 서버 시작
	 public void ServerStart(){
	      try{ //서버 포트 번호를 7000번으로 지정	
              //IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 7000);
              IPEndPoint ipep = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 7000);
               server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
	           server.Bind(ipep); //서버를 포트 번호를 바인드 합니다.
		        server.Listen(10); //클라이언트 접속을 대기합니다.
		        wnd.Add_MSG("채팅 서버 시작...");

	           client = server.Accept(); //클라이언트가 접속되면
                    
                      // 접속한 클라이언트 ip 주소를 출력합니다
	           IPEndPoint ip = (IPEndPoint)client.RemoteEndPoint;
	           wnd.Add_MSG(ip.Address + "접속...");
                       
                     //Receive 메서드를 스레드로 등록하고 실행합니다.
	           th = new Thread(new ThreadStart(Receive));
	           th.Start();
          }
	      catch(Exception ex){ //채팅 서버에서 예외가 발생하면 
                     wnd.Add_MSG(ex.Message); //예외 메시지 txt_info에 출력합니다.
	      }
	 }

           // 채팅 서버 중지
	 public void ServerStop(){
	      try{	
	           if(client != null) //클라이언트가 접속된 상태라면
	           {
		           if( client.Connected){	
		                client.Close();  // 클라이언트 접속 끊음
		                if(th.IsAlive) // Receive 메서드 스레드가 실행중이라면
			                th.Abort();  // 스레드 종료
		            }
		        }						
		        server.Close(); //채팅 소켓을 닫습니다.
	      }
	      catch(Exception ex){ //예외 메시지 출력
	           wnd.Add_MSG(ex.Message);
	      }			
	 }

    // 채팅 서버와 연결
    public bool Connect(string ip){
	      try{   // 접속할 채팅 서버 ip 주소와 포트 번호를 지정합니다.
	            IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(ip), 7000);
                client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		        client.Connect(ipep); //채팅서버에 접속을 시도합니다.
		        wnd.Add_MSG(ip+ "서버에 접속 성공...");
	            th = new Thread(new ThreadStart(Receive)); //채팅 문자열 수신하는 메서드를 스레드로
		        th.Start();  //생성하고 시작
		        return true; // 채팅 서버 접속하면 true 값을 반환
	      }
	      catch(Exception ex){
	           wnd.Add_MSG(ex.Message); //채팅서버 접속에 실패하면 예외 메시지를
		       return false; //출력하고 false 값을 반환합니다.
	      }
	 }
		
           // 채팅 서버와 연결 종료
	 public void DisConnect(){
	      try{
	           if(client != null){ // 채팅 서버와 연결 되어있다면
		            if( client.Connected ) 
		                client.Close(); // 채팅서버와의 연결을 끊습니다.
				
		            if(th.IsAlive)
		                th.Abort(); //Receive 메서드 스레드 중지합니다.
		        }
	            wnd.Add_MSG("채팅 서버 연결 종료!");
	      }
	      catch(Exception ex){ //채팅 서버 연결 해재와 스레드 종료시 예외가 발생하면
	           wnd.Add_MSG(ex.Message); // txt_info에 예외 메시지 출력
	      }
	 }

           // 상대방의 데이터 수신
     public void Receive(){
	      try{ //상대방과 연결되었다면
	           while( client != null && client.Connected ){
	                //ReceiveData 메서드를 사용해 바이트 단위로 데이터를 읽어옵니다.
	                byte [] data = ReceiveData();
                    wnd.Add_MSG ("[상대방]"+ Encoding.Default.GetString(data));
                     
	           }  
          }
	      catch(Exception ex){ 
	           wnd.Add_MSG(ex.Message);
	      }
    }

           // 상대방에게 데이터 송신
	 public void Send(string msg){
         try
         {
             if (client.Connected)
             { // 상대방과 연결되어 있으면
                 // 문자열을 바이트 배열 형태로 변경합니다.
                 byte[] data = Encoding.Default.GetBytes(msg);
                 SendData(data); //바이트 배열을 상대방에 전송			
             }
             else
             { //상대방과 연결되어 있지 않다면
                 wnd.Add_MSG("메시지 전송 실패!");
             }
         }
         catch (Exception ex)
         {
             wnd.Add_MSG(ex.Message);
         }
	 }
   
     private void SendData(byte[] data){
	      try{		 
	           int total = 0; //전송된 총 크기
	           int size = data.Length; //전송할 바이트 배열의 크기
	        	 int left_data = size ; //남은 데이터 량
		        int send_data = 0; // 전송된 데이터 크기

		        // 전송할 실제 데이터의 크기 전달
		        byte [] data_size =new byte[4]; //정수형태로 데이터 크기 전송
		        data_size = BitConverter.GetBytes(size);
		        send_data = client.Send(data_size);		

		        // 실제 데이터 전송
		        while(total < size){  
	                     send_data  = client.Send(data, total, left_data, SocketFlags.None);
			            total += send_data;
			            left_data -= send_data;
	            }
	       }
	       catch(Exception ex)	{//데이터 전송중 예외가 발생하면 에러메시지 출력
		        wnd.Add_MSG(ex.Message);
           }
	  }
	
      private byte[] ReceiveData(){
	       try{
	           int total = 0; //수신된 데이터 총량
	    	    int size = 0; //수신할 데이터 크기
		        int left_data = 0; //남은 데이터 크기
		        int recv_data = 0; //수신한 데이터 크기

		        // 수신할 데이터 크기 알아내기   
		         byte[] data_size = new byte[4];  
		        recv_data = client.Receive(data_size, 0, 4, SocketFlags.None);
		        size = BitConverter.ToInt32(data_size, 0);
		        left_data = size;

	    	    byte [] data = new byte[size]; //바이트 배열 생성
		         // 서버에서 전송한 실제 데이터 수신
		        while(total < size) { //상대방이 전송한 데이터를 읽어옴
	                  recv_data = client.Receive(data, total, left_data, SocketFlags.None);
                      if (recv_data == 0) break;
		              total += recv_data ;
		              left_data -= recv_data;
		        }
		        return data;
            }
	        catch(Exception ex)	{ //데이터수신중 예외가 발생하면 에러메시지 출력
		      wnd.Add_MSG(ex.Message);
		      return null;
	        }
	  }		
    }
}
