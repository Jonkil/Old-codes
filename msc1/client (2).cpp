#include <iostream> 
#include "mysocket.h"
int main (int argc, char** argv)
{ 
 int size, fd;
 char buf[80], nick[20];
 cout << "Enter your name: " << endl;
 cin.getline (nick, 20);
  fd_set f;
 if (argc<2)  { cerr << "Error: Few arguments" <<endl; exit(1); }
 ClientSocket C(argv[1], atoi(argv[2]));
 C.Connect();
 fd=C.GetDescr();
  write (fd, nick, strlen(nick)+1);
  read(fd, nick, strlen(nick)+1);
  if (nick[0]=='\0')  { cerr << "Nickname is already in use "<<endl; exit(1); }
 while (1)
 {
	FD_ZERO(&f);
	FD_SET(fd, &f);
	FD_SET(0, &f);
	if (select(fd+1, &f, NULL, NULL, NULL)<=0) { perror("select:"); exit(1); }
	if (FD_ISSET(0, &f)) { size=read(0, buf, 80); if (!size) break; write(fd, buf, size);}
	if (FD_ISSET(fd, &f)) { size=read(fd, buf, 80); if (!size) break; write(1, buf, size); }
 }

 C.Shutdown();
 C.Close();
return 0;
}
