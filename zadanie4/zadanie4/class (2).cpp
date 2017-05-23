#include "zagolovok.h"
#include <iostream>
#include <math.h>
#include <string.h>
using namespace std;


Air_competition::Air_competition()//konstructor dlia videlenia pamiati pod stroku;
	{
		name=new char[50];
	}

Air_competition::Air_competition(char* name1,int speed1, int force1) //konstructor c izvestnimi znacheniami poley;
	{
		name=new char[strlen(name1)+1];
		strcpy(name,name1);
		speed=speed1;
		force=force1;
	}
Air_competition::~Air_competition(){ delete[]name;}

Air_competition::Air_competition(const Air_competition& rez){
	force=rez.force;
	speed=rez.speed;
	for(int i=0;i<(strlen(rez.name));i++)
		name[i]=rez.name[i];

}
const Air_competition& Air_competition::operator=(const Air_competition& v){
	speed=v.speed;
	force=v.force;
	if(this!=&v){ delete[]name;
	name=new char[strlen(v.name)+1];
	for(int i=0; i<(strlen(v.name));i++)
		name[i]=v.name[i];}

return *this;
}



void Air_competition::show_player()
 {
  cout<< "\n Name is   " << name << ":";
  cout<< "\n    Speed  is    " << speed ;
  cout<< "\n    Force is     " << force;

 }

void Air_competition::get_player() 
{
  cout<< "\n Name is:> ";
   cin >> name;
  cout<< "\n    Speed  :>    "; 
   cin >> speed ;
  cout<< "\n    Force :>     "; 
   cin >> force;
 }
void change_speed(Air_competition & b,int d){
	b.speed=d;
}
std::ostream& operator <<(ostream& out ,Air_competition & b)
{
 b.show_player();
  return out; // for example cout << Air_competition << "STROKA";
}

std::istream& operator >>(istream& in ,Air_competition & b)
{
 b.get_player();
  return in; // for example cin >> Air_competition >> STR;
}






Airplane::Airplane(){name=new char[50];}
Airplane::Airplane(char *n,int sp,int f,int t):Air_competition(n,sp,f)
	{kol_topliva=t;
	}
void Airplane::show_player() 
  {
   Air_competition::show_player();
   cout<<"\n    toplivo: " << kol_topliva;
            cout<<"\n It is Airplane!\n\n\n";
  }

void Airplane::get_player()
  {
   cout<<"\n Class Airplane:";
   
   Air_competition::get_player();
   cout<<"\n  toplivo:>    ";
   cin >> kol_topliva;
  }
bool Airplane::operator<(int i)
{if((kol_topliva-i)>=0){ return true;}
  else return false;}


bool Airplane::operator >(Air_competition& air){
return (speed+force)>(air.speed+air.force); 

}







Balloon::Balloon(){name=new char[50];}
Balloon::Balloon(char* n,int sp,int f,int v,int w):Air_competition(n,sp,f){
	v_gaza=v;
	wind_speed=w;
	}
void Balloon::show_player()
  {
   Air_competition::show_player();
   cout<<"\n    obyom gaza: " << v_gaza;
   cout<<"\n    skorost vetra: " << wind_speed;  
   cout<<"\n It is Ballon!\n\n\n";
  }

void Balloon::get_player()
  {
   cout<<"\n Class Ballon:";
   
   Air_competition::get_player();
   cout<<"\nobyom gaza:>    ";
   cin >> v_gaza;
   cout<<"\nskorost vetra:>    ";
   cin >> wind_speed;
  }
bool Balloon::operator<(int h){
	if((v_gaza-h)>=0) {return true;}
	else {return false;}}
bool Balloon::operator >(Air_competition& air){
return (speed+force)>(air.speed+air.force); 
}
int operator++(Balloon& b){
	return (b.speed+b.wind_speed);
}






Bird::Bird(){name=new char[50];}
Bird::Bird(char* n,int sp,int f,int v):Air_competition(n,sp,f){
	razmah_kr=v;
	}
void Bird::show_player() 
  {
   Air_competition::show_player();
   cout<<"\n    razmah kriliev: " << razmah_kr;  
   cout<<"\n It is Bird!\n\n\n";
  }

void Bird::get_player()
  {
   cout<<"\n Class Bird:";
   
   Air_competition::get_player();
   cout<<"\nrazmah kriliev:>    ";
   cin >> razmah_kr;
  }

bool Bird::operator >(Air_competition& air){

return (razmah_kr*speed+force)>(air.speed+air.force); 

}

bool operator--(Bird& b)//hvatit li sili ptici doletet;
{
	int h=b.force;
	if((h-3)>=0) return true;
	else return false;
}
void operator++(Bird& c)//ustalas ptici za ves put!;
{
--c.force;
--c.force;
--c.force;
}

