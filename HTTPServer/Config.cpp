#include "Config.h"

#include <algorithm>

#include "Logger.h"
#include "FileHandler.h"

ConfigObjectMap ConfigMap;

Config::Config()
{
	SetupConfigStrings();
	ReadConfig();
}

Config::~Config()
{

}

void Config::SetupConfigStrings()
{
	ConfigMap["serverfilesfolder"] = CONFIG_SERVER_FILES_FOLDER;
	ConfigMap["serverport"] = CONFIG_SERVER_PORT;
}

void Config::ReadConfig()
{
	FileHandler *fHandler = new FileHandler();
	if (!fHandler->OpenFile(CONFIG_FILE_PATH))
	{
		LOG_ERROR("Could not open configuration file ");
		exit(1);
	}

	std::string line, configName, value;
	while (fHandler->NextLine(line))
	{
		size_t pos = line.find_first_of('=');
		configName = line.substr(0, pos);
		value = line.substr(pos + 1);

		ApplyIfValid(configName, value);
	}

	fHandler->CloseFile();
}

void Config::ApplyIfValid(std::string configName, std::string value)
{
	std::transform(configName.begin(), configName.end(), configName.begin(), tolower);

	ConfigObjectMap::iterator it = ConfigMap.find(configName);
	if (it == ConfigMap.end())
		return;

	ApplyConfig(it->second, value);
}

void Config::ApplyConfig(int ConfigNum, std::string value)
{
	switch (ConfigNum)
	{
		default:
			break;

		case CONFIG_SERVER_FILES_FOLDER:
			LOG("Set server files folder to " + value);
			Configuration.ServerFilesFolder = value;
			break;

		case CONFIG_SERVER_PORT:
			LOG("Set server port to " + value);
			Configuration.ServerPort = value;
			break;
	}
}