#pragma once

#include <fstream>
#include <string>

class FileHandler
{
public:
	FileHandler();
	~FileHandler();

	bool OpenFile(std::string);
	void CloseFile();

	bool NextLine(std::string&);
	std::string GetFileData();

	long long int GetFileLength();

private:
	bool FileIsOk();

	std::fstream CurrentFile;
};

