#include "Response.h"

Response::Response()
{
	response = HTTP_VERSION;
}

Response::~Response()
{
}

void Response::SetResponseType(ResponseType rType)
{
	response += " ";
	switch (rType)
	{
		case RESPONSE_OK:
			response += "200 OK";
			break;

		case RESPONSE_NOT_FOUND:
			response += "404 Not Found";
			break;

		default:
			break;
	}
	response += "\n";
}

void Response::SetContentType(ContentType cType)
{
	response += "Content-Type: " + ContentHandler::GetContentType(cType);
	response += "\n";
}

void Response::SetContentType(std::string fileName)
{
	response += "Content-Type: " + ContentHandler::GetContentType(fileName);
	response += "\n";
}

void Response::SetData(std::string data)
{
	response += "Content-Length: " + std::to_string(data.length());
	response += "\n\n";
	response += data;
}