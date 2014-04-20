#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <cstdlib>
#include <iostream>
#include "GUI.h"

class CApp
{
public:
	CApp(void);
	~CApp(void);

	void Run();

private:
	CGUI* m_gui;
};

