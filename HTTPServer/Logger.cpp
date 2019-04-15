#include "Logger.h"

#include <iostream>
#include <ctime>

Logger::Logger()
{

}

Logger::~Logger()
{

}

void Logger::Log(std::string msg)
{
	std::cout << msg << std::endl;
}

void Logger::LogError(std::string msg)
{
	std::cout << "Error: " << msg << std::endl;
}

void Logger::LogAlert(std::string msg)
{
	std::cout << "Alert: " << msg << std::endl;
}