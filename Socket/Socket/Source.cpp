#include <iostream>
#include <WinSock2.h>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
//#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
//#endif
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>                     // MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT

#define REG_SUCC 13
#define REG_DUP 12

#define GAME_WIN 20
#define GAME_LOSE 21

using namespace std;

void main()
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		wVersionRequested = MAKEWORD(2, 2);

		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0) {
			printf("Khong the khoi tao thu vien!\n");
			exit(-1);
		}

		SOCKET sockServer = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in serverAdd;
		serverAdd.sin_family = AF_INET;
		serverAdd.sin_port = htons(30000);
		serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);
		bind(sockServer, (sockaddr*)&serverAdd, sizeof(serverAdd));
		listen(sockServer, 5);

		SOCKET sockClient[3];

		string Nickname[3];
		for (int i = 0; i<3; i++)
		{
			sockClient[i] = accept(sockServer, NULL, NULL);
			printf("Da tiep nhan client %d/%d\n", i + 1, 3);
		}
		for (int i = 0; i < 3; i++)
		{
			int len;
			recv(sockClient[i], (char*)&i, sizeof(int), 0);
			recv(sockClient[i], (char*)&Nickname[i], len, 0);
			bool flag = true;
			int code;

			for (int j = i; j < 3; j++)
			{
				if (Nickname[i] == Nickname[j])
				{
					code = REG_DUP;
					send(sockClient[i], (char*)&code, sizeof(int), 0);
				}
			}

			if (code == 12)
			{
				printf("Client thu %d dang ki da dang ki trung Nickname", i);
				i--;
				continue;
			}
			else
			{
				printf("Client thu %d dang ki da dang ki thanh cong voi Nickname: %s", i,Nickname[i]);
				code = REG_SUCC;
				send(sockClient[i], (char*)&code, sizeof(int), 0);

				//Gửi số thứ tự đăng kí đến
				send(sockClient[i], (char*)&i, sizeof(int), 0);
			}
		}

		int Score[3];
		for (int i = 0; i < 3; i++)
		{
			Score[i] = 0;
			//Gửi điểm ban đầu đến từng người
			send(sockClient[i], (char*)&Score[i], sizeof(int), 0);
		}


		ifstream ReadDB;
		ReadDB.open("database.txt", ios::in);
		int Qnum;
		ReadDB >> Qnum;
		vector<string> DapAn;
		vector<string> GoiY;
		for (int i = 0; i < Qnum; i++)
		{
			ReadDB >> DapAn[i];
			ReadDB >> GoiY[i];
		}

		srand(time(NULL));
		int q = rand() % (Qnum);

		int Len_Question;
		Len_Question = DapAn[q].length();
		int Len_GoiY;
		Len_GoiY = GoiY[q].length();

		char* DA;
		strcpy(DA, DapAn[q].c_str());
		char* GY;
		strcpy(GY, GoiY[q].c_str());

		for (int i = 0; i < 3; i++)
		{
			printf("Gui cau hoi va goi y toi client thu: %d \n", i);
			send(sockClient[i], (char*)&Len_Question, sizeof(int), 0);

			send(sockClient[i], (char*)&Len_GoiY, sizeof(int), 0);
			send(sockClient[i], GY, Len_GoiY, 0);
		}

		int turn = 1;
		char*  GueassCharacter;
		char* GueassWords;
		int Len_gueasswords;
		bool Play[3];
		for (int i = 0; i < 3; i++)
		{
			Play[i] == true;
		}
		int pos = 0;
		while (turn <= 5)
		{
			
			if (Play[pos] == true) //Neu Nguoi choi i van con duoc phep choi
			{
				recv(sockClient[pos], GueassCharacter, sizeof(char), 0);

				//Kiem tra cum tu doan
				recv(sockClient[pos], (char*)&Len_gueasswords, sizeof(int), 0);
				if (Len_gueasswords != 0)
				{
					recv(sockClient[pos], GueassWords, Len_gueasswords, 0);
					if (turn > 2)
					{
						if (strcmp(DapAn[q].c_str(), GueassWords) == 0)
						{
							int code = GAME_WIN;
							send(sockClient[pos], (char*)&code, sizeof(int), 0);
							Score[pos] += 5;
							break;
						}
						else
						{
							int code = GAME_LOSE;
							send(sockClient[pos], (char*)&code, sizeof(int), 0);
							Play[pos] == false;
						}
					}
				}




			}
			
		}
	}
}