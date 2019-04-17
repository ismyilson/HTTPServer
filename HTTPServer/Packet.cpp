#include "Packet.h"

#include "Logger.h"

Packet::Packet(std::string data)
{
	std::string dataFirstLine = data.substr(0, data.find('\n'));

	size_t HTTPPos = dataFirstLine.find_last_of("HTTP") - 4;
	HTTPVersion = dataFirstLine.substr(HTTPPos);

	size_t FileNamePos = dataFirstLine.find_first_of('/');
	FileName = dataFirstLine.substr(FileNamePos, HTTPPos - FileNamePos);
}

Packet::~Packet()
{

}

