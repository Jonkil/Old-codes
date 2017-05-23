#include <exception>
#include <string.h>

class myDigit:public exception{
public:
	const char* what()const throw();
};
class myNull:public exception{
public:
	const char* what()const throw();
};
