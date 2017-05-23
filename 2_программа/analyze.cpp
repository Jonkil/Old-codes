#include <iostream>
#include <ctype.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <list>
using namespace std;

#define SIZE 100
#define TID_SIZE 150
#define TNUM_SIZE 150
#define STR_SIZE 150

enum lex_type {LEX_AND,LEX_ELSE,LEX_FOR,LEX_IF,
	       LEX_INT,LEX_NOT,LEX_OR,LEX_REAL,LEX_CONTINUE,LEX_PROG,LEX_READ,LEX_WHILE,LEX_WRITE,POLIZ_GO,POLIZ_FGO,POLIZ_ADRESS};



enum del_type {LEX_RAVNO,LEX_EQ,LEX_NEQ,LEX_GT,LEX_LT,LEX_GE,LEX_LE, LEX_PLUS, LEX_MINUS,
	       LEX_AST, LEX_SLASH,LEX_Z, LEX_TZ, LEX_LBR, LEX_RBR, LEX_BEGIN,
		 LEX_END,LEX_DELIT};

class identificator{
public:
	char* name;
	bool declare;
	bool assign;
	char type[7];
	int value_i;
	double value_r;
	identificator(const char* s) 
		{
		name=new char[strlen(s)+1];
		strcpy(name, s);
		declare=false;
		assign=false;
		value_i=0;
		value_r=0.0;
		}
	~identificator() { if (name) delete[] name; }
	void set_name(const char* n) { name=new char[strlen(n)+1]; strcpy(name, n); }
	char *get_name() { return name; }
	char *get_type(){return type;}
	void set_decl() { declare=true; }
	bool get_decl() { return declare; }
	void set_as() { assign=true; }
	bool get_as() { return assign; }
	void set_val_i(int k){value_i=k;}
	void set_val_r(double k){value_r=k;}
	int get_val_i(){return value_i;}
	double get_val_r(){return value_r;}
	friend class Executer;
};
					
			class lex {
					int group;
					int value;
					int colsl;
			public:	lex (int g=-1, int v=-1,int sl=-1): group(g), value(v),colsl(sl){ }

				friend ostream &operator<<(ostream& s, lex cur_lex)
                      {
                        s << '(' << cur_lex.group << ',' << cur_lex.value << ',' <<cur_lex.colsl<< ") ";
                        return s;
                      }
				lex operator=(lex cur_lex)
					{
					this->group=cur_lex.group;
					this->value=cur_lex.value;
					this->colsl=cur_lex.colsl;
					return *this;
					}
	
				int get_val() { return value;}
				int get_gr() { return group; }
				int get_colsl(){return colsl;}
	
			};
	char* TW[]={"and","else","for", "if", "int", "not", "or","real","continue","program", "read","while", "write", NULL};
	char* TD[]={"=", "==","!=", ">", "<", ">=", "<=", "+", "-", "*", "/",",",";","(", ")", "{", "}","%",NULL};
	char* STR[STR_SIZE];
	identificator *TID[TID_SIZE];
	int* TNUM_INT[TNUM_SIZE];
	double* TNUM_REAL[TNUM_SIZE];
	int j,n,col=0,cnt=0;
	char c,c1, pred;
	int buf_top=0;
	char buf[SIZE];


class lex_analyze{
	enum state {H, ID, NUM, FIN, DELIM, COM,STR1};
	state CS;
	ifstream in;
	ofstream outp;

	void clear()
	{buf_top=0;
	 memset(buf, '\0', SIZE);}

			void add()
			{
			 buf[buf_top++]=c;
			}

	int look_id(identificator **cur_id)
	{ int i=0;
	  while (cur_id[i]!=NULL)
	{
	 if (!strcmp(cur_id[i]->get_name(), buf)) return i;
	 i++;
	}
	 return -1;
	}

			int put_id(identificator **d)
			{ 
			   for (int i=0; i<TID_SIZE; i++)
				if (d[i]==NULL) { d[i]=new identificator(buf);  return i;}
			return -1;
			}

	int look_for(char **t)
	{ 
	  int i=0;
	  while (t[i]!=NULL)
		{if (!strcmp(t[i], buf)) return i; i++;}
	 return -1;
	}

			int looknum_int(int **d)
			{ int num=atoi(buf);
			  int i=0;
			  while (d[i]!=NULL)
				{
				 if (*(d[i])==num) return i;
				 i++;
				}
			 return -1;
			}
		int put(char **t)
		{for (int i=0; i<STR_SIZE; i++)
				if (t[i]==NULL) {t[i]=(char*)malloc((strlen(buf))*sizeof(char));strcpy(t[i],buf);  return i;}
			return -1;
		}
int putnum_int()
{ int i;
  for (i=0; i<150; i++)
	if (TNUM_INT[i]==NULL)  {int a=atoi(buf); TNUM_INT[i]=new int(a); cout<<*(TNUM_INT[i])<<'\n';return i; }
}
			int looknum_real(double **d)
			{ double num=atof(buf);
			  int i=0;
			  while (d[i]!=NULL)
				{
				 if (*(d[i])==num) return i;
				 i++;
				}
			 return -1;
			}
int putnum_real()
{ int i;
  for (i=0; i<150; i++)
	if (TNUM_REAL[i]==NULL)  {double b=atof(buf); TNUM_REAL[i]=new double(b);cout<<*(TNUM_REAL[i])<<'\n'; return i; }
}


			void free()
			{
			 int i;
			for(i=0; i<TID_SIZE; i++)
				if (TID[i]!=NULL) {  delete TID[i]; }
					else break;
 
			for(i=0; i<TNUM_SIZE; i++)
				if(TNUM_INT[i]!=NULL) { delete TNUM_INT[i]; }
			for(i=0; i<TNUM_SIZE; i++)
				if(TNUM_REAL[i]!=NULL) { delete TNUM_REAL[i]; }
			}

public:
				lex_analyze(const char *inp, const char *out) 
					{
					in.open(inp);
					outp.open(out, ios::out|ios::trunc);
					CS=H;
					clear();
					 }
				~lex_analyze() { in.close(); free(); }
				
				void get_c() { pred=c; in.get(c); }
				void put_c() { outp.put(c);}

				
void analizator(){
get_c();
		CS=H;
		 do
		 {
		   switch(CS)
			{
			case H: if (c==' ' || c=='\t' || c=='\r') get_c();
				else if(c=='\n'){cnt++; get_c();}
				else if(c=='"'){clear();get_c();CS=STR1;}
				else
				if (isalpha(c))
				{
				  clear();
				  add();
				  get_c();
				  CS=ID; 
				}
				else if (isdigit(c))
				{clear();
				  add();
				  get_c();
				  CS=NUM;
				}
				else if (c=='-')
				{
				clear();
					 if (pred=='=' || pred=='>' || pred=='<' || pred=='(' 
						|| pred=='+' || pred=='-' || pred=='*' || pred=='/'|| pred=='%' || isspace(pred))
						{
						 add();
						 CS=NUM;
						 get_c();
						}
					 else 
						{
						 add();
						  j=look_for(TD); if (j==-1) throw "v tablice takogo znaka net!!!";
						 outp<< lex(2,j,cnt); get_c(); CS=H; clear();
						}
				
				}
				else if (c=='/')
				{
				  get_c();
				  if (c=='*')  { CS=COM; get_c(); }
					else { CS=H; outp << lex(2, LEX_SLASH,cnt); }
				}
                else CS=DELIM;
				break;

			case NUM:
				
				if (isdigit(c))
				{
				add(); get_c();CS=NUM;
				}
				else if(c=='.'){if(!(isdigit(pred))) throw "pered to4koy net 4isla" ;
					else {if(col>0) throw "mnogo to4ek v 4isle";
							else {col++;add();get_c();CS=NUM;}}
				}
				else {
					if(col==0){ 
					if ((j=looknum_int(TNUM_INT))!=-1)  { outp << lex(3,j,cnt); col=0; CS=H; }
					else { j=putnum_int(); outp << lex(3,j,cnt);col=0; CS=H; }
					} 
					else if(col==1){
					if ((j=looknum_real(TNUM_REAL))!=-1)  { outp << lex(5,j,cnt); col=0; CS=H; }
					else { j=putnum_real(); outp << lex(5,j,cnt);col=0; CS=H; }
					}
				}
				break;


			case ID: if (isalpha(c) || isdigit(c) || c=='_')
				{
				 add();
				 get_c();
				}
				else if ((j=look_for(TW))!=-1) 
					{ outp << lex(1,j,cnt); clear(); CS=H;}
				else if ((j=look_id(TID))==-1) 
					{ j=put_id(TID); outp << lex(4,j,cnt); clear(); CS=H;}
				else  { outp<< lex(4,j,cnt); CS=H; clear(); }
				break;	
			case DELIM: clear(); 
				if (c=='=')
					{
					get_c();
					if (c=='=') { outp << lex(2,LEX_EQ,cnt); get_c(); CS=H; }
					else { outp << lex(2, LEX_RAVNO,cnt); CS=H; }
					}
				  else if (c=='!')
					{
					get_c(); if (c=='=') {outp << lex(2, LEX_NEQ,cnt); get_c(); CS=H; }
							else throw c;
					}
				  else if (c=='>')
					{
					get_c();
					if (c=='=') {outp<< lex(2, LEX_GE,cnt); get_c(); CS=H; }
						else  {outp<< lex(2, LEX_GT,cnt); CS=H; }
					}
				  else if (c=='<')
					{
					get_c();
					if (c=='=') { outp<< lex(2, LEX_LE,cnt); get_c(); CS=H; }
						else  {outp<< lex(2, LEX_LT,cnt); CS=H; }
					}
				  else if (c=='{')
					{
					 { outp << lex(2, LEX_BEGIN,cnt); get_c(); CS=H; }
					}
				  else if (c=='}')
					{
					 { outp << lex(2, LEX_END,cnt); get_c(); CS=H; }
					}
				  else 
					{
					 add();
					 j=look_for(TD); if (j==-1) {cout<<buf<<' ';throw "unexpected symbol";}
					 outp << lex(2,j,cnt); get_c(); CS=H;
					}
				   break;
							case STR1:
							while (c!='"')
								 {
								  add(); get_c();
								 }
                    
								 if ((j=look_for(STR))!=-1)  { outp << lex(7,j,cnt); CS=H; }
								 else { j=put(STR); outp<< lex(7,j,cnt); CS=H; }
								 get_c();
								 break;
			case COM: if (c=='*') 
				  {
				   get_c();
				   if (c=='/') { get_c(); CS=H; }
				  }
				 else if (c=='@') throw "neverno zapisan komentatiy";
				 else get_c();
				break;
		  }	
 		} while(!(in.eof()));
		outp.close();
		return;
	}
};



//класс ПОЛИЗ
class Poliz {
    lex *p;
     int free;
public:
     Poliz()
     {
     p=new lex[1000];
     free = 0;
     }

     ~Poliz() { delete[] p; }

     void put(lex l)
     {
     put_l(l, free);
     free++;
     }
     void put_l(lex l, int i) { p[i]=l; }

     int get_free() { return free; }

     lex & operator[](int i)
     {
     if (i > 1000) { cout<<"error v poliz"; throw this; }
     else
     if (i >free) { cout<<"error v poliz"; throw this; }
     else
     return p[i];
     }

     void print()
     { 
     cout <<"POLIZ: " << endl;
     cout << "free: " << free << endl;
     for (int i=0; i<free; ++i)
     cout << p[i];
     cout << endl;
     }
     void blank(){free++;}
     
};





//синтаксический и семантический анализ идет дальше :)))

class syntax_analyze{
	lex cur_lex;
	ifstream inp;
	lex next;
	bool if_peek;
	int enter;

	char i[5][5];
	char* s_my[30];
	int top_i;
	int top_s;
	void P();
	void Opt();
	void Opt1();
	void Per();
	void Opr();
	void Opr1();
	void V();
	void V1_2();
	void V1_3();
	void V1();
	void T();
	void F();
	void OprV();
public:
	Poliz prog;
    syntax_analyze(const char *input){

		if_peek=false;
		enter=0;
		top_i=0;
		top_s=0;
		inp.open(input);
	}
	~syntax_analyze() { inp.close();}
 
	void gl()
		{
		 if (if_peek) { cur_lex=next; if_peek=false; return; }
		if (inp.eof()) return;
		int g,v,y;
		inp.get(c);
		inp.get(c);
		g=c-'0';
		inp.get(c);
		inp.get(c);
		v=c-'0';
		inp.get(c);
		if(isdigit(c))
		{
			 v*=10;
			 v+=c-'0';
	    inp.get(c);
		 }
		inp.get(c);
		y=c-'0';
	    inp.get(c);
		if(isdigit(c))
		{
			 y*=10;
			 y+=c-'0';
		inp.get(c);
		}
		enter=y;
		cur_lex=lex(g,v,y);
		inp.get(c);
		}

	void peek()
		{
		if_peek=true;
		if (inp.eof()) return;
	    int g,v,y;
		inp.get(c);
		inp.get(c);
		g=c-'0';
		inp.get(c);
		inp.get(c);
		v=c-'0';
		inp.get(c);
		if(isdigit(c))
		{
			 v*=10;
			 v+=c-'0';
	    inp.get(c);
		 }
		inp.get(c);
		y=c-'0';
	    inp.get(c);
		if(isdigit(c))
		{
			 y*=10;
			 y+=c-'0';
		inp.get(c);
		}
		enter=y;
		next=lex(g,v,y);
		inp.get(c); 
		}
	
	bool eql(int g, int v)
		{
		 return ((cur_lex.get_val()==v) && (cur_lex.get_gr()==g));
		}

	void syn_an()
		{
		 gl();
		 P();
		cout << "done syntax analyze!!!!!!!'\n'";
		}


//функции семантического анализа
	char *gettype(char *op,char *t1,char *t2){

		if (!strcmp(t1,"int")&&!strcmp(t2,"int")){
				if(!strcmp(op,"+") || !strcmp(op,"-")|| !strcmp(op,"*") ||!strcmp(op,"/")||!strcmp(op,"%")|| !strcmp(op,"<") || !strcmp(op,">") || !strcmp(op,"<=")||!strcmp(op,">=") ||!strcmp(op,"==") || !strcmp(op,"!=")|| !strcmp(op,"and")||!strcmp(op,"or")||!strcmp(op,"="))
				{ cout<<"int'\n'";return "int";} 
				else return "no";
				}
		
		else if(!strcmp(t1,"real")&&!strcmp(t2,"real"))
		{if(!strcmp(op,"+") || !strcmp(op,"-")|| !strcmp(op,"*") ||!strcmp(op,"/")|| !strcmp(op,"<") || !strcmp(op,">") || !strcmp(op,"<=")||!strcmp(op,">=") || !strcmp(op,"==") || !strcmp(op,"!=") || !strcmp(op,"="))
		{cout<<"real'\n'"; return "real";} 
		else return "no";
		}
	

	else if(!strcmp(t1,"real")&&!strcmp(t2,"int"))
		{if(!strcmp(op,"+") || !strcmp(op,"-")|| !strcmp(op,"*") ||!strcmp(op,"/")){cout<<"real'\n'"; return "real";}
		else if(!strcmp(op,"<") || !strcmp(op,">")|| !strcmp(op,"<=")||!strcmp(op,">=") || !strcmp(op,"==") || !strcmp(op,"!=") ||!strcmp(op,"="))
		{cout<<"int'\n'";return "int";}
		else return"no";
		}
	

	else 	if(!strcmp(t1,"int")&&!strcmp(t2,"real"))
	{if(!strcmp(op,"+") || !strcmp(op,"-")|| !strcmp(op,"*")|| !strcmp(op,"/")||!strcmp(op,"="))
	{cout<<"real'\n'"; return "real";}
	else if(!strcmp(op,"<") || !strcmp(op,">")|| !strcmp(op,"<=")|| !strcmp(op,">=") || !strcmp(op,"==") || !strcmp(op,"!="))
	{cout<<"int'\n'"; return "int";}
	else return "no";
	}
	else return "no";
}


void spush(char *k){
	if(top_s!=30){s_my[top_s]=new char[strlen(k)-1];strcpy(s_my[top_s],k);top_s++;cout<<"spush="<<k<<'\n';}
	else throw "stack is full!!";
}
char *spop(){
	if(top_s==0)throw"stack is empty!!";
    else{top_s--;return s_my[top_s];}
}
lex make_op(char *v){
int i;
if((i=look_me(TW,v))!=-1) return lex(1,i);
else if((i=look_me(TD,v))!=-1) return lex(2,i);
}

int look_me(char **t,char *g){
int i=0;
while(t[i]!=NULL){
	if(!strcmp(t[i], g))return i; i++;}
	return -1;
}

		void checkop()
			{char *t1;char *t2;char *op;
			char *res;
			t1=spop();
			op=spop();
			t2=spop();
			res=gettype(op,t1,t2);
			cout<<res<<"=res'\n'";
			if(strcmp(res,"no")) spush(res);//vse ok!!!
			else{throw "types ne sootvetstvuyut!!";}
			prog.put(make_op(op));
			}

	void checknot(){
	if(strcmp(spop(),"int")){throw "operand u operacii not ne int!!";}
	else spush("int");
	}


char *ipop(){return i[1];}
void ipush(char *l){top_i++;strcpy(i[top_i],l);}
void ifree(){top_i=0;}

			void dec_id(const int i){
				char *s;
				s=ipop();
				if(TID[i]->declare){cout <<enter; throw ":id uje opisan!!";}
				TID[i]->set_decl();
				strcpy(TID[i]->type,s);
			}
			bool checkid(const char *s2){
				char *s1;
				s1=ipop();
				if(strcmp(s1,s2))return false;
				return true;
			}

};




	//сами функции синтаксического анализа!!
	void syntax_analyze::P()
	{
	  if (eql(1,9))
		{
		 gl();
		 if (eql(2,15))
			{
			 gl();
			 Opt();
			 Opr();
		  if (!eql(2,16)) {cout <<enter;cout<<":error nehvataet }  "; throw cur_lex; }
			}
		 else{cout <<enter;cout<<":error nehvataet {  "; throw cur_lex;}
		}
	else	 { cout <<enter; cout<<":error v konce neverno!!"; throw cur_lex; }
		
	}

			void syntax_analyze::Opt()
			{
			if (!eql(1,7) && !eql(1,4)) { return;}
			  while (eql(1,7) || eql(1,4))
			  {if(eql(1,7)) ipush("real");
			  else if(eql(1,4)) ipush("int");
				  gl();
				  Per();
				  Opt1();
				  if (!eql(2,12)) {cout <<'\n'<<enter<<":error net razdelitelia " ; throw cur_lex; }
				  else gl();
			  }
			}
	

				void syntax_analyze::Opt1()
				{
				if (!eql(2,11) ) {ifree(); return;}
				while (eql(2,11)){
		  					gl();
							 Per();
							}
				}


	void syntax_analyze::Per()
	{lex pre;
	int my;
	 if (cur_lex.get_gr()==4)
		{dec_id(cur_lex.get_val());
		pre=cur_lex;
		  gl();
		  if (eql(2,0)) {my=pre.get_val(); prog.put(lex(6,my));gl();
		  if (cur_lex.get_gr()==3){prog.put(cur_lex);prog.put(lex(2,0));if(checkid("int")) gl(); else {cout <<enter; throw":id i konstanta raznih tipov";}}
		  else if(cur_lex.get_gr()==5){prog.put(cur_lex);prog.put(lex(2,0));if (checkid("real")) gl();else {cout <<enter;throw":id i konstanta raznih tipov";}}
		  else {cout <<enter<<":posle ravno ne chislo " ; throw cur_lex;} 
		  }
		}
	else { cout <<enter<<":error not ID " ;throw cur_lex; }
	}


void syntax_analyze::Opr()
	{
	while (!eql(2,16))
		Opr1();
	}

void syntax_analyze::Opr1(){
	char *x;
	int p1,p2,p3,p4,p0;
	
	
	if (eql(1,3))//IF
			{
			 gl();
			 if (!eql(2,13)) {cout <<enter<<":error net ( " ; throw cur_lex; }
			 gl();
			 V();
				x=spop();
				if(strcmp(x,"int")){cout <<enter; throw ":v IF vyrajenie ne int";}
			p1=prog.get_free();
			prog.blank();
			prog.put(lex(1,POLIZ_FGO));
			 if(!eql(2,14)) { cout <<enter<<":error net )  " ;throw cur_lex; }
		
			 gl();
			 Opr1();
			 p2=prog.get_free();
			 prog.blank();
			 prog.put(lex(1,POLIZ_GO));
			 prog.put_l(lex(0,prog.get_free()),p1);
             if (eql(1,1)) { gl(); Opr1();prog.put_l(lex(0,prog.get_free()),p2);}
			 else{cout<<enter<<"nedopisan operator IF net ELSE";throw cur_lex;}
			}
		else if (eql(1,11))//WHILE
			{
			 gl();
			 if (!eql(2,13))  {cout <<enter<<":error net ( ";throw cur_lex; }
			 gl();
			 p1=prog.get_free();
			 V();
			 
			 x=spop();
			 if(strcmp(x,"int")){cout <<enter; throw ":v WHILE vyrajenie ne int";}
			 if(!eql(2,14)) {cout <<enter<<":error net ) 2 ";throw cur_lex; }
			 gl();
			 p2=prog.get_free();
			 prog.blank();
			 prog.put(lex(1,POLIZ_FGO));
			
			 Opr1();
			 prog.put(lex(0,p1));
			 prog.put(lex(1,POLIZ_GO));
			 prog.put_l(lex(0,prog.get_free()),p2);

			}
		else if (eql(1,12))//WRITE
			{
			 gl();
			 if (!eql(2,13))  { cout <<enter<<":error net ( ";throw cur_lex; }
			 gl();
			 V();
			 	 prog.put(lex(1,12));
			 while (eql(2,11)){
				 gl();
				 V();
				 prog.put(lex(1,12));
			 }
		
			 if (!eql(2,14)) {cout <<enter<<":error net ) 3 ";throw cur_lex; }
			
			
			 gl();
			 if (!eql(2,12)) {cout <<enter<<":error net ; ";throw cur_lex; }
			 gl();
			}
		else if (eql(1,10))//READ
			{
			 gl();
			 if (!eql(2,13))  {cout <<enter<<":error net ( ";throw cur_lex; }
			 gl();
			 if(!(cur_lex.get_gr()==4)) {cout <<enter<<":error ne ID ";throw cur_lex; }
             else if(!(TID[cur_lex.get_val()]->get_decl())){cout<<enter; throw":id ne obyavlen!!";}
			 TID[cur_lex.get_val()]->set_as();
			 prog.put(lex(6,cur_lex.get_val()));
			 prog.put(lex(1,10));
			 gl();
			 if (!eql(2,14)) {cout <<enter<<":error net ) 4 ";throw cur_lex; }
				gl();
			 if (!eql(2,12)) { cout <<enter<<":error net ; ";throw cur_lex; }
			 gl();
			}
	
		 else if (eql(1,2))//FOR
			{
			gl();
			if (!eql(2,13))  {cout <<enter<<":error net ( ";throw cur_lex; }
			
			gl();
			if (!eql(2,12)) { V();x=spop();
			if(strcmp(x,"int")){cout <<enter; throw ":v FOR vyrajenie 1 ne int";} }
			if (!eql(2,12)) { cout <<enter<<":error net ; ";throw cur_lex; }
			gl();
			p3=prog.get_free();
			if (!eql(2,12)) { V();x=spop();
			if(strcmp(x,"int")){cout <<enter; throw ":v FOR vyrajenie 2 ne int";}}
			else prog.put(lex(3,0));
			if (!eql(2,12)) { cout <<enter<<":error net ; ";throw cur_lex; }
			p0=prog.get_free();
			prog.blank();
			prog.put(lex(1,POLIZ_FGO)); 
			p1=prog.get_free();
			prog.blank();
			prog.put(lex(1,POLIZ_GO));
			gl();
			p4=prog.get_free();

			if (!eql(2,14)) {V(); x=spop();
			if(strcmp(x,"int")){cout <<enter; throw ":v FOR vyrajenie 3 ne int";}}
			prog.put(lex(0, p3));
			prog.put(lex(1, POLIZ_GO));


			if (!eql(2,14)) { cout <<enter<<":error net ) 5";throw cur_lex; }
			gl();
			prog.put_l(lex(0,prog.get_free()),p1);
		
			Opr1();
			prog.put(lex(0,p4));
			prog.put(lex(1,POLIZ_GO));
			prog.put_l(lex(0,prog.get_free()),p0);

			}
		else if (eql(2,15))//SOST OPERATOR
			{
			 gl();
			 Opr();
			 if (!eql(2,16)) {cout <<enter<<":error net } "; throw cur_lex; }
	 		 gl();
			}
		
		/*else if (eql(1,8)) //CONTINUE
		{
			prog.put(lex(0,pos[--mytop])); 
			if(pos==0){throw"error v continue";}
			prog.put(lex(1,POLIZ_GO));
			gl();
			if (!eql(2,12)) {cout <<enter<<":error net ; "; throw cur_lex; }
			gl();
			}*/
		else if(cur_lex.get_gr()>2) OprV();
	    else { cout <<enter<<":takogo net!!!";throw cur_lex; }
}

 void syntax_analyze::V()
	{lex pre;
     int zn;
	if (cur_lex.get_gr()==4)
	{pre=cur_lex;
	zn=pre.get_val();
	    if(!(TID[cur_lex.get_val()]->get_decl())){cout<<enter; throw":id ne obyavlen!!";} else{spush(TID[cur_lex.get_val()]->get_type());}
		 peek();
		 if ((next.get_gr()==2)&&(next.get_val()==0))
			{prog.put(lex(6,zn));
			spush("=");
			gl();
			gl();
			V1();
			checkop();
			}
		 else
			{
			// prog.put(pre);
			 V1();
			 if (eql(2,1) || eql(2,2) || eql(2,6) || eql(2,4) || eql(2,3) || eql(2,5))
				{if(eql(2,1))spush("==");
			 else if(eql(2,2))spush("!=");
			 else if(eql(2,6))spush("<=");
			 else if(eql(2,4))spush("<");
			 else if(eql(2,3))spush(">");
			 else if(eql(2,5))spush(">=");
			 gl();
			 V1();
			 checkop();
				}
			}
		}
	else
		{
		 V1();
			 if (eql(2,1) || eql(2,2) || eql(2,6) || eql(2,4) || eql(2,3) || eql(2,5))
				{if(eql(2,1))spush("==");
			 else if(eql(2,2))spush("!=");
			 else if(eql(2,6))spush("<=");
			 else if(eql(2,4))spush("<");
			 else if(eql(2,3))spush(">");
			 else if(eql(2,5))spush(">=");
				 gl();
				 V1();
				checkop();
				}
		}

	}

void syntax_analyze::V1()
	{
	 T();//+,-.or
	 while (eql(2,7) || eql(2,8) || eql(1,6))
	   { if(eql(2,7))spush("+");
	 else if(eql(2,8))spush("-");
	 else if(eql(1,6))spush("or");
	
		gl(); T();checkop();}

	}
void syntax_analyze::T()
	{
	 F();//*,/,%/and
	  while (eql(2, 9) || eql(2,10) ||eql(2,17)||eql(1,0))
	     {
		if(eql(2, 9))spush("*");
	
		else if(eql(2,10))spush("/");
	    else if(eql(2,17))spush("%");
		else if(eql(1,0))spush("and");
			
		gl();F(); checkop();}
	}

void syntax_analyze::F()
	{

	if (eql(1,5))
		{
		 gl();
		 F();
		 checknot();
		 prog.put(lex(1,5));
		}//sled (v);
	else if (eql(2,13))
		{
		 gl();
		 V();
		 if (!eql(2,14))  {cout <<enter<<":error net ) ";throw cur_lex; }
		 gl();
		} 
	else if(cur_lex.get_gr()==3){prog.put(cur_lex);spush("int");gl();}
	else if(cur_lex.get_gr()==4) {prog.put(cur_lex);if(!(TID[cur_lex.get_val()]->get_decl())){cout<<enter; throw":id ne obyavlen!!";} else {spush(TID[cur_lex.get_val()]->get_type());gl();}}
	else if(cur_lex.get_gr()==5){prog.put(cur_lex);spush("real");gl();}
	else if(cur_lex.get_gr()==7){prog.put(cur_lex);gl();}
    else {cout <<enter<<":error v virajenii!! "; throw cur_lex; }
	}

void syntax_analyze::OprV()
	{
	 V();
if (!eql(2,12)) {cout <<enter<<":error net ;"; throw cur_lex; }
else	gl();
	}








//КЛАСС ЕКЗЕК

class Executer{
	lex elem;
     list<int> st1;
	 list<double>st2;
     /*double* st1;
	 int* st2;
	 int top1;
	 int top2;*/
	 
public:
	
   
    Executer() {lex elem();/*top1=0;top2=0;st1=new double[50];st2=new int[50];*/}
	 ~Executer() { elem.~lex();}
/////////////////////////
	void push(double a){
		 //cout<<"push="<<a<<' ';
		           st2.push_back(a);
	 }
	 double pop(){
		 double a=st2.back();
          st2.pop_back();
          return a;
	 }

	 void add(int a){
		 st1.push_back(a);}
	 int get(){
		int  a=st1.back();
          st1.pop_back();
          return a;
	 }
	      void print() {
     list<int>:: iterator it;
          it=st1.begin();
          cout << "Stack: \n";
          while(it!=st1.end())
          cout << *it++ << " ";
          cout << endl;
          }
		void print1() {
     list<double>:: iterator it;
          it=st2.begin();
          cout << "Stack: \n";
          while(it!=st2.end())
          cout << *it++ << " ";
          cout << endl;
          }

///////////////////////////

	void go(Poliz &p);
};

void Executer::go(Poliz &p){
p.print();
bool cont=false;
int size=p.get_free(), index=0;
double d;
int i,j,rez,k;
char* type;
 while (index<size) {
     elem=p[index];
	 if (elem.get_gr()==0){i=elem.get_val();push((double)i);}

     else if (elem.get_gr()==1) {
               switch(elem.get_val()) {
				   
                    case 0: get();get(); i=(int)pop();//and
								d=(double)(((int)pop())&&i);
                              push(d);
								add(3);
                              break;
               
                    case 5:get();
                             i=(int)pop();//not
							d=(double)(!i);     
							push(d);
							add(3);
                              break;
                    case 6: get();get(); 
							i=(int)pop();//or
							d=(double)((int)pop()||i);
                            push(d);
							add(3);
                            break;

                    case 10:    i=(int)pop();//read
                              type=TID[i]->get_type();
                              if (!strcmp(type, "int")) {
								cout <<'\n'<< "INT value : " << TID[i]->get_name() << endl;
                                   cin >> rez;
                              TID[i]->set_val_i(rez);}
                              else if (!strcmp(type, "real")) {
                
									cout <<'\n'<<"REAL value: " << TID[i]->get_name() <<endl;
                                   
                                   cin >> d;
                                  TID[i]->set_val_r(d);
}
                              TID[i]->set_as();
                              break;
                    case 12:	i=get();//write
								if(i==3){rez=(int)pop();
								if (p[index-1].get_gr()==7) cout <<' '<<STR[rez]<<' ';
								else cout <<' '<< rez<<'\n';
								}else if(i==5){
								d=pop();
								cout<<' '<<d<<'\n';}
								break;
					

                    case POLIZ_GO:
                              index=(int)pop()-1;
                              break;

                    case POLIZ_FGO:
                              i=(int)pop()-1;
                              if (!((int)pop())) index=i;
                              break;
					
                         }
                    }
     else if (elem.get_gr()==2) {
               switch (elem.get_val()) {
                  
			   case 0:
				   i=get();
				   k=get();
				   if(k==5 && i==3){
					   rez=(int)pop();
					   j=(int)pop();
					TID[j]->set_val_r((double)rez);
					push((double)rez);add(5);
				   }else if(k==3&&i==5){
					   d=pop();
					   j=(int)pop();
					 TID[j]->set_val_i((int)d);
					push(d);add(3);
				   }
				   else if(k==5 && i==5){
					d=pop();
					 j=(int)pop();
					 TID[j]->set_val_r(d);
					 push(d);add(5);}
				   else if(k==3 && i==3){
					rez=(int)pop();
					   j=(int)pop();
					TID[j]->set_val_i(rez);
					push((double)rez);add(3);
				   }
				   break;


                    case 1:     i=get();j=get();//==
								 if(i==3&&j==3){
									rez=(int)pop();
									push((double)((int)pop()==rez));
									}else    if(i==5 && j==5){
															d=pop();
															push((double)(pop()==d));
									}else    if(i==3 && j==5){
															i=(int)pop();
															push((double)(pop()==i));
									}else    if(i==5 && j==3){
										d=pop();
										push((double)((int)pop()==d));
										} add(3);break;
                    case 2: i=get();j=get();//!=
											 if(i==3&&j==3){
							rez=(int)pop();
							push((double)((int)pop()!=rez));
							}else    if(i==5 && j==5){
													d=pop();
													push((double)(pop()!=d));
							}else    if(i==3 && j==5){
													i=(int)pop();
													push((double)(pop()!=i));
							}else    if(i==5 && j==3){
													d=pop();
													push((double)((int)pop()!=d));
							} add(3);
														  break;
                    case 3: i=get();j=get();//>
											 if(i==3&&j==3){
							rez=(int)pop();
							push((double)((int)pop()>rez));
							}else    if(i==5 && j==5){
													d=pop();
													push((double)(pop()>d));
							}else    if(i==3 && j==5){
													i=(int)pop();
													push((double)(pop()>i));
							}else    if(i==5 && j==3){
							d=pop();
							push((double)((int)pop()>d));
							}add(3);break;


                    case 4:     i=get();j=get();//<
								if(i==3&&j==3){
								rez=(int)pop();
								push((double)((int)pop()<rez));
								}else    if(i==5 && j==5){
														d=pop();
														push((double)(pop()<d));
								}else    if(i==3 && j==5){
														i=(int)pop();
														push((double)(pop()<i));
								}else    if(i==5 && j==3){
														d=pop();
														push((double)((int)pop()<d));
								} add(3);break;
                    case 5:     i=get();j=get();//>=
												 if(i==3&&j==3){
								rez=(int)pop();
								push((double)((int)pop()>=rez));
								}else    if(i==5 && j==5){
														d=pop();
														push((double)(pop()>=d));
								}else    if(i==3 && j==5){
														i=(int)pop();
														push((double)(pop()>=i));
								}else    if(i==5 && j==3){
														d=pop();
														push((double)((int)pop()>=d));
								} add(3);break;
                    case 6:     i=get();j=get();//<=
												 if(i==3&&j==3){
								rez=(int)pop();
								push((double)((int)pop()<=rez));
								}else    if(i==5 && j==5){
														d=pop();
														push((double)(pop()<=d));
								}else    if(i==3 && j==5){
														i=(int)pop();
														push((double)(pop()<=i));
								}else    if(i==5 && j==3){
														d=pop();
														push((double)((int)pop()<=d));
								} add(3);  break;


							case 7:     i=get();j=get();//+
	    											 if(i==3&&j==3){
								rez=(int)pop();
								push((double)((int)pop()+rez));
								add(3);
								}else    if(i==5 && j==5){
														d=pop();
														push((double)(pop()+d));
												add(5);
								}else    if(i==3 && j==5){
														i=(int)pop();
														push((double)(pop()+i));
												add(5);
								}else    if(i==5 && j==3){
														d=pop();
														push((double)((int)pop()+d));
												add(5);
								}
								break;
                    case 8:     i=get();j=get();//-
	    						if(i==3&&j==3){
								rez=(int)pop();
								push((double)((int)pop()-rez));
								add(3);
								}else    if(i==5 && j==5){
														d=pop();
														push((double)(pop()-d));
												add(5);
								}else    if(i==3 && j==5){
														i=(int)pop();
														push((double)(pop()-i));
												add(5);
								}else    if(i==5 && j==3){
														d=pop();
														push((double)((int)pop()-d));
												add(5);
								}

															  break;
                    case 9:        i=get();j=get();//*
	    						if(i==3&&j==3){
								rez=(int)pop();
								push((double)((int)pop()*rez));
								add(3);
								}else    if(i==5 && j==5){
														d=pop();
														push((double)(pop()*d));
												add(5);
								}else    if(i==3 && j==5){
														i=(int)pop();
														push((double)(pop()*i));
												add(5);
								}else    if(i==5 && j==3){
														d=pop();
														push((double)((int)pop()*d));
												add(5);
								}
								break;
                    case 10:     i=get();j=get();//'/'
	    						if(i==3&&j==3){
								rez=(int)pop();
								push((double)((int)pop()/rez));
								add(3);
								}else    if(i==5 && j==5){
														d=pop();
														push((double)(pop()/d));
												add(5);
								}else    if(i==3 && j==5){
														i=(int)pop();
														push((double)(pop()/i));
												add(5);
								}else    if(i==5 && j==3){
														d=pop();
														push((double)((int)pop()/d));
												add(5);
								}
								break;
                    
                    case 17:get();get();	i=(int)pop();
								j=(int)pop();
								rez=j%i;
								push((double)rez);
								add(3);












                   break;
}
}
else if (elem.get_gr()==3){ i=(*TNUM_INT[elem.get_val()]);push((double)i);add(3);}
else if (elem.get_gr()==4){ type=TID[elem.get_val()]->get_type();
if(!strcmp(type,"int")){
i=TID[elem.get_val()]->get_val_i();push((double)i);add(3);}
else if(!strcmp(type,"real")){
d=TID[elem.get_val()]->get_val_r();push(d);add(5);}
}
     
else if (elem.get_gr()==5) { 
d=*TNUM_REAL[elem.get_val()];push(d);add(5);}
else if (elem.get_gr()==6) {i=elem.get_val();type=TID[i]->get_type();push((double)i);
if(!strcmp(type,"int"))add(3);else add(5); }
else if (elem.get_gr()==7){i=elem.get_val();push((double)i);}

 index++;
}
if(cont){throw "error in continue";}
//print();
//print1();


cout<<"Executer finish!!!!'\n'";
}

/////////////////////////////////////////////////////
//главная функция!!!


int main(){
	char input[20];
	char output[20];
    cout<<"vvedite file input:'\n'";
	cin>>input;
	cout<<"vvedite file output:'\n'";
	cin>>output;
		lex_analyze L(input, output);
	syntax_analyze S(output);
	Executer E;
		
		try{
	L.analizator();
	S.syn_an();
//	S.prog.print();
	E.go(S.prog);
		}
		catch(lex ){cout<<'\n';}
		catch(char){cout<<"nepravilniy simvol'\n'";}
		catch(char *k){cout<<k<<'\n';}
		
return 0;
		
}
