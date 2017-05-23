#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
using namespace std;
class MySocket {
protected:
	int Socket_d;
	struct sockaddr_in addr;
public:
	MySocket() { }
	~MySocket() { }
	int GetDescr();
	//void Send (const char* str, int len);
	//char* Get (void);
	void Shutdown ();
	void Close ();
};
class ServSocket: public MySocket {
public:
	ServSocket (int port);
	~ServSocket() {}
	int Accept();
	void Bind();
	void Listen (int QueueLen);

};
class ClientSocket: public MySocket {
public: 
	ClientSocket (char* hostname, int port);
	~ClientSocket() { }
	void Connect();
};
