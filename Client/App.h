#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "GUI.h"
#include "Network.h"
#include "utils.h"

#define PORT_APP	30000

#define REG_SUCC		13
#define REG_DUP			12

#define GAME_WIN		20
#define GAME_LOSE		21
#define GAME_END		22
#define GAME_CON		23
#define GAME_TURNOUT	24
#define GAME_RESIGN		25

#define GAME_GSS_CHR	30
#define GAME_GSS_STR	31

class CApp
{
public:
	CApp(void);
	~CApp(void);

	bool Connect();
	void Run();

private:
	bool checkName(std::string);
	CGUI* m_gui;
	CNetwork* m_network;

	int m_no;
	int m_turn;
	int m_opencount;
};

