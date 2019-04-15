#include "Server.h"

#include <thread>
#include <WS2tcpip.h>

#include "Logger.h"

Server::Server()
{
	Socket = INVALID_SOCKET;
	RequiresCleanUp = false;
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
		printf("Listen failed with error: %ld\n" + WSAGetLastError());
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

	std::thread thrd(&Server::AcceptClientEx, this, ClientSocket);
	thrd.join();
}

void Server::AcceptClientEx(SOCKET ClientSocket)
{
	char response[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 55\n\n<html><body><h1>This is your website!<h1></body></html>";

	send(ClientSocket, response, strlen(response), 0);
}