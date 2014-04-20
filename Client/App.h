#pragma once

#include <cstdlib>
#include <iostream>
#include "GUI.h"
#include "Network.h"

#define PORT_APP 30000

class CApp
{
public:
	CApp(void);
	~CApp(void);

	bool Connect();
	void Run();
	bool Is_Ok();

private:
	CGUI* m_gui;
	CNetwork* m_network;

	bool m_error;

	WORD m_wVersionRequested;
	WSADATA m_wsaData;
	SOCKET m_sockClient;
};

