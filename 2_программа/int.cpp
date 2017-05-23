#include <iostream>
#include <ctype.h>
#include <fstream>

#define SIZE 80
#define TID_SIZE 128
#define TNUM_SIZE 128

using namespace std;

// GROUP NUMBER 1
enum lex_type {LEX_AND=0, LEX_BOOL, LEX_BREAK, LEX_ELSE, LEX_FALSE, LEX_FOR=5, LEX_IF,
	       LEX_INT, LEX_NOT, LEX_OR, LEX_PROG=10, LEX_READ, LEX_TRUE, LEX_WHILE, LEX_WRITE=14};

//GROUP NUMBER 2
enum del_type {LEX_ENDP=0, LEX_ASSIGN, LEX_EQ, LEX_NEQ, LEX_GT, LEX_LT=5, LEX_GE, LEX_LE, LEX_PLUS, LEX_MINUS,
	       LEX_AST=10, LEX_SLASH, LEX_BSLASH, LEX_COM, LEX_POINT, LEX_SEMCOL=15, LEX_COL, LEX_LBR, LEX_RBR, LEX_BEGIN,
		 LEX_END=20,LEX_NULL};

class ident {
	char* name;
	bool declared;
	bool assigned;
	int value;
public:
	ident(const char* s) 
		{
		name=new char[strlen(s)+1];
		strcpy(name, s);
		declared=false;
		assigned=false;
		}
	~ident() { if (name) delete[] name; }
	void setname(const char* n) { name=new char[strlen(n)+1]; strcpy(name, n); }
	char *getname() { return name; }
	int getval() { return value; }
	void setval(int a) { value=a; }
	void set_decl() { declared=true; }
	bool get_decl() { return declared; }
	void set_assign() { assigned=true; }
	bool get_assign() { return assigned; }
};


char* TW[]={"and", "boolean", "break", "else", "false",
	    "for", "if", "int", "not", "or",
	   "program", "read", "true", "while", "write", NULL};
char* TD[]={"@", "=", "==","!=", ">", "<", ">=", "<=", "+", "-", "*", "/", "\\", ",", ".", ";", ":","(", ")", "{", "}", NULL};
ident* TID[TID_SIZE];
int* TNUM[TNUM_SIZE];
int j,n, cnt=0;
char c, prev;
int buf_top=0;
char buf[SIZE];


class lex {
	int group;
	int value;
public:
	lex (int g=-1, int v=-1): group(g), value(v) { }

	friend ostream &operator<<(ostream& str, lex lexx)
                      {
                        str << '(' << lexx.group << ',' << lexx.value << ") ";
                        return str;
                      }
	lex operator=(lex temp)
		{
		this->group=temp.group;
		this->value=temp.value;
		return *this;
		}
	
	int get_val() { return value;}
	int get_gr() { return group; }
	
};


//BEGINNING OF CLASS SCANNER
class scanner {
	enum state {H, ID, NUM, END, DEL, COM};
	state CS;
	ifstream in;
	ofstream out;

void clear()
{
 buf_top=0;
 memset(buf, '\0', SIZE);
}

void add()
{
 buf[buf_top++]=c;
}

int lookid(ident **t)
{ 
  int i=0;
  while (t[i]!=NULL)
	{
	 if (!strcmp(t[i]->getname(), buf)) return i;
	 i++;
	}
 return -1;
}

int putid(ident **t)
{ 
   for (int i=0; i<TID_SIZE; i++)
    if (t[i]==NULL) { t[i]=new ident(buf);  return i;}
return -1;
}

int look(char **t)
{ 
  int i=0;
  while (t[i]!=NULL)
	{
	 if (!strcmp(t[i], buf)) return i;
	 i++;
	}
 return -1;
}

int put(char **t)
{ 
   for (int i=0; i<TID_SIZE; i++)
    if (t[i]==NULL) { t[i]=new char[strlen(buf+1)]; strcpy(t[i], buf); return i;}
return -1;
}

int looknum(int **t)
{ int num=atoi(buf);
  int i=0;
  while (t[i]!=NULL)
	{
	 if (*(t[i])==num) return i;
	 i++;
	}
 return -1;
}

int putnum()
{ int i=0;
  for (; i<128; i++)
	if (TNUM[i]==NULL)  { TNUM[i]=new int(atoi(buf)); return i; }
}

void freeall()
{
 int i;
 for(i=0; i<TID_SIZE; i++)
	if (TID[i]!=NULL) {  delete TID[i]; }
		else break;
 
 for(i=0; i<TNUM_SIZE; i++)
	if(TNUM[i]!=NULL) { delete TNUM[i]; }
}

public:
	scanner(const char *input, const char *output) 
		{
		in.open(input);
		out.open(output, ios::out|ios::trunc);
		CS=H;
		clear();
		 }
	~scanner() { in.close(); freeall(); }
	
	void getc() { prev=c; in.get(c); }
	void putc() { out.put(c);}	

	void analyze()
	{	
		getc();
		CS=H;
		 do
		 {
		   switch(CS)
			{
			case H: if (c==' ' || c=='\t' || c=='\n') getc();
				else
				if (isalpha(c))
				{
				  clear();
				  add();
				  getc();
				  CS=ID; 
				}
				else if (isdigit(c))
				{
				  add();
				  getc();
				  CS=NUM;
				}
				else if (c=='-' || c=='+')
				{
				 clear();
					 if (pred=='=' || pred=='>' || pred=='<' || pred=='(' 
						|| pred=='+' || pred=='-' || pred=='*' || pred=='/' || pred=='%' || isspace(pred))
						{
						 add();
						 CS=NUM;
						 getc();
						}
					 else 
						{
						 add();
						  j=look_for(TD); if (j==-1) throw "error!!!";
						 out << lex(2,j); getc(); CS=H; clear();
						}
				
				}
				else if (c=='/')
				{
				  getc();
				  if (c=='*')  { CS=COM; getc(); }
					else { CS=H; out << lex(2, LEX_SLASH); }
				}
				else CS=DEL;
				break;

			case ID: if (isalpha(c) || isdigit(c) || c=='_')
				{
				 add();
				 getc();
				}
				else if ((j=look(TW))!=-1) 
					{ out << lex(1,j); clear(); CS=H;}
				else if ((j=lookid(TID))==-1) 
					{ j=put_id(TID); out << lex(4,j); clear(); CS=H;}
				else  { out << lex(4,j); CS=H; clear(); }
				break;

			case NUM: 
				if (isdigit(c))
				{
				add(); getc();
				}
				else if ((j=looknum(TNUM))!=-1)  { out << lex(3,j); CS=H; }
					else { j=putnum(); out << lex(3,j); CS=H; }
				break;
				
			case DEL: clear(); 
				if (c=='=')
					{
					getc();
					if (c=='=') { out << lex(2,LEX_EQ); getc(); CS=H; }
					else { out << lex(2, LEX_ASSIGN); CS=H; }
					}
				  else if (c=='!')
					{
					getc(); if (c=='=') {out << lex(2, LEX_NEQ); getc(); CS=H; }
							else throw c;
					}
				  else if (c=='>')
					{
					getc();
					if (c=='=') {out << lex(2, LEX_GE); getc(); CS=H; }
						else  {out << lex(2, LEX_GT); CS=H; }
					}
				  else if (c=='<')
					{
					getc();
					if (c=='=') { out << lex(2, LEX_LE); getc(); CS=H; }
						else  {out << lex(2, LEX_LT); CS=H; }
					}
				  else if (c=='{')
					{
					 { out << lex(2, LEX_BEGIN); getc(); CS=H; }
					}
				  else if (c=='}')
					{
					 { out << lex(2, LEX_END); getc(); CS=H; }
					}
				  else 
					{
					 add();
					 j=look(TD); if (j==-1) throw "error!!!";
					 out << lex(2,j); getc(); CS=H;
					}
				   break;
			case COM: if (c=='*') 
				  {
				   getc();
				   if (c=='/') { getc(); CS=H; }
				  }
				  else if (c=='@') throw c;
				   else getc();
					break;
					 
			case END: break;
			} //end of switch
 		} while( !in.eof||(CS!=FIN);
	out.close();

// 	for (int i=0; i<TID_SIZE; i++)
// 	  if (TID[i]) cerr << TID[i]->getname() << endl;
	return;
	} // end of analyze
}; 
//END OF CLASS SCANNER


//BEGINNING OF CLASS PARSER
class parser {
	lex curr_lex;
	lex next;
	bool if_peek;
	ifstream in;
	
	void P();
	void V2();
	void V();
	void O();
	void O1();
	void E();
	void E1();
	void T();
	void F();
	void OV();
	void SO();

public:
	parser(const char *input) 
		{
		if_peek=false;
		in.open(input);
		}
	~parser() { in.close(); }
 
	void gl()
		{
		 if (if_peek) { curr_lex=next; if_peek=false; return; }
		 if (in.eof()) return;
		 int g,v;
		 in.get(c);
		 in.get(c);
		 g=c-'0';
		 in.get(c);
		 in.get(c);
		 v=c-'0';
		 in.get(c);
		 if (isdigit(c)) { v*=10; v+=c-'0'; in.get(c); }
		 curr_lex=lex(g,v);
		 in.get(c);
		 cnt++;
		}

	void peek()
		{
		 if_peek=true;
		 if (in.eof()) return;
		 int g,v;
		 in.get(c);
		 in.get(c);
		 g=c-'0';
		 in.get(c);
		 in.get(c);
		 v=c-'0';
		 in.get(c);
		 if (isdigit(c)) { v*=10; v+=c-'0'; in.get(c); }
		 next=lex(g,v);
		 in.get(c);
		}
	
	bool eq(int g, int v)
		{
		 return (curr_lex.get_val()==v && curr_lex.get_gr()==g);
		}

	void go()
		{
		 gl();
		 P();
		 cout << "last lex number " << cnt  << " " << curr_lex << endl;
		 cout << "SUCCES!!! YCnEX!!! /7O6EgA!!!" << endl;
		}
};
//END OF CLASS PARSER


//BEGINNING OF PARSER FUNCTIONS
void parser::P()
	{
	  if (eq(1, LEX_PROG))
		{
		 gl();
		 if (eq(2, LEX_BEGIN))
			{
			 gl();
			 V2();
			 O1();
			 if (!eq(2, LEX_END)) {cerr << " v konce chego to ne to" << endl; throw curr_lex; }
			}
		}
	else	 { cerr << "P" << endl; throw curr_lex; }
		
	}

void parser::V2()
	{
	if (!eq(1, LEX_BOOL) && !eq(1, LEX_INT)) { gl(); return; }
	  while (eq(1, LEX_BOOL) || eq(1, LEX_INT))
		{
		  gl();
		  V();
		  while (eq(2, LEX_COM))
		  	{
			 gl();
			 V();
			}
		  if (!eq(2, LEX_SEMCOL)) { cerr << "V2" << endl; throw curr_lex; }
		  gl();
		}
	}

void parser::V()
	{
	 if (curr_lex.get_gr()==4)
		{
		  gl();
		  if (eq(2, LEX_ASSIGN)) { gl(); if (curr_lex.get_gr()==3 || eq(1, LEX_TRUE) || eq(1, LEX_FALSE))  gl(); return; }
		}
	else { cerr << "V" << endl; throw curr_lex; }
	}

void parser::O1()
	{
		while (!eq(2, LEX_END))
			O();
	}

void parser::O()
	{
		if (eq(1, LEX_IF))
			{
			 gl();
			 if (!eq(2, LEX_LBR)) { cerr << 1 << endl; throw curr_lex; }
			 gl();
			 E();
			 if(!eq(2, LEX_RBR)) { cerr <<cnt << "  " << 2 << endl; throw curr_lex; }
			 gl();
			 O();
			 if (eq(1, LEX_ELSE)) { gl(); O(); }
			}
		else if (eq(1, LEX_WHILE))
			{
			 gl();
			 if (!eq(2, LEX_LBR))  { cerr << 5 << endl; throw curr_lex; }
			 gl();
			 E();
			 if(!eq(2, LEX_RBR)) { cerr << 6 << endl; throw curr_lex; }
			 gl();
			 O();
			}
		else if (eq(1, LEX_WRITE))
			{
			 gl();
			 if (!eq(2, LEX_LBR))  { cerr << 7 << endl; throw curr_lex; }
			 gl();
			 E();
			 while (eq(2, LEX_COM))
				E();

			 if (!eq(2, LEX_RBR)) {  cerr << 9 << endl; throw curr_lex; }

			 gl();
			 if (!eq(2, LEX_SEMCOL)) { cerr << cnt << " "<< 71 << endl; throw curr_lex; }
			 gl();
			}
		else if (eq(1, LEX_READ))
			{
			 gl();
			 if (!eq(2, LEX_LBR))  { cerr << 10 << endl; throw curr_lex; }
			 gl();
			 if(!curr_lex.get_gr()==4) { cerr << 11 <<endl; throw curr_lex; }
			 gl();
			 if (!eq(2, LEX_RBR)) { cerr << 12 << endl; throw curr_lex; }

			 gl();
			 if (!eq(2, LEX_SEMCOL)) { cerr <<cnt << " " << 711 << endl; throw curr_lex; }
			 gl();
			}
		else if (eq(1, LEX_BREAK))
			{
			gl();
			if (!eq(2, LEX_SEMCOL)) {cerr << 111 << endl; throw curr_lex; }
			gl();
			}
		else if (eq(1, LEX_FOR))
			{
			 gl();
			 if (!eq(2, LEX_LBR))  { cerr << 13 << endl; throw curr_lex; }
			 
			 gl();
			 if (!eq(2, LEX_SEMCOL)) { E(); }
			 if (!eq(2, LEX_SEMCOL)) { cerr << cnt << " " << 14 << endl; throw curr_lex; }

			 gl();
			 if (!eq(2, LEX_SEMCOL)) { E(); }
			 if (!eq(2, LEX_SEMCOL)) { cerr << 15 << endl; throw curr_lex; }
			
			 gl();
			  if (!eq(2, LEX_RBR)) { E(); }
			 if (!eq(2, LEX_RBR)) { cerr << cnt <<" " << 16 << endl; throw curr_lex; }

			gl();
			O();
			}
		else if (eq(2, LEX_BEGIN)) 
			{
			 gl();
			 O1();
			 if (!eq(2, LEX_END)) { cerr << cnt << " "<< 23 << endl; throw curr_lex; }
	 		 gl();
			}
		
		else	if(curr_lex.get_gr() > 2) OV();
		else 	{ cerr <<cnt << " " << 666 << endl; throw curr_lex; }
}

 void parser::E()
	{
	if (curr_lex.get_gr()==4)
		{
		 peek();
		 if (next.get_val()==LEX_ASSIGN)
			{
			 gl();
			 gl();
			 E1();
			}
		 else
			{
			 E1();
			 if (eq(2, LEX_EQ) || eq(2, LEX_NEQ) || eq(2, LEX_GE) || eq(2, LEX_LE) || eq(2, LEX_GT) || eq(2, LEX_LT))
				{
				 gl();
				 E1();
				}
			}
		}
	else
		{
		 E1();
			 if (eq(2, LEX_EQ) || eq(2, LEX_NEQ) || eq(2, LEX_GE) || eq(2, LEX_LE) || eq(2, LEX_GT) || eq(2, LEX_LT))
				{
				 gl();
				 E1();
				}
		}
	}
	

void parser::E1()
	{
	 T();
	 while (eq(2, LEX_PLUS) || eq(2, LEX_MINUS) || eq(1, LEX_OR))
	   { gl(); T(); }

	}
void parser::T()
	{
	 F();
	  while (eq(2, LEX_AST) || eq(2, LEX_SLASH) || eq(1, LEX_AND))
	     { gl(); F(); }
	}

void parser::F()
	{
	if (eq(1, LEX_NOT))
		{
		 gl();
		 F();
		}
	else if (eq(2, LEX_LBR))
		{
		 gl();
		 E();
		 if (!eq(2, LEX_RBR))  { cerr << 22 << endl; throw curr_lex; }
		 gl();
		}
	else  if ((curr_lex.get_gr()>2) || eq(1, LEX_TRUE) || eq(1, LEX_FALSE))  gl();
	else { cerr << cnt  << " " << curr_lex << endl; cerr << 100 << endl; throw curr_lex; }
	}

void parser::OV()
	{
	 E();
	if (!eq(2, LEX_SEMCOL)) {cerr << "OV" <<endl; throw curr_lex; }
			 else gl();
	}
//END OF PARSER FUNCTIONS

int main(){
char input[20];
	char output[20];
    cout<<"vvedite file input:'\n'";
	cin>>input;
	cout<<"vvedite file output:'\n'";
	cin>>output;
	scanner A(input, output);
	parser B(output);
	try {
	A.analyze();
	B.go();
	}
	catch(char c) { cerr << "Syvmol gde to leviy pohodu" <<endl; }
	catch(const char* hehe) { cerr << hehe <<endl; }
	catch(lex gy) { cerr << "AAA vse propalo!!!!" << endl; cerr << gy.get_gr() << " " <<gy.get_val() << endl; }
return 0;
}
