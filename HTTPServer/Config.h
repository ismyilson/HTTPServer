#pragma once

#include <map>

#define	CONFIG_FILE_PATH	"config/config.cfg"

typedef std::map<std::string, int> ConfigObjectMap;

enum ConfigEnum
{
	CONFIG_SERVER_FILES_FOLDER = 0,
	CONFIG_SERVER_PORT,

	CONFIG_MAX
};

struct ConfigStructure
{
public:
	std::string ServerFilesFolder;
	std::string ServerPort;
};

class Config
{
public:
	Config();
	~Config();

	std::string GetFilesFolder() { return Configuration.ServerFilesFolder; }
	std::string GetPort() { return Configuration.ServerPort; }

private:
	void SetupConfigStrings();
	void ReadConfig();
	void ApplyConfig(int, std::string);

	void ApplyIfValid(std::string, std::string);

	ConfigStructure Configuration;
};

extern ConfigObjectMap ConfigMap;