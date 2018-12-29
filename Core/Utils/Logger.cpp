//Declaration header
#include"Logger.h"
#include<cstdio>

namespace HipHop
{
	Logger::Logger(): m_File(nullptr)
	{

	}


	Logger::~Logger()
	{
		fprintf(m_File,"---END--");
		fclose(m_File);
	}


	void Logger::Init(const char* pHeader)
	{
		m_File = fopen("log.txt","w");
		if (!m_File)
		{
			printf("Failed to create a log m_File");
			return;
		}

		fprintf(m_File,pHeader);
		fprintf(m_File,"\n");
	}


	void Logger::LogToFile(const char* pFormat,...)
	{
		va_list list;
		va_start(list,pFormat);
		vfprintf(m_File,pFormat,list);
		fprintf(m_File, "\n");
		va_end(list);
	}

	void Logger::Log(const char * pFormat, ...)
	{
		LogToConsole(pFormat);
		LogToFile(pFormat);
	}

	void Logger::Log(const std::string & logString)
	{
		LogToConsole(logString.c_str());
		LogToFile(logString.c_str());
	}


	void Logger::LogToConsole(const char* pFormat, ...)
	{
		fprintf(stderr,pFormat);
	}
}



