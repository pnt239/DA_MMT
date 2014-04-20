#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <string>

#include "utils.h"

enum NetworkError
{
	NoError = 1, 
	NotInitLib,
	NotInitSocket,
	NotConnect
};

class CNetwork
{
public:
	CNetwork(void);
	~CNetwork(void);

	bool Listen(int port);
	SOCKET Accept();

	void Send(SOCKET, int);
	void Send(SOCKET, std::string);
	void Send(SOCKET, ClientInfo);

	std::string ReadString(SOCKET);
private:
	WORD m_wVersionRequested;
	WSADATA m_wsaData;
	SOCKET m_sockServer;

	NetworkError m_error;
};

