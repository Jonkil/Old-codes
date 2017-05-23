#include <exception>
#include <string.h>
#include "zagolovok.h"
#include <stdio.h>
#include<iostream>
using namespace std;
int main(){
	try{
int  c;
int kol,probel,j,size;
int bukvi[256]={0};

myNull ex1;
myLetter ex2;
size=0;
probel=0;
kol=1;
   while((c=getchar())!='$'){
	if (c==' ') {probel++;}
	if (((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z'))) {++kol;}
   for(j=0;j<256;j++){
		if (c==j){bukvi[j]++;}
		}
   size++;
   }
for(j=0;j<256;j++){ 
if(bukvi[j]!=0){cout<<(char)j<<"= "<<bukvi[j]<<'\n';}
}  
if(probel==size){throw ex1;return 0;}
if(kol==1){throw ex2;return 0;}
}
catch(myLetter &e){cout<<e.what()<<'\n';}
catch(myNull &e){cout<<e.what()<<'\n';}
return 0;
}