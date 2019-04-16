#pragma once

#include <map>

#define	CONFIG_FILE_PATH	"config/config.cfg"
#define	CONFIG_ELEMENTS	1

typedef std::map<int, std::string> ConfigObject;

enum ConfigEnum
{
	CONFIG_SERVER_FILES_FOLDER = 0
};

ConfigObject ConfigStrings =
{
	{ CONFIG_SERVER_FILES_FOLDER, "serverfilesfolder" }
};

struct ConfigStructure
{
public:
	std::string ServerFilesFolder;
};

class Config
{
public:
	Config();
	~Config();

private:
	void SetupConfigStrings();
	void ReadConfig();
	void ApplyConfig(int);

	ConfigObject IsValidConfig(std::string);

	ConfigStructure Configuration;
};



extern Config *GlobalConfig;