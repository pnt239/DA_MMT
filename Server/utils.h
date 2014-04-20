#pragma once
#include <string>

struct ClientInfo
{
	SOCKET Socket;
	int No;
	std::string NickName;
	int Score;
	bool Enable;
};