#ifndef INC_HIP_HOP_LOGGER_H
#define INC_HIP_HOP_LOGGER_H

//C++ headers
#include <cstdio>
#include <cstdarg>
#include <string>
#include "Singleton.h"

class Logger : public Singleton<Logger>
{
public:
	~Logger();
	Logger();
	void Init(const char* header);
	void LogToConsole(const char* pFormat,...);
	void LogToFile(const char* pFormat,...);
	void Log(const char*pFormat,...);
	void Log(const std::string& logString);

private:
	FILE* m_File;
};

#define LOG(format,data) Logger::GetInstance().Log(format,data)

#endif