
#include<fstream.h>
#include<iostream.h>
int main(){
char name1[30],name2[30];
char symb;
cin>>name1;
ofstream f(name1,ios::app);
if(!f) {cout<<"not open f\n";return 1;}
cin>>name2;
ifstream g(name2,ios::in);
if(!g) {cout<<"not open g\n";return 1;}
while(g.get(symb)){
	f.put(symb);}
f.put('\0');
g.close;
f.close;
return 0;
}