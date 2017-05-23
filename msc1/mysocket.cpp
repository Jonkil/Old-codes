#include "mysocket.h"
//mysock
void MySocket::Shutdown(){
	shutdown(Socket_d, 2);
}
void MySocket::Close(){
	close(Socket_d);
}
int MySocket::GetDescr(){
	return Socket_d;
}
//server
ServSocket::ServSocket (int port)
{
   if ((Socket_d=socket(AF_INET, SOCK_STREAM, 0))==-1) { perror("socket"); exit(1); }
   addr.sin_family=AF_INET;
   addr.sin_port=htons(port);
   //addr.sin_addr=INADDR_ANY;
   //inet_aton("127.0.0.1", &addr.sin_addr);
   inet_aton("192.168.58.16", &addr.sin_addr);
 
}
void ServSocket::Bind(){
	if (bind(Socket_d, (struct sockaddr *) &addr, sizeof(addr))==-1) { perror("bind"); exit(1); }
}
void ServSocket::Listen(int QueueLen){
	if (listen(Socket_d, QueueLen)==-1) { perror("listen"); exit(1);}
}
int ServSocket::Accept(){	
	int fd;
	socklen_t len;
	//ServSocket temp(addr.sin_port);
	struct sockaddr_in client;
	len=sizeof(client);
	if ((fd=accept(Socket_d, (struct sockaddr *) &client, &len)) < 0) { perror("accept");  exit(1); }
	return fd;

}
//client
void ClientSocket::Connect (){
	if (-1==connect(Socket_d, (struct sockaddr *) &addr, sizeof(addr))) { perror("connect"); exit(1); }
}
ClientSocket::ClientSocket (char* hostname, int port){	
	struct hostent *h;
	if ((Socket_d=socket(AF_INET, SOCK_STREAM, 0))==-1) { perror("socket"); exit(1); }
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	//gethostname(buf, 80);
	//if ((h=gethostbyname(buf))==NULL) { cerr << "Chet ne tak!" << endl; exit(1); }
	//memcpy(&addr.sin_addr, h->h_addr_list[0], sizeof(addr.sin_addr));
	inet_aton(hostname, (struct in_addr *) &addr.sin_addr);
}
