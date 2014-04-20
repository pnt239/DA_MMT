#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>

#include "GUI.h"
#include "Network.h"
#include "utils.h"

#define PORT_APP	30000

#define REG_SUCC	13
#define REG_DUP		12

class CApp
{
public:
	CApp(void);
	~CApp(void);

	bool Connect();
	void Run();

private:
	CGUI* m_gui;
	CNetwork* m_network;

	int m_no;
	
};

