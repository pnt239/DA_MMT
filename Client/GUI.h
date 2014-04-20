#pragma once
#include <string>
#include <vector>

#include "utils.h"

#define CONSOLE_COLUMN 80
#define QUESTION_WIDTH 40

class CGUI
{
public:
	CGUI(void);
	~CGUI(void);

	void ShowCell();

	std::string AskUser(std::string);
	void ShowMsg(std::string);
	void ShowGame();

	void AddPlayer(PlayerInfo);
	void RemovePlayer(int id);

	void SetQuestion(std::string);
	void SetAlert(std::string);
	void SetAnswer(int length);
private:
	void printWrappedText(std::string, int);
	char* CreateLogo();
	void GoToXY(int column, int line);

	int m_curCol;
	int m_curRow;

	std::string m_question;
	std::string m_alert;
	int m_lenAnswer;
	char* m_answer;
	bool m_isVisible;
	int m_mode; // 1-ask, 2-isPlaying, 3-isViewing

	std::vector<PlayerInfo> players;
};

