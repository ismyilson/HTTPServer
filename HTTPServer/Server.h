#pragma once

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

	// Starts listening for incoming connections on the main thread
	void AcceptClients();

	// Creates a thread for handling the accepted client
	void AcceptClient(SOCKET);

	// Actually accepts the client
	void AcceptClientEx(SOCKET);

	// Builds the response
	void RespondToClient(SOCKET);

	// Sends a response to the socket
	void SendResponse(Response*, SOCKET);
	
	Config *ServerConfig;
};