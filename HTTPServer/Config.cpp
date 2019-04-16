#include "Config.h"

#include <algorithm>

#include "Logger.h"
#include "FileHandler.h"

Config *GlobalConfig;

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
	ConfigStrings.insert(std::pair<int, std::string>(0, "asd"));
}

void Config::ReadConfig()
{
	FileHandler *fHandler = new FileHandler();
	if (!fHandler->OpenFile(CONFIG_FILE_PATH))
	{
		LOG_ERROR("Could not open configuration file ");
		exit(1);
	}

	std::string line, configName;
	while (fHandler->NextLine(line))
	{
		configName = line.substr(0, line.find('='));

		auto ConfigObj = IsValidConfig(configName);
		if (ConfigObj.first != -1)
		{
			ApplyConfig(ConfigObj);
		}
	}
}

ConfigObject Config::IsValidConfig(std::string configName)
{
	std::transform(configName.begin(), configName.end(), configName.begin(), tolower);

	for (ConfigObject configObj : ConfigStrings)
	{
		if (strcmp(configObj.second.c_str(), configName.c_str()) == 0)
		{
			return configObj;
		}
	}

	return NULL;
}

void Config::ApplyConfig(int ConfigNum)
{
	switch (ConfigNum)
	{
		default:
			break;

		case CONFIG_SERVER_FILES_FOLDER:
			break;
	}
}