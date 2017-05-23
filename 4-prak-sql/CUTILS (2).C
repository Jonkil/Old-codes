/* Error will only send the message */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "sql.h"
#include "server.h"

extern int SSend(char*);

int Error(char *m) {  /* Outputs an error */
	printf("%s\n\r",m);
	return -1;
}

int CError(char *m) { /* ?????????????????  Client error, maybe */
	printf("%s\n\r",m);
	return -1;
}

void charncpy(char *s,char *d,int n) { /* copies the chars but less then n */
int i=0;
	while(i<n && s[i])
		d[i]=s[i++];
	if(i>=strlen(s))
		d[i]=0;
}

char *ToUpper(char *a) { /* Converts the string to the upper case except "" */
int i;
	for (i=0;i<strlen(a);i++) {
		if(a[i]=='\"') {
			printf(" String was found\n");
			i++;
			while(a[i] && a[i]!='\"')
				i++;
			}
		if(a[i]=='\'') {
			i++;
			while(a[i] && a[i]!='\'')
				i++;
			}
		if(!a[i]) {
			Error("User error : can't locate the string");
			return NULL;
			}
		if(islower(a[i]))
			a[i]=toupper(a[i]);
		}
	return a;
}


char* LToA(long l) { /* Converts the LONG to the string */
char *tmp;
int i=0;
	tmp=(char*)calloc(Size("Long"),1);
	while(l) {
		tmp[-1+Size("Long")-i++]=l%10+'0';
		l/=10;
		}
	return tmp;
}


void ClrScr(void) { /* Clears the screen on UNIX systems */
int i;
	for(i=0;i<25;i++)
		printf("\n");
}


int WildCmp(char *w,char *s) { /* Compares the strings with wildcards */
int i=0,j=0;
int Found=0;
int t,NotFlag=NO;
	for(;i<strlen(w);i++) {
		switch(w[i]) {
		case '*' :
			if(!strlen(s)) {
				for(t=0;t<strlen(w);t++)
					if(w[t]!='*')
						return NO;
				return YES;
				};
			return (WildCmp(w+i+1,s+i) || WildCmp(w+i,s+i+1));
			/*break;*/
		case '!' :
			if(j<strlen(s))
				j++;
			else
				return NO;
			break;
		case '[' :
			if(j>=strlen(s))
				return NO;
			if(w[i++]==']')
				return NO;
			if (w[i]=='^') {
				NotFlag=YES;
				i++;
				};
			if(w[i]==']' || w[i]=='-')
				Error(" Missing ] in LIKE ");
			t=w[i++];
			if(w[i]=='-') {/* range */
				if(s[j]>=t && s[j]<=w[i+1])
					Found=YES;
				}
			else {
				if(s[j]==t)
					Found=YES;
				else {
					while(w[i]!=']')
						if(w[i++]==s[j])
							Found=YES;
					}
				}
			if(NotFlag)
				Found=!Found;
			return Found;
		default	  :
			if(j>=strlen(s))
				return NO;
			if(w[i]=='\\')
				i++;
			if(s[j]==w[i])
				j++;
			else
				return NO;
			break;
		}
	}
	if(!s[j])
		return YES;

	return NO;
}


int Size(char *t) { /* Returns the size of any possible in DB types */
int c,s=0;
	t=ToUpper(t);
	if(!strcmp(t,"STRING")) return 20;
	if(!strcmp(t,"LONG")) return 8;
	if(strstr(t,"STRING")!=NULL) {
		c=strlen("STRING")+1;
		while(t[c]!=']' && c<=strlen(t))
			s=s*10+t[c++]-'0';
		if(c>strlen(t))
			if(Error("Syntax error : check the braces in type."))
				return -1;
		return s;
		}
	return -1;
}

char *AlignLong(char *s1) {
int i;
char *s;
	s=(char*)calloc(Size("LONG")+1,1);
	for(i=0;i<strlen(s1);i++)
		s[Size("LONG")-strlen(s1)+i]=s1[i];
	for(i=0;i<Size("LONG")-strlen(s1);i++)
		s[i]='0';
	return s;
}

int IsType(char *s) { /* Checks that the S is an type name  */
int i=1;
char *t;
	if((t=strstr(s,"STRING"))!=NULL) {
		t+=strlen("STRING");
		if(!(*t))
			return YES;
		if(*t!='[') {
			Error("User error : STRING length not specified");
			return -1;
			}
		t++;
		while(*t && *t!=']') {
			if(!isdigit(*t)) {
				Error("User error : STRING length has inallowed type");
				return -1;
				};
			if(i>3) {
				Error("User error : STRING length is too long");
				return -1;
				};
			t++;
			i++;
			}
		if(*t!=']') {
			Error("User error : can't find the ]");
			return -1;
			}
		return YES;
		}
	if(!strcmp(s,"LONG"))
		return YES;
	return NO;
}

int IsIdent(char *s) { /* Chaecks that the S is an identifier */
int i;
	for(i=0;i<strlen(s);i++)
		if(!(isdigit(s[i]) || isalpha(s[i]) || s[i]=='_'))
			return NO;
	return YES;
}

int IsTerm(char c) {
	if(c==':' || c=='+' || c=='<' || c=='>' ||\
	   c=='=' || c=='*' || c=='%' || c=='/' ||\
	   c==')' || c=='(' || c==',')
		return YES;
	return NO;
}

int IsNumber(char *s) {
int i;
	for(i=0;i<strlen(s);i++)
		if(!isdigit(s[i]))
			return NO;
	return YES;
}

int IsString(char *s) {
	if(s[0]=='\"' && s[strlen(s)-1]=='\"' ||\
		s[0]=='\'' && s[strlen(s)-1]=='\'')
		return YES;
	return NO;
}

char *GetNextLex(char *s1) { /* gets the next lexem from the s */
int t1=0,t2=0;
char *t,*s=s1+Curr;
	if(!(*s))
		return NULL;
	s1+=Curr;
	while(*s==' ')
		t1++,s++;
	t2=t1;
	if (IsTerm(*s))
		t2=t1+1,s++;
	else {
		while(*s!=' ' && *s && !IsTerm(*s))
			t2++,s++;
		}
	t=(char*)calloc(t2-t1+1,1);
	strncpy(t,s-t2+t1,t2-t1);
	Curr+=s-s1;
	return t;
}

int GetLex(char *s,char *s1) { /* Get the lexem specified by S from the Source string */
char *c;
int i;
	if ((c=strstr(s1,s))!=NULL) {
		for(i=c-s1;i<strlen(s)+c-s1;i++)
			s1[i]=0;
		return c-s1;
		}
	return 0;
}

char *Trim(char *s1) {
int t1=0,t2=0;
char *s=s1;
	while(*s==' ')
		t1++,s++;
	t2=t1;
	while(*s!=' ' && *s)
		t2++,s++;
	s1[t2]=0;
	return(s1+t1);
}
