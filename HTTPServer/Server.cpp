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

	CreateThreads();
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

	result = getaddrinfo(NULL, ServerConfig->GetPort().c_str(), &hints, &addrResult);
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

void Server::CreateThreads()
{
	std::thread *t1 = new std::thread(&Server::AcceptClients, this);
	std::thread *t2 = new std::thread(&Server::CheckIfDataReceived, this);

	t1->join();
	t2->join();
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
	auto it = std::find(Clients.begin(), Clients.end(), ClientSocket);
	if (it == Clients.end())
	{
		Clients.push_back(ClientSocket);
		LOG("New client accepted");
	}
}

void Server::CheckIfDataReceived()
{
	LOG("Waiting for data");

	int status;
	fd_set readSet;
	timeval *timeInterval = new timeval();
	timeInterval->tv_sec = 0;
	timeInterval->tv_usec = 30;

	while (true)
	{
		for (size_t i = 0; i < Clients.size(); ++i)
		{
			FD_ZERO(&readSet);
			FD_SET(Clients[i], &readSet);

			status = select(Clients[i] + 1, &readSet, NULL, NULL, timeInterval);

			if (status == SOCKET_ERROR)
			{
				RemoveSocket(Clients[i]);
				i--;
				continue;
			}

			if (status == 0)
			{
				// No data to receive from this socket
				continue;
			}

			if (!ReceiveClientData(Clients[i]))
			{
				i--;
			}
		}
	}
}

bool Server::ReceiveClientData(SOCKET ClientSocket)
{
	LOG("Data Received");
	
	char buffer[1024];
	int result;
	
	result = recv(ClientSocket, buffer, sizeof buffer, 0);

	if (result == 0)
	{
		RemoveSocket(ClientSocket);
		return false;
	}

	RespondToClient(ClientSocket, buffer);
	return true;
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
	LOG("Response sent");
	send(ClientSocket, response->GetResponseCStr(), strlen(response->GetResponseCStr()), 0);
}

void Server::RemoveSocket(SOCKET ClientSocket)
{
	auto it = std::find(Clients.begin(), Clients.end(), ClientSocket);
	if (it == Clients.end()) // Should never happen?
	{
		return;
	}

	int pos = std::distance(Clients.begin(), it);

	closesocket(Clients[pos]);
	Clients.erase(Clients.begin() + pos);
}