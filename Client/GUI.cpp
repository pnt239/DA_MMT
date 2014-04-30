#include "GUI.h"
#include <windows.h>
#include <iostream>
using namespace std;


CGUI::CGUI(void)
{
	m_question = "Rua nao co bon muoi chan? pham ngoc thanh pham ngocj thanh pham ngoc thanh pham ngoc thanh";
	m_answer = CreateLogo();
	m_mode = 1;
}


CGUI::~CGUI(void)
{
}

string CGUI::ShowAsk(string ques)
{
	ShowCell(true);
	ShowAlert(m_alert, 1);
	return AskUser(ques, m_curRow);
}

void CGUI::ShowInfo(string info)
{
	ShowCell(true);
	ShowAlert(info, 0);
}

void CGUI::ShowGame(int no, int turn, bool view)
{
	int ansRow;

	ShowCell(false);
	ShowOrder(no, turn);
	ShowQuestion(m_question);
	ShowAlert(m_alert, 0);

	ansRow = m_curRow;
	m_curRow += 3;

	ShowPlayers();

	GoToXY(m_curCol, ansRow);

	if (!view)
	{
		m_guessstr = m_guesschar = "";
		if (turn > 2)
		{
			string yesno = AskUser("Ban muon doan luon khong? (1-yes | 2-no) ", ansRow++);
			if (yesno == "1")
			{
				m_guessstr = AskUser("Cum tu doan: ", ansRow++);
				return;
			}
		}
		m_guesschar = AskUser("Ban doan chu: ", ansRow++);
	}

}

void CGUI::Pause()
{
	GoToXY(m_curCol, m_curRow++);
	system("pause");
}

void CGUI::AddPlayer(PlayerInfo player)
{
	players.push_back(player);
}

void CGUI::UpdatePlayer(int pos, int score)
{
	players[pos].Score = score;
}

void CGUI::RemovePlayer(int id)
{
	players.erase (players.begin()+id);
}

void CGUI::SetQuestion(string ques)
{
	m_question = ques;
}

void CGUI::SetAlert(string alert)
{
	m_alert = alert;
}

void CGUI::SetAnswer(string s)
{
	m_answer = s;
}

void CGUI::SetAnswer(int pos, char c)
{
	m_answer[pos] = c;
}

string CGUI::GetGuessChar()
{
	return m_guesschar;
}

string CGUI::GetGuessString()
{
	return m_guessstr;
}

string CGUI::AskUser(string question, int line)
{
	GoToXY(m_curCol, line);

	cout<<question;

	string res;
	cin>>res;
	return res;
}

void CGUI::ShowCell(bool showcell)
{
	system("cls");
	// Calculate First Position
	int padding = 2;
	int lenAnswer = m_answer.length();

	m_curCol = (CONSOLE_COLUMN - lenAnswer - padding*2)/2;
	m_curRow = 1;

	// Show line
	GoToXY(m_curCol, m_curRow++);
	for (int i = 0; i < lenAnswer + padding*2; i++)
		cout<<(char)176;

	// Show space
	GoToXY(m_curCol, m_curRow++);
	cout<<(char)176;
	for (int i = 0; i < lenAnswer + padding; i++)
		cout<<' ';
	cout<<(char)176;

	// Show text
	GoToXY(m_curCol, m_curRow++);
	
	cout<<(char)176;
	cout<<' ';
	for (int i = 0; i < lenAnswer; i++)
		if (m_answer[i] == ' ')
			cout<<(char)177;
		else 
			cout<<m_answer[i];

	cout<<' ';
	cout<<(char)176;

	// Show space
	GoToXY(m_curCol, m_curRow++);
	cout<<(char)176;
	for (int i = 0; i < lenAnswer + padding; i++)
		cout<<' ';
	cout<<(char)176;

	// Show line
	GoToXY(m_curCol, m_curRow++);
	for (int i = 0; i < lenAnswer + padding*2; i++)
		cout<<(char)176;
	
	// endline
	m_curRow++;

	// Show cell table info
	if (!showcell)
	{
		GoToXY(m_curCol, m_curRow++);
		cout<<"O chu co "<<lenAnswer<<" chu cai.";
	}

	// Set indent again
	m_curCol = (CONSOLE_COLUMN - QUESTION_WIDTH) / 2;
	m_curRow++;
}

void CGUI::ShowOrder(int no, int turn)
{
	GoToXY(m_curCol, m_curRow++);
	cout<<"Ban o vi tri thu "<<no<<" - Luot: "<<turn<<endl;

	m_curRow++;
}

void CGUI::ShowQuestion(string question, string title)
{
	if (question.length() == 0)
		return;

	// Show quetion with wrapper
	GoToXY(m_curCol, m_curRow++);

	cout<<title;

	printWrappedText(m_question, title.length());

	m_curRow++;
}

void CGUI::ShowAlert(string alert, int alertmode)
{
	if (alert.length() == 0)
		return;

	m_curRow++;
	GoToXY(m_curCol, m_curRow++);

	string title;
	switch (alertmode)
	{
	case 0:
		title = "MC: ";
		break;
	case 1:
		title = "Loi: ";
		break;
	}

	cout<<title;
	printWrappedText(alert, title.length());

	m_curRow++;
}

void CGUI::ShowPlayers()
{
	GoToXY(m_curCol, m_curRow++);
	cout<<"---------- Player List ----------";
	for (int i=0; i<players.size(); i++)
	{
		GoToXY(m_curCol, m_curRow++);
		cout<<" "<<players[i].No<<". "<<players[i].NickName<<"\t"<<players[i].Score;
	}
}

string CGUI::CreateLogo()
{
	return "CHIECNONKYDIEU";
}

void CGUI::printWrappedText(string text, int indent)
{
	for (int i=0, k=0; i<text.length(); i++, k++)
	{
		cout<<text[i];
		if (k >= QUESTION_WIDTH - indent && text[i] == ' ')
		{
			GoToXY(m_curCol + indent, m_curRow++);
			k = -1;
		}
	}
}

void CGUI::GoToXY(int column, int line)
{
    // Create a COORD structure and fill in its members.
    // This specifies the new position of the cursor that we will set.
    COORD coord;
    coord.X = column;
    coord.Y = line;

    // Obtain a handle to the console screen buffer.
    // (You're just using the standard console, so you can use STD_OUTPUT_HANDLE
    // in conjunction with the GetStdHandle() to retrieve the handle.)
    // Note that because it is a standard handle, we don't need to close it.
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(hConsole, coord))
    {
        // Uh-oh! The function call failed, so you need to handle the error.
        // You can call GetLastError() to get a more specific error code.
        // ...
    }
}