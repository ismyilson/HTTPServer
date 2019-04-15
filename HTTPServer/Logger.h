#pragma once

#include <string>

class Logger
{
public:
	Logger();
	~Logger();

	// Log a message
	static void Log(std::string);

	// Log an error message
	static void LogError(std::string);

	// Log an alert message
	static void LogAlert(std::string);
};

#define	LOG(x);	Logger::Log(x);
#define LOG_ERROR(x)	Logger::LogError(x);
#define LOG_ALERT(x)	Logger::LogAlert(x);