#pragma once

#include <string>

#include "ContentHandler.h"

#define	HTTP_VERSION	"HTTP/1.1"

enum ResponseType
{
	RESPONSE_OK,
	RESPONSE_NOT_FOUND
};

class Response
{
public:
	Response();
	~Response();

	std::string GetResponseStr() { return response; }
	const char* GetResponseCStr() { return response.c_str(); }

	void SetResponseType(ResponseType);

	void SetContentType(ContentType);
	void SetContentType(std::string);

	void SetData(std::string);

private:
	std::string response;
};

