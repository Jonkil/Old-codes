#include <stdio.h>
#include "utils.h"

enum Whence { BEGIN=0, CURRENT, END };

struct Fields {
	char    	Type[MAX_TYPE_NAME_LENGTH];	/* Type of the field */
	char 		Name[MAX_FIELD_NAME_LENGTH];	/* Name of the field */
};

struct DB {
	struct Fields 	*Structure;	/* Type & name vector    */
	int 	Number;         /* Number of Fields              */
	char 	*Name;		/* Name of the base (file)       */
	int 	FullSize;	/* Full Size of the record       */
	FILE 	*Source;	/* FILE descriptor               */
	char	*WorkBuf;	/* Working buffer                */
	int	Curr;           /* Current position              */
	int Modifiable;		/* You can add some fields       */
	int 	Psp;		/* Beginning of the first record */
	};

enum GetTypes { BOOL, LONG, STRING };

extern struct DB* OpenDB(char*,int);
extern int AddFields(struct DB*,int,struct Fields*);
extern void CloseDB(struct DB*);
extern int WriteToFile(struct DB*,enum Whence,int);
extern int ReadFromFile(struct DB*,enum Whence,int);
extern char *GetField(struct DB*,char*);
extern void PutField(struct DB*,char*,char*);
extern void PrintWorkBuf(struct DB*);
extern void DeleteDB(char*);
/*extern enum GetType GetDBType(char*,struct DB*);*/
extern char* GetValue(char*,struct DB*);
extern struct DB* GetDB(char*);
extern char* Format(struct DB*,char*);
extern void ClearWorkBuf(struct DB*);
extern int IsField(char*,struct DB*);
/*
extern char* strstr(char*,char*);
*/
