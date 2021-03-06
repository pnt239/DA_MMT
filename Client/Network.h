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

	bool Connect(const char* ip, int port);

	int ReadInt();
	std::string ReadString();
	PlayerInfo ReadPlayer();

	void operator<<(int);
	void operator<<(std::string);

	void operator>>(int&);
	void operator>>(std::string&);
private:
	WORD m_wVersionRequested;
	WSADATA m_wsaData;
	SOCKET m_sockClient;

	NetworkError m_error;
};

