#include<iostream>
using namespace std;
template <class A> void sdvig(A *a,int m,int n){
	A *b;
	b=new A[m];
	int i,j;
	for(i=0;i<m-n;i++){
	for(j=n;j<m;j++){
           b[i]=a[j];
		}
	}
    for(i=0;i<n;i++){
	for(j=m-n;j<m;j++){
		b[j]=a[i];
	}
	}
	for(i=0;i<m;i++){
		a[i]=b[i];
	}
}
int main(){
 int* aint,m,n,i;
 //double *adouble,k,l;
 cout<<"vvedite kol-vo elementov v massive aint\n";
 cin>>m;
 aint=new int[m];
 cout<<"\nvvedite n";
 cin>>n;
 cout<<"\nvvedite elementi massiva\n";
 for(i=0;i<m;i++){
	cin>>aint[i];
	cout<<" ";
 }
 cout<<"\nvvedennie  elementi massiva\n";
 for(i=0;i<m;i++){
	cout<<aint[i]<<" ";
 }
 sdvig(aint,m,n);
 return 0;
}
