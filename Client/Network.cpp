#include "Network.h"
using namespace std;

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
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

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

int CNetwork::ReadInt()
{
	int ret = 0;

	if (m_sockClient == INVALID_SOCKET)
		return 0;

	recv(m_sockClient, (char*)&ret, sizeof(int), 0);
	return ret;
}

string CNetwork::ReadString()
{
	if (m_sockClient == INVALID_SOCKET)
		return string("");

	char len;
	recv(m_sockClient, (char*)&len, sizeof(char), 0);
	char* s = new char[(int)len];
	recv(m_sockClient, s, len, 0);

	string ret(s);
	delete s;
	return ret;
}

PlayerInfo CNetwork::ReadPlayer()
{
	PlayerInfo player;
	*this>>player.No;
	*this>>player.NickName;
	*this>>player.Score;
	return player;
}

void CNetwork::operator<<(int number)
{
	if (m_sockClient == INVALID_SOCKET)
		return;

	send(m_sockClient, (char*)&number, sizeof(int), 0);
}

void CNetwork::operator<<(string s)
{
	if (m_sockClient == INVALID_SOCKET)
		return;

	char len = (char)(s.length() + 1);
	send(m_sockClient, (char*)&len, sizeof(len), 0);
	send(m_sockClient, s.c_str(), len, 0);
}

void CNetwork::operator>>(int& number)
{
	number = ReadInt();
}

void CNetwork::operator>>(std::string& s)
{
	s = ReadString();
}