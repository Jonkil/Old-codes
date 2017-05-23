#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "utils.h"

#define ADDRESS "mysocket"

int Count=1;
char ToSend[MAX_MSG_LENGTH];
int Curr=0;

void Init(void) {
/* Banner */
	printf("\n|\\    /|     AMC SQL Client\n");
	printf("|*\\/\\/*|     Copyright 1995 by AMC Soft\n");
	printf("|*/\\/\\*|     All Rights Not Reserved\n");
	printf("|/    \\|     E-mail : amc@cs.msu.su or amc@red.com.ru\n\n");
/* Channel initialization */
}

int ClientHandler(char *s) {
int i;
char *t;
int flag=FALSE;
	t=(char*)calloc(MAX_MSG_LENGTH,1);
	for(i=0;i<strlen(s);i++)
		if(s[i]<32 && s[i])
			s[i]=32;
	strcpy(t,s);
	s=Trim(s);
	Count++;
	s=ToUpper(s);
	if (!strcmp(s,"EXIT"))
		flag=TRUE;
	if (!strcmp(s,"QUIT"))
		flag=TRUE;
	if (!strcmp(s,"BYE"))
		flag=TRUE;
	strcpy(s,t);
	return flag;
}

void Run(void) {
char c;
FILE *file;
int i,s,len;
struct sockaddr_un	sockun;
char* Msg;
int ch,p=0;
int End=NO;
	Msg=(char*)calloc(MAX_MSG_LENGTH,1);
	if((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("Client: socket ceration error.\n");	
		exit(1);
		}
	sockun.sun_family = AF_UNIX;
	strcpy(sockun.sun_path, ADDRESS);
	len = sizeof(sockun.sun_family) + strlen(sockun.sun_path);
	if(connect(s, &sockun, len) <  0) {
		perror("Client: connection error.\n");
		exit(1);
		}
	file = fdopen(s, "r");
	while(!End) {
		printf("\n\r %d > ",Count);
		i=0;
		while(!((ch=getchar())=='\n' && p=='\n')) {
			p=ch;
			Msg[i++]=ch;
			}
		Msg[i++]='@'; 
		Msg[i]='\0';
		End=ClientHandler(Msg);
		if(!End) {
			send(s,Msg,strlen(Msg),0);
			while ((signed char)(Msg[i++]=fgetc(file))!=(signed char)-1) 
{
				if(Msg[i-1]!='@')
					putchar(Msg[i-1]);
				if (Msg[i-1]=='@') {
					Msg[i-1]=0;
					break;
					}
				}	
			}
	}
	free(Msg);
	close(s);
	printf(" Totally encountered %d transactions.\n",Count-1);
	printf(" Have a nice UNIX.\n");
}

void Done(void) {
/* Channel destruction */
}

main() {
	Init();
	Run();
	Done();
	return 0;
}
