#pragma once
#include "Singleton.h"	
#include <cstdio>
#include <cstdarg>
class Logger final : public Singleton<Logger>
{
public:
	static void Error(std::string msg) { std::cout<<"Error:"<<msg<<std::endl; }
	static void Warning(std::string msg) { std::cout<<"Warning:"<<msg<<std::endl; }
	static void Print(char *format, ...) {

		va_list ap;

		va_start(ap, format);

		int n = vprintf(format, ap);

		va_end(ap);


		
	}
private:
	Logger() : Singleton<Logger>() {}
	~Logger();
	
};