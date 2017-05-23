#include <exception>
#include <string.h>
#include "zagolovok.h"
#include <stdio.h>
#include<iosream>
using namespace std;
int main(){
int  c;
int k=0,kol=0,i,j,size=0;
char *massive;
int massive1[256]={0};
myNull ex1;
myDigit ex2;
massive1=new int[256];
massive=new char[70]; 
while((c=getchar())!='$'){
	massive[k]=c;
	k++;
}
size=k;
if(k==0){throw ex1;
}
for(i=0;i<size;i++){
	for(j=0;j<256;j++){
		if (massive[i]==j){massive1[j]++;}
	}
}
for(j=0;j<256;j++){
	if(massive1[j]!=0){cout<<j<<"= "<<massive1[j]<<endln;
	}
	if(massive1[j]==' '){ kol++;}
}
if(kol==size){throw ex1;}
kol=0;
for(j='a';j<'Z';j++){
	if(massive1[j]!=0){kol++;}
}
if(kol==0){throw ex2;}
}

