#pragma once

#include <vector>
#include <WinSock2.h>

#include "Config.h"
#include "Response.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define	LANDING_PAGE	"index.html"

class Server
{
public:
	Server();
	~Server();

	// Starts the server
	void Start();

private:
	// Creates and opens socket
	void InitSocket();
	SOCKET Socket;

	// Clears everything before for a safe exit
	void CleanUp();
	bool RequiresCleanUp;

	// Creates the listen and the recv threads
	void CreateThreads();

	// Starts listening for incoming connections on the main thread
	void AcceptClients();

	// Creates a thread for receiving data from client
	void AcceptClient(SOCKET);

	// Checks if there's any data received from the connected clients
	void CheckIfDataReceived();

	// Reads data form the client socket, returns false if socket closes
	bool ReceiveClientData(SOCKET);

	// Builds the response
	void RespondToClient(SOCKET, std::string);

	// Sends a response to the socket
	void SendResponse(Response*, SOCKET);

	// Closes and removes a socket from the connected sockets
	void RemoveSocket(SOCKET);
	
	Config *ServerConfig;

	std::vector<SOCKET> Clients;
};