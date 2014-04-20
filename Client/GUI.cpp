#include "GUI.h"
#include <iostream>
using namespace std;


CGUI::CGUI(void)
{
}


CGUI::~CGUI(void)
{
}

string CGUI::AskUser(string question)
{
	string res;
	cout<<question;
	cin>>res;
	return res;
}

void CGUI::ShowMsg(string message)
{
	cout<<"\t-> "<<message<<endl;
}