#pragma once

#include <string>

#include "FileHandler.h"

enum ContentType
{
	CONTENT_PLAIN,
	CONTENT_HTML
};

class ContentHandler
{
public:
	ContentHandler();

	static std::string GetContentType(ContentType);
	static std::string GetContentType(std::string);
};

