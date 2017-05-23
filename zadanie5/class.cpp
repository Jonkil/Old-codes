#include <exception>
#include <string.h>
#include "zagolovok.h"
const char* myLetter::what()const throw(){
	return "NoLetterException";
}

const char* myNull::what()const throw(){
	return "NullException";
}