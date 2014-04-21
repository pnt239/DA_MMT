#include "GUI.h"
#include <windows.h>
#include <iostream>
using namespace std;


CGUI::CGUI(void)
{
	m_question = "Rua nao co bon muoi chan? pham ngoc thanh pham ngocj thanh pham ngoc thanh pham ngoc thanh";
	m_alert = "Cung giai ma o chu chung t ngay hom nay!";
	m_lenAnswer = 14;
	m_answer = CreateLogo();
	m_isVisible = true; //default is true
	m_mode = 1;
}


CGUI::~CGUI(void)
{
}


void CGUI::ShowCell()
{
	// Calculate First Position
	int padding = 2;
	m_curCol = (CONSOLE_COLUMN - m_lenAnswer - padding*2)/2;
	m_curRow = 1;

	// Show line
	GoToXY(m_curCol, m_curRow++);
	for (int i = 0; i < m_lenAnswer + padding*2; i++)
		cout<<(char)176;

	// Show space
	GoToXY(m_curCol, m_curRow++);
	cout<<(char)176;
	for (int i = 0; i < m_lenAnswer + padding; i++)
		cout<<' ';
	cout<<(char)176;

	// Show text
	GoToXY(m_curCol, m_curRow++);
	cout<<(char)176;
	cout<<' ';
	for (int i = 0; i < m_lenAnswer; i++)
		if (m_isVisible)
			cout<<m_answer[i];
		else
			cout<<(char)177;

	cout<<' ';
	cout<<(char)176;

	// Show space
	GoToXY(m_curCol, m_curRow++);
	cout<<(char)176;
	for (int i = 0; i < m_lenAnswer + padding; i++)
		cout<<' ';
	cout<<(char)176;

	// Show line
	GoToXY(m_curCol, m_curRow++);
	for (int i = 0; i < m_lenAnswer + padding*2; i++)
		cout<<(char)176;
	
	// Show cell table info
	m_curRow++;
	m_curCol = (CONSOLE_COLUMN - QUESTION_WIDTH) / 2;
	GoToXY(m_curCol, m_curRow++);
	if (!m_isVisible)
	{
		cout<<"O chu co "<<m_lenAnswer<<" chu cai.";
	}

	// Show quetion with wrapper
	GoToXY(m_curCol, m_curRow++);
	int lengthQues = 0;

	if (m_mode == 2)
	{
		cout<<"Cau hoi: ";
		lengthQues = 9; // 9 is length of "Cau hoi: "
	}

	printWrappedText(m_question, lengthQues);

	if (m_mode != 1)
	{
		// Not in ask mode
		m_curRow++;
		GoToXY(m_curCol, m_curRow++);
		cout<<"MC: ";
		printWrappedText(m_alert, 4);

		m_curRow++;
		GoToXY(m_curCol, m_curRow++);
	}
}

string CGUI::AskUser(string question)
{
	m_mode = 1;
	m_question = question;

	ShowCell();

	string res;
	cin>>res;
	return res;
}

void CGUI::ShowMsg(string message)
{
	GoToXY(m_curCol, m_curRow++);
	cout<<"\t-> "<<message<<endl;
}

void CGUI::ShowGame()
{
	string c;
	string s;
	int ansRow;
	m_mode = 2;
	m_isVisible = false;

	ShowCell();
	
	ansRow = m_curRow;

	m_curRow += 3;
	GoToXY(m_curCol, m_curRow++);
	cout<<"---------- Player List ----------";
	for (int i=0; i<players.size(); i++)
	{
		GoToXY(m_curCol, m_curRow++);
		cout<<" "<<players[i].No<<". "<<players[i].NickName<<"\t"<<players[i].Score;
	}

	if (m_mode == 2) // is playing
	{
		GoToXY(m_curCol, ansRow);
		cout<<"Ban doan chu: ";
		cin>>c;

		GoToXY(m_curCol, ansRow+1);
		cout<<"Cum tu doan luon: ";
		cin>>s;
	}
}

void CGUI::AddPlayer(PlayerInfo player)
{
	players.push_back(player);
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

void CGUI::SetAnswer(int length)
{
	if (m_answer != NULL)
		delete m_answer;

	m_lenAnswer = length;
	m_answer = new char[m_lenAnswer];
	for (int i=0; i<m_lenAnswer; i++)
		m_answer[i] = ' ';
}

char* CGUI::CreateLogo()
{
	char* ret = new char[14];

	ret[0] = 'C';
	ret[1] = 'H';
	ret[2] = 'I';
	ret[3] = 'E';
	ret[4] = 'C';
	ret[5] = 'N';
	ret[6] = 'O';
	ret[7] = 'N';
	ret[8] = 'K';
	ret[9] = 'Y';
	ret[10] = 'D';
	ret[11] = 'I';
	ret[12] = 'E';
	ret[13] = 'U';

	return ret;
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