#include <iostream>
#include <ctype.h>
#include <fstream>
using namespace std;

#define SIZE 100
#define TID_SIZE 150
#define TNUM_SIZE 150



enum del_type {LEX_ENDP=0, LEX_ASSIGN, LEX_EQ, LEX_NEQ, LEX_GT, LEX_LT=5, LEX_GE, LEX_LE, LEX_PLUS, LEX_MINUS,
	       LEX_AST=10, LEX_SLASH, LEX_BSLASH, LEX_COM, LEX_POINT, LEX_SEMCOL=15, LEX_COL, LEX_LBR, LEX_RBR, LEX_BEGIN,
		 LEX_END=20,LEX_NULL};



class identificator{
	char* name;
	bool declare;
	bool assign;
	int value;
	public:
	identificator(const char* s) 
		{
		name=new char[strlen(s)+1];
		strcpy(name, s);
		declare=false;
		assign=false;
		}
	~identificator() { if (name) delete[] name; }
	void set_name(const char* n) { name=new char[strlen(n)+1]; strcpy(name, n); }
	char *get_name() { return name; }
	int get_val() { return value; }
	void set_val(int a) { value=a; }
	void set_decl() { declare=true; }
	bool get_decl() { return declare; }
	void set_as() { assign=true; }
	bool get_as() { return assign; }
};
					
			class lex {
					int group;
					int value;
			public:
				lex (int g=-1, int v=-1): group(g), value(v) { }

				friend ostream &operator<<(ostream& s, lex cur_lex)
                      {
                        s << '(' << cur_lex.group << ',' << cur_lex.value << ") ";
                        return s;
                      }
				lex operator=(lex cur_lex)
					{
					this->group=cur_lex.group;
					this->value=cur_lex.value;
					return *this;
					}
	
				int get_val() { return value;}
				int get_gr() { return group; }
	
			};
	char* TW[]={"and","else","for", "if", "int", "not", "or","real","continue","program", "read","while", "write", NULL};
	char* TD[]={"=", "==","!=", ">", "<", ">=", "<=", "+", "-", "*", "/", "\\", ",", ".", ";", ":","(", ")", "{", "}","%",NULL};
	identificator* TID[TID_SIZE];
	int* TNUM_INT[TNUM_SIZE];
	double* TNUM_REAL[TNUM_SIZE];
	int j,n;
	char c, pred;
	int buf_top=0;
	char buf[SIZE];


class lex_analyze{
	enum state {H, ID, NUM, FIN, DELIM, COM};
	state CS;
	ifstream inp;
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

int putnum_int()
{ int i;
  for (i=0; i<150; i++)
	if (TNUM_INT[i]==NULL)  { TNUM_INT[i]=new int(atoi(buf)); return i; }
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
	if (TNUM_REAL[i]==NULL)  { TNUM_REAL[i]=new double(atof(buf)); return i; }
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
				lex_analyze(const char *in, const char *out) 
					{
					inp.open(in);
					outp.open(out, ios::out|ios::trunc);
					CS=H;
					clear();
					 }
				~lex_analyze() { inp.close(); free(); }
				
				void get_c() { pred=c; inp.get(c); }
				void put_c() { outp.put(c);}

				
void analizator(){
get_c();
		CS=H;
		 do
		 {
		   switch(CS)
			{
			case H: if (c==' ' || c=='\t' || c=='\n'||c=='\r') get_c();
				else
				if (isalpha(c))
				{
				  clear();
				  add();
				  get_c();
				  CS=ID; 
				}
				else if (isdigit(c))
				{
				  add();
				  get_c();
				  CS=NUM;
				}
				else if (c=='-' || c=='+')
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
						 outp<< lex(2,j); get_c(); CS=H; clear();
						}
				
				}
				else if (c=='/')
				{
				  get_c();
				  if (c=='*')  { CS=COM; get_c(); }
					else { CS=H; outp << lex(2, LEX_SLASH); }
				}
				else if(c=='@'){ CS=FIN;}
                else CS=DELIM;
				break;

			case ID: if (isalpha(c) || isdigit(c) || c=='_')
				{
				 add();
				 get_c();
				}
				else if ((j=look_for(TW))!=-1) 
					{ outp << lex(1,j); clear(); CS=H;}
				else if ((j=look_id(TID))==-1) 
					{ j=put_id(TID); outp << lex(4,j); clear(); CS=H;}
				else  { outp<< lex(4,j); CS=H; clear(); }
				break;

			/*case NUM: 
				if (isdigit(c))
				{
				add(); getc();
				}
				else if ((j=looknum(TNUM))!=-1)  { out << lex(3,j); CS=H; }
					else { j=putnum(); out << lex(3,j); CS=H; }
				break;
			*/	
			case DELIM: clear(); 
				if (c=='=')
					{
					get_c();
					if (c=='=') { outp << lex(2,LEX_EQ); get_c(); CS=H; }
					else { outp << lex(2, LEX_ASSIGN); CS=H; }
					}
				  else if (c=='!')
					{
					get_c(); if (c=='=') {outp << lex(2, LEX_NEQ); get_c(); CS=H; }
							else throw c;
					}
				  else if (c=='>')
					{
					get_c();
					if (c=='=') {outp<< lex(2, LEX_GE); get_c(); CS=H; }
						else  {outp<< lex(2, LEX_GT); CS=H; }
					}
				  else if (c=='<')
					{
					get_c();
					if (c=='=') { outp<< lex(2, LEX_LE); get_c(); CS=H; }
						else  {outp<< lex(2, LEX_LT); CS=H; }
					}
				  else if (c=='{')
					{
					 { outp << lex(2, LEX_BEGIN); get_c(); CS=H; }
					}
				  else if (c=='}')
					{
					 { outp << lex(2, LEX_END); get_c(); CS=H; }
					}
				  else if(c=='@'){CS=FIN;}
				  else 
					{
					 add();
					 j=look_for(TD); if (j==-1) throw "takogo znaka net v tablice!!";
					 outp << lex(2,j); get_c(); CS=H;
					}
				   break;
			case COM: if (c=='*') 
				  {
				   get_c();
				   if (c=='/') { get_c(); CS=H; }
				  }
				 else if (c=='@') throw c;
				 else get_c();
					break;
			
 		} while((CS!=FIN)and(!inp.eof));
		outp.close;
		return;
	}
};



//main!!!!!!!!!!!!
/*int main(){
	char in[20];
	char out[20];
    cout<<"vvedite file input:'\n'";
	cin>>in;
	cout<<"vvedite file output:'\n'";
	cin>>out;
		lex_analyze L(in, out);
		try{
			L.analizator;
		}
		catch(char c){cerr<<"wto-to ne tak!!!'\n'";}
		catch(char*){cerr<<"wto-to ne tak!!!'\n'";}
        
		
}
*/                    