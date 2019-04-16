#include "FileHandler.h"

#include "Logger.h"

FileHandler::FileHandler()
{

}

FileHandler::FileHandler(std::string path)
{
	OpenFile(path);
}

FileHandler::~FileHandler()
{
	if (FileIsOk())
		CurrentFile.close();

	LOG("File closed");
}

bool FileHandler::OpenFile(std::string path)
{
	CurrentFile = std::fstream(path);
	if (!FileIsOk())
	{
		LOG_ERROR("Could not open file '" + path + "'");
		return false;
	}

	return true;
}

bool FileHandler::NextLine(std::string &line)
{
	if (CurrentFile.eof())
		return false;

	std::getline(CurrentFile, line);
	return true;
}

bool FileHandler::FileIsOk()
{
	if (!CurrentFile || !CurrentFile.good() || !CurrentFile.is_open())
	{
		return false;
	}

	return true;
}