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

	std::string ShowAsk(std::string);
	void ShowInfo(std::string);
	void ShowGame(int, int);

	void AddPlayer(PlayerInfo);
	void RemovePlayer(int id);

	void SetQuestion(std::string);
	void SetAlert(std::string);
	void SetAnswer(std::string);
	void SetAnswer(int, char);

	std::string GetAnswer() { return m_answer; }
	std::string GetGuessChar();
	std::string GetGuessString();
private:
	std::string AskUser(std::string, int);

	void ShowCell(bool);
	void ShowOrder(int, int);
	void ShowQuestion(std::string, std::string title = "Cau hoi: ");
	// alertmode: 0-info 1-error
	void ShowAlert(std::string, int alertmode = 0);
	void ShowPlayers();

	void printWrappedText(std::string, int);
	std::string CreateLogo();
	void GoToXY(int column, int line);

	int m_curCol;
	int m_curRow;
	int m_ansRow;

	std::string m_question;
	std::string m_alert;
	std::string m_answer;
	std::string m_guesschar;
	std::string m_guessstr;

	int m_mode; // 1-ask, 2-isPlaying, 3-isViewing

	std::vector<PlayerInfo> players;
};

