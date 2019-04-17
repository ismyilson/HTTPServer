#include "ContentHandler.h"

ContentHandler::ContentHandler()
{

}

std::string ContentHandler::GetContentType(ContentType cType)
{
	switch (cType)
	{
		case CONTENT_PLAIN:
			return "text/plain";

		case CONTENT_HTML:
			return "text/html";

		default:
			return "text/plain";
	}
}

std::string ContentHandler::GetContentType(std::string fileName)
{
	std::string extension = fileName.substr(fileName.find_last_of('.') + 1);
	
	if (strcmp(extension.c_str(), "html") == 0)
		return GetContentType(CONTENT_HTML);
	else
		return GetContentType(CONTENT_PLAIN);
}