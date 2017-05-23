#include <exception>
#include <string.h> 
class myLetter:public exception{
public:
	const char* what()const throw();
};
class myNull:public exception{
public:
	const char* what()const throw();
};
