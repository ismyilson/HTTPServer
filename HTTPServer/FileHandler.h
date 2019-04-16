#pragma once

#include <fstream>
#include <string>

class FileHandler
{
public:
	FileHandler();
	FileHandler(std::string);
	~FileHandler();

	bool NextLine(std::string&);
	bool OpenFile(std::string);

private:
	bool FileIsOk();

	std::fstream CurrentFile;
};

