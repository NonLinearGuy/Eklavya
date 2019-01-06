#ifndef INC_HIP_HOP_LOGGER_H
#define INC_HIP_HOP_LOGGER_H

//C++ headers
#include <cstdio>
#include <cstdarg>
#include <string>

	class Logger
	{
	public:
		~Logger();
		inline static Logger* GetInstance()
		{
			static Logger instance;
			return &instance;
		}
		void Init(const char* header);
		void LogToConsole(const char* pFormat,...);
		void LogToFile(const char* pFormat,...);
		void Log(const char*pFormat,...);
		void Log(const std::string& logString);

	private:
		FILE* m_File;
		Logger();
	};

#endif