#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <list>
#include <map>
#include <netinet/in.h>

using namespace std;

class msocket{
	int port;
	char* host;
	
	int fd;
	fd_set fset;
	map<int,char*> mapcon;
	map<int,char*>::iterator mapit;
	list<int> sfd; //spisok deskriptorov
	list<int>::iterator it; //iterator
	
public:
msocket();
int binds(const char* ports="2008");
void listens();
int connects(char* hostname="localhost",char* ports="2008");	
void send_recieve(int fda,char*snickname);	
	
	
	
	
	
	
	
	
	};
	
	
	
	
