#include "sock_class.h"
#include <iostream>
#include <errno.h>


using namespace std;

msocket::msocket(){
	if ((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
		{
		perror("socket");
		throw 1;
		}
	int x=1;
	setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&x,sizeof(x));
	it=sfd.begin();
	}
	
int msocket::binds(const char* ports){
	int res;
	struct sockaddr_in inf;
	inf.sin_family=AF_INET;
	inf.sin_port=htons(atoi(ports));
	inf.sin_addr.s_addr=INADDR_ANY;
	if (res=bind(fd,(struct sockaddr *)&inf,sizeof(inf))) {
		perror("bind");
		throw 1;
		}
	return res;
	}
	
void msocket::listens(){
	list<int> todel;
	
	char buf[256];
	char*msg;
	int h=9;
	int del;
	int fds;
	int bs;
	socklen_t alen;
	struct sockaddr_in a;
	listen(fd,5);
	sfd.push_back(fd);
	while (1){
	//cout<<"hello world"<<endl;
	FD_ZERO(&fset);
	
	for(it=sfd.begin();it!=sfd.end();it++){
	FD_SET(*it,&fset);
	//cout<<"adding "<<*it<<endl;
	}
	
	
	select(FD_SETSIZE,&fset,NULL,NULL,NULL);
	//cout<<"selecting"<<endl;
	
	
		for(it=sfd.begin();it!=sfd.end();it++){
			if (FD_ISSET(*it,&fset)&&*it!=fd) {
				////////buf=new char[256];
				bs=read(*it,&buf,sizeof(buf));
				write(1,buf,bs);
				write(1,"|",1);
				//cout<<buf<<"_-------------"<<endl;
				 //reading from the stream;
				if (bs==0){ //if user has disconnected
					cout<<"begining>bs="<<bs<<endl;
					todel.push_back(*it);
					//telling everyone that user has left
					
					for(mapit=mapcon.begin();mapit!=mapcon.end();mapit++){
					if (mapit->first!=*it){
					write(mapit->first,"The user has left:",strlen("The user has left:"));
					write(mapit->first,mapcon[*it],strlen(mapcon[*it]));
					write(mapit->first,"\n",1);}
					}
					mapcon.erase(*it);
					
				}else{
					if (buf[0]=='#'){
					
						if (buf[1]=='N'){
							//write(*it,"-hello-\n",8);
							if (bs>20) {write(*it,">Nickname is too long try again!",strlen(">Nickname is too long try again!"));continue;}
							msg=new char[bs-4];
							int l;
							for(l=2;l<bs;l++) msg[l-2]=buf[l];
							msg[l-2]='\0';
							cout<<"N>msg="<<msg<<endl;
							cout<<"N>len="<<strlen(msg)<<endl;
							//checking for existing name;
							for(mapit=mapcon.begin();mapit!=mapcon.end();mapit++)
							{ 
							if ((h=strcmp(mapit->second,msg))==0) break; 
							}
							//adding name
							if(h==0){
							
							write(*it,">Username exists. Setting your name to unknown",strlen(">Username exists. Setting your name to unknown"));
							char tmp[10];
							sprintf(tmp,"%d",*it);
							write(*it,tmp,strlen(tmp));
							write(*it,"\n>Please change your name\n",strlen("\n>Please change your name\n"));
							delete msg;
							msg=new char[strlen("unknown")+strlen(tmp)];
							sprintf(msg,"unknown%d",*it);
									}
							delete[] mapcon[*it];
							mapcon[*it]=new char[strlen(msg)+1];
							strcpy(mapcon[*it],msg);
							write(*it,">Nickname registered: ",strlen(">Nickname registered: "));
							write(*it,msg,strlen(msg));
							write(*it,"\n",1);
							delete msg;
										}
						else if(buf[1]=='U'){
							write(*it,"Online users: ",strlen("Online users: "));
							for (mapit=mapcon.begin();mapit!=mapcon.end();mapit++) {
								write(*it,mapit->second,strlen(mapit->second));
								write(*it," ",1);									}
								write(*it,"\n",1);					
											}
						else if(buf[1]=='M'){
							char to[20]; //maximum nick length
							int z=0;
							int nick;
							for(z=0;z<20;z++){ 
							to[z]=buf[z+2];
							if (nick=(buf[z]==':')) {to[z-2]='\0'; break;}
											}				
							
							if (nick==0) {
								for(mapit=mapcon.begin();mapit!=mapcon.end();mapit++){
								write(mapit->first,"<MassMessage>",strlen("<MassMessage>"));
								write(mapit->first,mapcon[*it],strlen(mapcon[*it]));
								write(mapit->first,":",strlen(":"));
								
								
								char*msg;
								msg=new char [bs-2];
								int u;
								for (u=0;u<(bs-1);u++)
								msg[u]=buf[u+2];
								msg[u]='\0';
									
								write(mapit->first,msg,strlen(msg));
								cout<<"stage1\n"<<endl;
								delete msg;
								
							}
								continue;
						}
							char*msg;
							msg=new char [bs-2-z];
							int u;
							for (u=0;u<(bs-1-z);u++)
								msg[u]=buf[z+u+1];
							msg[u]='\0';	
							cout<<"to:"<<to<<strlen(to)<<endl;
							cout<<"msg:"<<msg<<strlen(msg)<<endl;	
							//searching for destanation point
							int cmp=23;
							for (mapit=mapcon.begin();mapit!=mapcon.end();mapit++)
								if ((cmp=strcmp(mapit->second,to))==0) break;
							if (cmp==0){
								write(mapit->first,mapcon[*it],strlen(mapcon[*it]));
								write(mapit->first,":",1);
								write(mapit->first,msg,strlen(msg));
								write(mapit->first,"\n",1);	
																											
										}else{
								write(*it,"User does not exist\n",strlen("User does not exist\n"));			
											}			
					delete msg;
											}else {write(*it,">No such command\n",strlen(">No such command\n"));}
									}
					for (int i=0;i<256;i++) buf[i]='\0';
					}
			
			
			/////delete buf;
			}
		//cout<<"end for"<<endl;
		}






if (FD_ISSET(fd,&fset)) {
		
		
		alen=sizeof(a);
		if ((fds=accept(fd, (struct sockaddr*)&a, &alen))==-1){
		perror("accept");			
		}
		sfd.push_back(fds);
		cout<<fds<<" added"<<endl;
		}





for (int g=0;g<255;g++) buf[g]='\n';
//if (del!=0) {sfd.remove(del);del=0;}
while (!todel.empty()) {sfd.remove(todel.front());todel.pop_front();}
cout<<"end while"<<endl;
}
	
}

int msocket::connects(char* hostname,char* ports){
	int z,u;
	struct hostent *h;
	struct sockaddr_in t;
	
	t.sin_family=AF_INET;
	t.sin_port=htons(atoi(ports));
	
	if ((h=gethostbyname(hostname))==NULL){cout<<"error getting hostname";return -1;}
	memcpy(&t.sin_addr,h->h_addr_list[0],sizeof(t.sin_addr));
	u=connect(fd,(struct sockaddr *) &t,sizeof(t));
	if (u==0) return fd;
	else {
		cout<<strerror(errno)<<endl;
		return -1;}
	
	}

void msocket::send_recieve(int fda,char*snickname){
char buf[256];
char*msg;
int bs;
char* nm=new char[strlen(snickname)+2];
for(int za=0;za<strlen(snickname);za++)
nm[za+2]=snickname[za];
nm[0]='#';
nm[1]='N';
write(fda,nm,2+strlen(snickname));
		while (1){
FD_ZERO(&fset);

	FD_SET(fda,&fset);
	FD_SET(0,&fset);
	
	select(FD_SETSIZE,&fset,NULL,NULL,NULL);
	
	
	if (FD_ISSET(fda,&fset)) {
		
		bs=read(fda,&buf,sizeof(buf));
		if (bs==0) {write(1,"Connection was terminated\n",strlen("Connection was terminated\n"));break;}
		//cout<<buf<<endl;
		write(1,&buf,bs);
		//cout<<"bs[server]="<<bs<<endl;
		
}
	
	if (FD_ISSET(0,&fset)){
		
		bs=read(0,&buf,sizeof(buf));
		buf[bs-1]='\0';
		if (buf[0]!='#'){
			msg=new char[bs+2];
			for (int i=0;i<bs;i++)		
			msg[i+2]=buf[i];
			msg[0]='#';
			msg[1]='M';
			write(fda,msg,strlen(msg));
			continue;
		}
		write(fda,&buf,bs);
		//cout<<"bs[client]="<<bs<<endl;
		
	}
	
	
	
	


}
}
