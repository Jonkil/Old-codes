#include <iostream>
#include "sock_class.h"

int main(int argc,char**argv){

if (argc<2) cout<<">Not enough parametrs please use hostname as first parametr\n>and port as the second one"<<endl;
msocket sock;
int qfd;
//cout<<"stage1"<<endl;
if ((qfd=sock.connects(argv[1],argv[2]))==-1) return 1; 
cout<<">Chat client:"<<endl;
cout<<">To list all users use command '#U'"<<endl;
cout<<">To change your nickname at any time type #Nnickname"<<endl;
cout<<">To message to a specific person use pattern nickname:message"<<endl;
cout<<">To mass message just type in the message without specifying nickname"<<endl;
cout<<"--------------------------------------------------"<<endl;
char snick[21];
while(1){
cout<<">Please enter your nickname"<<endl;
for (int i=0;i<22;i++) snick[i]='\0';
cin>>snick;
if (snick[21]!='\0') {cout<<"Name too long";} else break;
}
sock.send_recieve(qfd,snick);
	
	
	
return 0;	
	
}




