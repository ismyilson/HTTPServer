#pragma once

#include <WinSock2.h>

#include "Config.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

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

	// Clears everything before for a safe exit
	void CleanUp();

	// Starts listening for incoming connections on the main thread
	void AcceptClients();

	// Creates a thread for handling the accepted client
	void AcceptClient(SOCKET);

	// Sends a response back to the client
	void AcceptClientEx(SOCKET);

	SOCKET Socket;
	bool RequiresCleanUp;
};