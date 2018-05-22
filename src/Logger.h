#pragma once
#include "Singleton.h"	
#include <cstdio>
#include <cstdarg>
class Logger final : public Singleton<Logger>
{
public:
	static void Error(std::string msg) { std::cout<<"Error:"<<msg<<std::endl; }
	static void Warning(std::string msg) { std::cout<<"Warning:"<<msg<<std::endl; }
	static void Print(const char *format, ...) {

		va_list ap;

		va_start(ap, format);

		vprintf(format, ap);

		va_end(ap);
		
	}

	static void XXXPrint(int level,std::string log,std::string x) {
		std::cout << "LogPrint:" << log << std::endl;
	}

public:
	Logger() {}
	~Logger(){};
	
};

#define LOG_PRINT(format,...) Logger::GetInstance()->Print(format,__VA_ARGS__)

#define LOG_ERROR(str) Logger::GetInstance()->Error(str)

#define LOG_WARN(str) Logger::GetInstance()->Warning(str)

enum LogLevel : byte
{
	Info,
	Warning,
	Error,
	Debug
};

// #define LOG(...) \
// 	Logger::XXXPrint( \
// 		__VA_ARGS__) 

static const std::string STARENGINE_LOG_TAG = "STARENGINE";
