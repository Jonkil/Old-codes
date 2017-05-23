
#include<fstream.h>
#include<iostream.h>
int main(){
char name1[30],name2[30];
char symb,h;
ofstream f;
ifstream g;
cout<<"vvedite f\n";
cin>>name1;
f.open(name1,ios::app);
if(!f) {cout<<"not open f\n";return 1;}
cout<<"vvedite g\n";
cin>>name2;
g.open(name2,ios::in);
if(!g) {cout<<"not open g\n";return 1;}
f.put('\n');
f.put('\n');
f.put('\n');
while(g.get(symb)){
h=symb;
f.put(h);}
return 0;
}

