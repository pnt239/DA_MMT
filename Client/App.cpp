#include "App.h"
#include <string>
using namespace std;

CApp::CApp(void)
{
	m_gui = new CGUI();
}


CApp::~CApp(void)
{
	delete m_gui;
}

void CApp::Run()
{
	/* Registration */
	// Enter checked nickname
	string nickname = m_gui->AskUser("Nhap nickname can dang ky: ");
	// Send nickname
	
}