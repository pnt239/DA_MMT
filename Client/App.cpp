#include "App.h"
#include <string>
using namespace std;

CApp::CApp(void)
{
	m_gui = new CGUI();
	m_network = new CNetwork();
	m_turn = 0;
}


CApp::~CApp(void)
{
	delete m_network;
	delete m_gui;
}

bool CApp::Connect()
{
	string ip = m_gui->ShowAsk("Nhap IP may chu: ");
	return m_network->Connect(ip.c_str(), PORT_APP);
}

void CApp::Run()
{
	/* Registration */
	// Enter checked nickname
	bool error = false;
	do
	{
		string nickname = m_gui->ShowAsk("Nhap nickname can dang ky: ");
		if (checkName(nickname))
		{
			m_gui->SetAlert("");
			*m_network<<nickname;
			error = false;
		}
		else
		{
			m_gui->SetAlert("Nickname chua ki tu khong hop le!");
			error = true;
		}

		if (!error)
			if (m_network->ReadInt() == REG_DUP)
				error = true;
	} while (error);

	// Get number ordinal
	m_gui->ShowInfo("Doi xiu de server lay so thu tu...");
	*m_network>>m_no;

	// Get Players List
	int nclient = m_network->ReadInt();
	for (int i = 0; i < nclient; i++)
		m_gui->AddPlayer(m_network->ReadPlayer());

	while (true)
	{
		// Get Hint and length of Answer
		int lenAns = m_network->ReadInt();
		string hint = m_network->ReadString();
		m_gui->SetAnswer(string(lenAns, ' '));

		m_gui->SetQuestion(hint);

		while (true)
		{
			int whoplay = m_network->ReadInt();

			if (whoplay == m_no)
			{
				*m_network>>m_turn;

				m_gui->ShowGame(m_no, m_turn);

				*m_network<<m_gui->GetGuessChar();
				*m_network<<m_gui->GetGuessString();

				if (m_turn > 2)
				{
					int code = GAME_LOSE;
					*m_network>>code;
					if (code == GAME_WIN) {
						// Do something
					}
					else
					{
						// Do something
					}
				}
				else
				{
					int countchar = 0, poschar = 0, ch;
					*m_network>>ch;
					*m_network>>countchar;
					for (int j = 0; j < countchar; j++)
					{
						*m_network>>poschar;
						m_gui->SetAnswer(poschar, (char)ch);
					}

					m_gui->SetAlert("Co " + to_string(countchar) + " chu " + string(1, (char)ch));
				}
			}

		}
	}
}

bool isCorrectNickname(char c)
{
	return isalpha(c) || isdigit(c) || c == '_';
}

bool CApp::checkName(string name)
{
	if (name.length() > 10)
		return false;

	for (string::iterator i = name.begin(); i != name.end(); i++)
		if (!isCorrectNickname(*i))
			return false;

	return true;
}