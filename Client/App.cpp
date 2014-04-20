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
	do
	{
		string nickname = m_gui->AskUser("Nhap nickname can dang ky: ");
		*m_network<<nickname;
	} while (m_network->ReadInt() == REG_DUP);

	// Get number ordinal
	*m_network>>m_no;

	// Get Players List
	int nclient = m_network->ReadInt();
	for (int i = 0; i < nclient; i++)
		m_gui->AddPlayer(m_network->ReadPlayer());

	while (true)
	{
		system("cls");
		m_gui->ShowGame();
	}
}