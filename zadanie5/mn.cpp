#include <exception>
#include <string.h>
#include "zagolovok.h"
#include <stdio.h>
#include<iostream>
using namespace std;
void main(){
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
 
		bukvi[c]++;
		
   size++;
   }
for(j=0;j<256;j++){ 
if(bukvi[j]!=0){cout<<static_cast<char>(j)<<"= "<<bukvi[j]<<'\n';}
}  
if(probel==size){throw ex1;}
if(kol==1){throw ex2;}
}
catch(exception &e){cout<<e.what()<<'\n';}
}