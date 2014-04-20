#include "Network.h"


CNetwork::CNetwork(void)
{
	m_wVersionRequested = MAKEWORD(2, 2);
	m_sockClient = INVALID_SOCKET;
	m_error = NetworkError::NoError;

	int err = WSAStartup(m_wVersionRequested, &m_wsaData);
	if ( err != 0 ) 
	{
		m_error = NetworkError::NotInitLib;
	}
}


CNetwork::~CNetwork(void)
{
	if (m_sockClient != INVALID_SOCKET)
	{
		shutdown(m_sockClient, SD_BOTH);
		closesocket(m_sockClient);
	}

	if (m_error != NetworkError::NotInitLib)
		WSACleanup();
}

bool CNetwork::Connect(const char* ip, int port)
{
	// Check Startup
	if (m_error == NetworkError::NotInitLib)
		return false;

	// Create Socket
	SOCKET m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sockClient == INVALID_SOCKET) {
		m_error = NetworkError::NotInitSocket;
		return false;
	}

	sockaddr_in serverAdd;
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_port = htons(port);
	serverAdd.sin_addr.s_addr = inet_addr(ip);

	// Connect to server
	int iResult = connect(m_sockClient, (sockaddr*)&serverAdd, sizeof(serverAdd));
	if (iResult == SOCKET_ERROR) {
		m_error = NetworkError::NotConnect;
		closesocket(m_sockClient);
		m_sockClient = INVALID_SOCKET;
		return false;
	}

	return true;
}