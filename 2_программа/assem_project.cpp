#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fstream>
 
enum type_of_lex
{
  LEX_NULL,
  LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT,
  LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE,
  LEX_FIN,
  LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,
  LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,
  LEX_NUM,
  LEX_ID, 
 }; 
template T <int,double>;
 

 
class Lex
{
  int t_lex;
  int v_lex;
public:
                      Lex (int t = -1, int v = -1): t_lex (t), v_lex (v) {}
         int  get_type ()  { return t_lex; }
         int  get_value () { return v_lex; }
  friend ostream &    operator<< (ostream &s, Lex l)
                      {
                        s << '(' << l.t_lex << ',' << l.v_lex << ");" ;
                        return s;
                      }
};
 
 
class Ident
{
         char       * name;
         bool         declare;
         int		type;
         bool         assign;
         char*         value;
public:
                      Ident() { declare = false; assign = false; }
         char       * get_name () { return name; }
         void         put_name (const char *n)
                      {
                        name = new char [ strlen(n)+1];
                        strcpy(name,n);
                      }
         bool         get_declare () { return declare; }
         void         put_declare () { declare = true; }
         int          get_type    () { return type; }
         void         put_type    (int t) { type = t; }
         bool         get_assign  () { return assign; }
         void         put_assign  (){ assign = true; }
         char*        get_value   () { return value; }
         char*  put_value   (char* v){ value=new char[strlen(v)+1];
						strcpy(value,v)}
};
 

 
class Scanner
{
         enum         state { H, IDENT, NUMB, COM, ALE, DELIM, NEQ };
  static char       * TW    [];
  static type_of_lex  words [];
  static char       * TD    [];
  static type_of_lex  dlms  [];
         state        CS;
         FILE       * fp;
         char         c;
         char         buf [ 80 ];
         int          buf_top;
         void         clear ()
                      {
                        buf_top = 0;
                        for ( int j = 0; j < 80; j++ )
                          buf[j] = '\0';
                      }
         void         add ()
                      {
                        buf [ buf_top++ ] = c;
                      }
         int          look ( const char *buf, char **list )
                      {
                        int i = 0;
                        while (list[i])
                        {
                          if ( !strcmp(buf, list[i]) )
                            return i;
                          ++i;
                        }
                        return 0;
                      }
         void         gc ()
                      {
                        c = fgetc (fp);
                      }
public:
                      Scanner ( const char * program )
                      {
                        fp = fopen ( program, "r" );
                        CS = H;
                        clear();
                        gc();
                      }
                 void scan(char *input, const char *output);
};
 
char *
Scanner::TW    [] = {"and", "real", "break", "else",
	    "for", "if", "int", "not", "or",
	   "program", "read","while", "write", NULL};
 
type_of_lex
Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT,
                     LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_NULL};
 
char *
Scanner::TD    [] = {"@", "=", "==","!=", ">", "<", ">=", "<=", "+", "-", "*",%, "/", "\\", ",", ".", ";", ":","(", ")", "{", "}", NULL};
 
type_of_lex
Scanner::dlms  [] = {LEX_NULL, LEX_FIN, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ,
                     LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_NULL};
 
Lex Scanner::scan(const char *input, const char *output) 
		{
		in.open(input);
		out.open(output, ios::out|ios::trunc);
		CS=H;
		clear();
		 }
	~scanner() { in.close(); freeall(); }
	
	void getc() { prev=c; in.get(c); }
	void putc() { out.put(c);}	

	void scan
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
					 if (prev=='=' || prev=='>' || prev=='<' || prev=='(' 
						|| prev=='+' || prev=='-' || prev=='*' || prev=='/' || isspace(prev)||prev=='%')
						{
						 add();
						 CS=NUM;
						 getc();
						}
					 else 
						{
						 add();
						  j=look(TD); if (j==-1) throw "no!!!"
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
					{ j=putid(TID); out << lex(4,j); clear(); CS=H;}
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
					 j=look(TD); if (j==-1) throw "something strange happened";
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
			} 
 		} while( !in.eof() );
	out.close();
	return;
	} 
};
int main(){
char file_in[20];
char file_out[20];
cin>>file_in;
cin>>file_out;
try{
	scan(file_in,file_out);
}
catch(...){cout<<"bad!'\n'<<endln; return 1;}

return 0;
}