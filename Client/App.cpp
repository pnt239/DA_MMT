#include "App.h"
#include <string>
using namespace std;

CApp::CApp(void)
{
	m_gui = new CGUI();
	m_network = new CNetwork();
	m_turn = 1;
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
			{
				m_gui->SetAlert("Nickname bi trung!");
				error = true;
			}
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
		int whoplay;
		int turn;
		bool isplay = false;
		int gameevent = 0;
		int lenAns = m_network->ReadInt();
		string hint = m_network->ReadString();

		m_gui->SetAnswer(string(lenAns, ' '));
		m_gui->SetQuestion(hint);

		while (true)
		{
			whoplay = m_network->ReadInt();

			// Check current player
			// isplay = true: Now is my turn, I will play
			// isplay = false: Now is my friend's turn... So I wait
			isplay = whoplay == m_no;
			
			// Get player's turn
			*m_network>>turn;
			// Check ouf of turn
			if ((gameevent = m_network->ReadInt()) == GAME_TURNOUT)
			{
				m_gui->SetAlert("Qua so luot choi quy dinh! Tro choi ket thuc....");
				m_gui->ShowGame(m_no, m_turn, true);
				m_gui->SetAlert("");
				break;
			}
			else if (gameevent == GAME_END)
			{
				m_gui->SetAlert("");
				break;
			}

			if (isplay) m_turn = turn;
			
			// Show interface
			m_gui->ShowGame(m_no, m_turn, !isplay);

			if (isplay)
			{
				// Send my guess char and guess string
				*m_network<<m_gui->GetGuessChar();
				*m_network<<m_gui->GetGuessString();
			}

			int ch, countchar = 0, poschar = 0, nextplayer;
			char guesschar;

			if ((isplay && m_gui->GetGuessString().length() == 0) || (!isplay && m_network->ReadInt() == GAME_GSS_CHR))
			{
				// My friend or I will recieve my guess char in oder to display on screen
				*m_network>>ch;
				guesschar = (char)ch;
				// My friend or I will recieve number correct character.
				*m_network>>countchar;

				string prefix;
				if (isplay)
					prefix = "Ban";
				else
					prefix = "Nguoi choi thu " + to_string(whoplay);
				prefix += " doan chu " + string(1, guesschar) + ". ";

				if (countchar <= 0)
				{
					// Get next player
					*m_network>>nextplayer;;
					m_gui->SetAlert(prefix + "Khong co hoac da duoc doan roi! Luot choi se danh cho nguoi choi thu " + to_string(nextplayer) + "...");
				}
				else
				{
					for (int j = 0; j < countchar; j++)
					{
						*m_network>>poschar;
						m_gui->SetAnswer(poschar, guesschar);
					}

					m_gui->SetAlert(prefix + "Co " + to_string(countchar) + " chu " + string(1, guesschar));
				}
			}

			// Wait for getting game event
			gameevent = GAME_LOSE;
			*m_network>>gameevent;

			// Update Players' info
			if (gameevent == GAME_WIN || countchar > 0)
				m_gui->UpdatePlayer(whoplay, m_network->ReadInt());

			string t;
			if (gameevent == GAME_WIN)
			{
				m_gui->SetAnswer(m_network->ReadString());

				if (isplay)
					t = "Chuc mung ban... Ban";
				else
					t = "Nguoi choi thu " + to_string(whoplay);

				m_gui->SetAlert(t + " da danh chien thang trong luot choi nay !!!");
				m_gui->ShowGame(m_no, m_turn, true);
				m_gui->SetAlert("");
				break;
			}
			else if (gameevent == GAME_LOSE)
			{
				if (isplay)
					t = "Ban";
				else
					t = "Nguoi choi thu " + to_string(whoplay);

				m_gui->SetAlert(t + " da thua roi...");
				m_gui->ShowGame(m_no, m_turn, true);
			}
			else if (gameevent == GAME_RESIGN)
			{
				m_gui->SetAlert("Nguoi choi thu " + to_string(whoplay) + " da bo cuoc!");
				m_gui->ShowGame(m_no, m_turn, true);
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