#include "App.h"
#include <string>
using namespace std;

CApp::CApp(void)
{
	m_gui = new CGUI();
	m_network = new CNetwork();
}


CApp::~CApp(void)
{
	delete m_network;
	delete m_gui;
}

bool CApp::Connect()
{
	string ip = m_gui->AskUser("Nhap IP may chu: ");
	return m_network->Connect(ip.c_str(), PORT_APP);
}

void CApp::Run()
{
	/* Registration */
	// Enter checked nickname
	string nickname = m_gui->AskUser("Nhap nickname can dang ky: ");
	// Send nickname

	send(m_sockClient, nickname.c_str(), nickname.length() + 1, 0);
}

bool CApp::Is_Ok()
{
	return m_error;
}