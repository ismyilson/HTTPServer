#include "Server.h"

#include <thread>
#include <WS2tcpip.h>

#include "FileHandler.h"
#include "Logger.h"
#include "Packet.h"

void Pause()
{
	system("pause");
}

Server::Server()
{
	atexit(Pause);

	Socket = INVALID_SOCKET;
	RequiresCleanUp = false;

	ServerConfig = new Config();
}

Server::~Server()
{
	if (RequiresCleanUp)
	{
		CleanUp();
	}
}

void Server::Start()
{
	LOG("Starting server");

	InitSocket();

	AcceptClients();
}

void Server::InitSocket()
{
	LOG("Initializing socket");

	int result;

	WSAData wsaData;

	// Startup WSAData
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		LOG_ERROR("WSAStartup failed, error code: " + result);
		exit(result);
	}

	// Get values of sockaddr
	struct addrinfo *addrResult = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	result = getaddrinfo(NULL, "80", &hints, &addrResult);
	if (result != 0)
	{
		LOG_ERROR("getaddrinfo failed, error code: " + result);
		WSACleanup();
		exit(result);
	}

	// Create the socket
	Socket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

	if (Socket == INVALID_SOCKET)
	{
		LOG_ERROR("socket failed, error code: " + WSAGetLastError());
		freeaddrinfo(addrResult);
		WSACleanup();
		exit(result);
	}

	// Bind the socket
	result = bind(Socket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		LOG_ERROR("bind failed, error code: " + WSAGetLastError());
		freeaddrinfo(addrResult);
		closesocket(Socket);
		WSACleanup();
		exit(result);
	}

	// Free memory allocated by getaddrinfo
	freeaddrinfo(addrResult);

	if (listen(Socket, SOMAXCONN) == SOCKET_ERROR) {
		LOG_ERROR("listen failed with error: " + WSAGetLastError());
		closesocket(Socket);
		WSACleanup();
		exit(result);
	}

	RequiresCleanUp = true;
}

void Server::CleanUp()
{
	closesocket(Socket);
	WSACleanup();
}

void Server::AcceptClients()
{
	LOG("Accepting clients");

	SOCKET ClientSocket;
	while (ClientSocket = accept(Socket, NULL, NULL))
	{
		if (ClientSocket == INVALID_SOCKET)
			continue;

		AcceptClient(ClientSocket);
	}
}

void Server::AcceptClient(SOCKET ClientSocket)
{
	LOG("New client accepted");

	std::thread recvThread(&Server::ReceiveClientData, this, ClientSocket);
	recvThread.join();
}

void Server::ReceiveClientData(SOCKET ClientSocket)
{
	char buffer[1024];
	int result = -1;

	while (result != 0)
	{
		result = recv(ClientSocket, buffer, sizeof buffer, 0);
		RespondToClient(ClientSocket, buffer);
	}
}

void Server::RespondToClient(SOCKET ClientSocket, std::string data)
{
	FileHandler *fHandler = new FileHandler();
	Packet *packet = new Packet(data);

	Response *response = new Response();
	response->SetResponseHTTPVersion(packet->HTTPVersion);

	std::string fileName = packet->FileName;
	if (strcmp(fileName.c_str(), "/") == 0)
	{
		fileName += LANDING_PAGE;
	}

	fileName.insert(0, ServerConfig->GetFilesFolder());

	if (!fHandler->OpenFile(fileName))
	{
		response->SetResponseType(RESPONSE_NOT_FOUND);
		response->SetContentType(CONTENT_PLAIN);
		response->SetData("404 Not Found");

		SendResponse(response, ClientSocket);
		return;
	}

	response->SetResponseType(RESPONSE_OK);
	response->SetContentType(fileName);
	response->SetData(fHandler->GetFileData());

	SendResponse(response, ClientSocket);

	fHandler->CloseFile();
}

void Server::SendResponse(Response *response, SOCKET ClientSocket)
{
	send(ClientSocket, response->GetResponseCStr(), strlen(response->GetResponseCStr()), 0);
}