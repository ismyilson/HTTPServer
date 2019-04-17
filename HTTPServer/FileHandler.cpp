#include "FileHandler.h"

#include "Logger.h"

FileHandler::FileHandler()
{

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

void FileHandler::CloseFile()
{
	CurrentFile.close();
}

bool FileHandler::NextLine(std::string &line)
{
	if (CurrentFile.eof() || !FileIsOk())
	{
		return false;
	}

	std::getline(CurrentFile, line);
	return true;
}

std::string FileHandler::GetFileData()
{
	std::string content = "";

	CurrentFile.clear();
	CurrentFile.seekg(0, std::ios::beg);

	std::string line;
	while (NextLine(line))
	{
		content += line + "\n";
	}

	return content;
}

long long FileHandler::GetFileLength()
{
	long long count = 0;
	std::string line;

	while (NextLine(line))
	{
		count += line.length();
		count++;
	}

	return count;
}

bool FileHandler::FileIsOk()
{
	if (!CurrentFile || !CurrentFile.good() || !CurrentFile.is_open())
	{
		return false;
	}

	return true;
}