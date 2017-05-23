#include <iostream>
#include <ctype.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <list>
using namespace std;
char* s[20];
char *rez;
int top=0;
void p(double rez)
{ char k[10];
sprintf(k,"%f",rez);

s[top]=new char[strlen(k)-1];
strcpy(s[top],k);
cout<<s[top]<<' '<<'\n';
top++;

}
char* pop()
{free(rez);
top--;
cout<<s[top];
return s[top];
//rez=new char[strlen(s[top]-1)];
//strcpy(rez,s[top]);
}

int main()
{char* r2;
char* r3;
int r1=1234;
double d1=12.34,d2=123.44;
p(d1);
p(d2);
d1=atof(pop());
cout<<d1;



return 0;
}