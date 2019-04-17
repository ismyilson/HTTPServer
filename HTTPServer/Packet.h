#pragma once

#include <string>

struct Packet
{
public:
	Packet(std::string data);
	~Packet();

	std::string HTTPVersion;
	std::string FileName;
};

