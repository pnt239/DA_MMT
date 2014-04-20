#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>

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

	bool Connect(const char* ip, int port);

private:
	WORD m_wVersionRequested;
	WSADATA m_wsaData;
	SOCKET m_sockClient;

	NetworkError m_error;
};

