#define MAX_FIELDS_NUMBER 60
#define MAX_DB_NAME_LENGTH 8
#define MAX_DB_NUMBER 20
#define MAX_SOURCE_LENGTH 255

enum GetTypes;

extern int Curr,Free;
extern enum GetTypes GetType;
extern struct DB *DBMS[MAX_DB_NUMBER];
/*
extern int Create(char*);
extern int Where(char*,struct DB*);
extern int Insert(char*,struct DB*);
extern int SQL(char*);
*/
