#include "Network.h"
using namespace std;

CNetwork::CNetwork(void)
{
	m_wVersionRequested = MAKEWORD(2, 2);
	m_sockServer = INVALID_SOCKET;
	m_error = NetworkError::NoError;

	int err = WSAStartup(m_wVersionRequested, &m_wsaData);
	if ( err != 0 ) 
	{
		m_error = NetworkError::NotInitLib;
	}
}


CNetwork::~CNetwork(void)
{
	if (m_sockServer != INVALID_SOCKET)
	{
		shutdown(m_sockServer, SD_BOTH);
		closesocket(m_sockServer);
	}

	if (m_error != NetworkError::NotInitLib)
		WSACleanup();
}

bool CNetwork::Listen(int port)
{
	// Check Startup
	if (m_error == NetworkError::NotInitLib)
		return false;

	// Create Socket
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sockServer == INVALID_SOCKET) {
		m_error = NetworkError::NotInitSocket;
		return false;
	}

	sockaddr_in serverAdd;
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_port = htons(port);
	serverAdd.sin_addr.s_addr=htonl(INADDR_ANY);

	// Listen to client
	int iResult = bind(m_sockServer, (sockaddr*)&serverAdd, sizeof(serverAdd));
	if (iResult == SOCKET_ERROR) {
		m_error = NetworkError::NotConnect;
		closesocket(m_sockServer);
		m_sockServer = INVALID_SOCKET;
		return false;
	}
	listen(m_sockServer, 10);

	return true;
}

SOCKET CNetwork::Accept()
{
	return accept(m_sockServer, NULL, NULL);
}

void CNetwork::Send(SOCKET socket, int number)
{
	if (socket == INVALID_SOCKET)
		return;

	send(socket, (char*)&number, sizeof(int), 0);
}

void CNetwork::Send(SOCKET socket, string s)
{
	if (socket == INVALID_SOCKET)
		return;

	char len = (char)(s.length() + 1);

	send(socket, (char*)&len, sizeof(char), 0);
	send(socket, s.c_str(), len, 0);
}

void CNetwork::Send(SOCKET socket, ClientInfo clientinfo)
{
	if (socket == INVALID_SOCKET)
		return;

	this->Send(socket, clientinfo.No);
	this->Send(socket, clientinfo.NickName);
	this->Send(socket, clientinfo.Score);
}

string CNetwork::ReadString(SOCKET socket)
{
	if (socket == INVALID_SOCKET)
		return string("");

	char len = 1;
	recv(socket, (char*)&len, sizeof(char), 0);
	char* s = new char[(int)len];
	s[0] = 0;
	recv(socket, s, len, 0);

	string ret(s);
	delete s;
	return ret;
}