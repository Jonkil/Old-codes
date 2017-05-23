#include<iostream>
#include<string.h>
using namespace std;
template <class A> void sdvig(A *a,int m,int n){
	A *b;
	b=new A[m];
	int i,j;
	for(i=0,j=m-n;i<n,j<m;i++,j++){
		b[j]=a[i];
	}
	for(j=n,i=0;j<m,i<m-n;j++,i++){
	b[i]=a[j];
	}
	for(i=0;i<m;i++){
		a[i]=b[i];
	}
}
void sdvig(char **a,int m, int n){
	 char **b;
	int i,j;
	b=new char*[m];
    for(i=0;i<m;i++){
    b[i]=new char[20];
	}
	for(i=0,j=m-n;i<n,j<m;i++,j++){
	strcpy(b[j],a[i]);
	}
	for(j=n,i=0;j<m,i<m-n;j++,i++){

	strcpy(b[i],a[j]);
	}
	for(i=0;i<m;i++){
	strcpy(a[i],b[i]);
	}
}

int main(){
	try{
 int* aint,m,n,i,j,k=20;
 char **achar;
 char *achar1;

//для целых чисел
 cout<<"vvedite kol-vo elementov v massive celih chisel\n";
 cin>>m;
 if(m<=0) throw 1;
 aint=new int[m];
 cout<<"\nvvedite n ";
 cin>>n;
 if(n<=0) throw 1;
 cout<<"\nvvedite elementi massiva\n";
 for(i=0;i<m;i++){
	cin>>aint[i];
	cout<<" ";
 }
 cout<<'\n';
 cout<<"\nvvedennie  elementi massiva\n";
 for(i=0;i<m;i++){
	cout<<aint[i]<<" ";
 }
 cout<<'\n';
 sdvig(aint,m,n);
 cout<<"\nelementi massiva posle sdviga\n";
for(i=0;i<m;i++){
	cout<<aint[i]<<" ";
 }
cout<<'\n';
//для символов
 cout<<"\nvvedite kol-vo elementov v massive symvolov\n";
 cin>>m;
 if(m<=0) throw 1;
achar1=new char[m];
cout<<"\nvvedite n ";
cin>>n;
if(n<0) throw 1;
cout<<"\nvvedite elementi massiva\n";
 for(i=0;i<m;i++){
	 cin>>achar1[i];}
 cout<<'\n';
cout<<"\nvvedennie  elementi massiva\n";
for(i=0;i<m;i++){
cout<<achar1[i]<<" "; }
cout<<'\n';
 sdvig(achar1,m,n);
cout<<"\nelementi massiva posle sdviga\n";
for(i=0;i<m;i++){
	cout<<achar1[i]<<" ";}
cout<<'\n';

 //для строк
 cout<<"\nvvedite kol-vo elementov v massive strok\n";
 cin>>m;
 if(m<=0) throw 1;
achar=new char*[m];
for(i=0;i<m;i++){
	achar[i]=new char[k];}

cout<<"\nvvedite n ";
cin>>n;
if(n<=0) throw 1;
cout<<"\nvvedite elementi massiva\n";
 for(i=0;i<m;i++){
	 j=0;
	 cin.get(achar[i][j]);
	 while((achar[i][j]!=' ')&&(achar[i][j]!='\n')){
		 j++;
		 cin.get(achar[i][j]);
	 }
	 if(j>20) {throw "exec"; exit(1);}
}
 cout<<'\n';
cout<<"\nvvedennie  elementi massiva\n";
for(i=0;i<m;i++){
cout<<achar[i]<<" "; }
 sdvig(achar,m,n);
 cout<<'\n';
cout<<"\nelementi massiva posle sdviga\n";
for(i=0;i<m;i++){
	cout<<achar[i]<<" ";}
cout<<'\n';
cout<<'\n';
}
catch(int){cout<<"\noshibka pri vvode\n";return 1;}
catch(...){cout<<"\nError!!\n";exit(1) ;}
return 0;
}
