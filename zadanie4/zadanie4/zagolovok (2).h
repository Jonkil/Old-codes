#include <iostream>
#include <math.h>
#include <string.h>
using namespace std;



class Air_competition{
public:char * name;
	int speed;
	int force;
public:
	Air_competition();
	Air_competition(char* name1,int speed1, int force1);
    ~Air_competition();
    Air_competition(const Air_competition& rez);
	const Air_competition& operator=(const Air_competition& v);

	
	virtual void show_player();
	virtual void get_player();
 friend void change_speed(Air_competition & b,int);
};
void change_speed(Air_competition & b,int);
ostream& operator<<(ostream& out ,Air_competition & b);
istream& operator>>(istream& in ,Air_competition & b);



class Airplane:public Air_competition{
	int kol_topliva;
public:
  Airplane();
  ~Airplane();
  Airplane(char *n,int sp,int f,int t);
  void show_player();
  void get_player();
  bool operator<(int i);
  bool operator>(Air_competition& air);
};



class Balloon:public Air_competition{
int v_gaza;
int wind_speed;
public:
	Balloon();
		~Balloon();

	Balloon(char* n,int sp,int f,int v,int w);
void show_player();
  void get_player();
  bool operator<(int h);
  bool operator>(Air_competition& air);
  friend int operator++(Balloon&);


};
int operator++(Balloon&);


class Bird:public Air_competition{
int razmah_kr;
public:
	Bird();
		~Bird();

	Bird(char* n,int sp,int f,int v);
void show_player();

  void get_player();
  bool operator>(Air_competition& air);
  friend bool operator--(Bird& h);
  friend void operator++(Bird& f);
};
bool operator--(Bird& h);
void operator++(Bird& f);
