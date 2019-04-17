#pragma once

#include <map>

#define	CONFIG_FILE_PATH	"config/config.cfg"

typedef std::map<std::string, int> ConfigObjectMap;

enum ConfigEnum
{
	CONFIG_SERVER_FILES_FOLDER = 0,

	CONFIG_MAX
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

	std::string GetFilesFolder() { return Configuration.ServerFilesFolder; }

private:
	void SetupConfigStrings();
	void ReadConfig();
	void ApplyConfig(int, std::string);

	void ApplyIfValid(std::string, std::string);

	ConfigStructure Configuration;
};

extern ConfigObjectMap ConfigMap;