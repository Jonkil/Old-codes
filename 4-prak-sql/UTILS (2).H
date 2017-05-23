#define TRUE  1
#define FALSE 0
#define YES TRUE
#define NO FALSE
#define ERROR -1
#define MAX_TYPE_NAME_LENGTH 20
#define MAX_FIELD_NAME_LENGTH 20
#define MAX_MSG_LENGTH (255*3)

extern void ClrScr(void);
extern void charncpy(char *,char *,int);
extern char *ToUpper(char *);
extern char *LToA(long);
extern int WildCmp(char *,char*);
extern int Size(char *);
extern int Error(char*);
extern int CError(char*);
extern char *AlignLong(char*);
extern int IsType(char*);
extern int IsIdent(char*);
extern int IsTerm(char);
extern int IsNumber(char*);
extern int IsString(char*);
extern char* GetNextLex(char*);
extern int GetLex(char*,char*);
extern char *Trim(char*);
/*
extern char* strstr _PARAMS((const char*,const char*));
*/

