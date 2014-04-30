#include <iostream>
#include <WinSock2.h>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <ctime>
#include <algorithm>

#include "Network.h"
#include "utils.h"

#define REG_SUCC 13
#define REG_DUP 12

#define GAME_WIN 20
#define GAME_LOSE 21
#define GAME_END 22
#define GAME_CON 23
#define GAME_TURNOUT 24
#define GAME_RESIGN 25

#define GAME_GSS_CHR 30
#define GAME_GSS_STR 31

#define PORT_APP 30000
#define MAX_CLIENT 10

using namespace std;

void InitClient(ClientInfo clients[], int n)
{
	for (int i = 0; i < n; i++)
		clients[i].Socket = INVALID_SOCKET;
}

void SendPlayerList(CNetwork* network, ClientInfo clients[], int nClient)
{
	for (int i = 0; i < nClient; i++)
	{
		network->Send(clients[i].Socket, nClient);
		for (int j = 0; j < nClient; j++)
			network->Send(clients[i].Socket, clients[j]);
	}
}

void ConnectClients(CNetwork* network, ClientInfo clients[], int nClient)
{
	// Chap nhan ket noi tu client
	for (int i = 0; i<nClient; i++)
	{
		if (clients[i].Socket == INVALID_SOCKET)
		{
			// Them client vao danh sach
			clients[i].Socket = network->Accept();
			cout<<"Da tiep nhan client "<<i + 1<<"/"<<nClient<<"\n";
		}

		// Nhan nickname
		string nick = network->ReadString(clients[i].Socket );

		int code = REG_SUCC;

		// Kiem tra co trung nickname khong
		for (int j = 0; j < nClient; j++)
			if (nick == clients[j].NickName)
			{
				code = REG_DUP;
				break;
			}

		// Thong bao cho client
		network->Send(clients[i].Socket, code);

		if (code == REG_DUP)
		{
			// Neu trung
			cout<<"Client thu "<< i <<" dang ki da dang ki trung nickname"<<endl;
			i--;
		}
		else
		{
			clients[i].No = i;
			clients[i].NickName = nick;
			clients[i].Score = 0;
			clients[i].Enable = true;

			cout<<"Client thu " << i << " dang ki da dang ki thanh cong voi nickname: " << clients[i].NickName << endl;

			// Gui so thu tu cua nguoi dang ky
			network->Send(clients[i].Socket, i);
		}
	}
}

bool ReadDatabase(string filename, vector<string>& answ, vector<string>& hints)
{
	int nQues;
	ifstream Database(filename, ios::in);
	if (!Database.is_open())
	{
		cout<<"Khong the mo file du lieu ^^"<<endl;
		return false;
	}

	Database >> nQues;
	for (int i = 0; i < nQues/2; i++)
	{
		string ans, hint;
		Database.ignore();
		getline(Database, ans);
		getline(Database, hint);

		// Chuyen Answer ve upper
		transform(ans.begin(), ans.end(), ans.begin(), toupper);

		answ.push_back(ans);
		hints.push_back(hint);
	}
	Database.close();
	return true;
}

int GetRandomQuestion(int nques)
{
	srand(time(NULL));
	return rand() % (nques);
}

void SendAll(CNetwork* network, ClientInfo clients[], int nClient, int info, int except = -1)
{
	for (int i = 0; i < nClient; i++)
		if (i != except)
			network->Send(clients[i].Socket, info);
}

void SendAll(CNetwork* network, ClientInfo clients[], int nClient, string info, int except = -1)
{
	for (int i = 0; i < nClient; i++)
		if (i != except)
			network->Send(clients[i].Socket, info);
}

void Refresh(ClientInfo clients[], int nClient)
{
	for (int i = 0; i < nClient; i++)
		clients[i].Enable = true;
}

void main()
{
	int iConnection = 2;
	int iNotPlay = 0;
	ClientInfo Clients[MAX_CLIENT];
	vector<string> Answers;
	vector<string> Hints;

	// Nhap so luong ket noi
	cout<<"Nhap so nguoi co the tham gia choi: ";
	cin>>iConnection;

	if (iConnection < 2) iConnection = 1;
	if (iConnection > 10) iConnection = 10;

	// Tao doi tuong network
	CNetwork* network = new CNetwork();
	// Tao danh sach client
	InitClient(Clients, MAX_CLIENT);

	// Lang nghe cac ket noi
	network->Listen(PORT_APP);

	// Thiet lap ket noi cac client
	ConnectClients(network, Clients, iConnection);

	// Gui danh sach nguoi choi
	SendPlayerList(network, Clients, iConnection);

	/* Doc DB */
	if (!ReadDatabase("database.txt", Answers, Hints))
		return;

	while (true)
	{
		// Lay cau hoi
		int q = GetRandomQuestion(Answers.size());
		string Answer = Answers[q];
		vector<char> OpenedChars;


		// SendAll do dai dap an va chuoi goi y
		for (int i = 0; i < iConnection; i++)
		{
			cout<<"Gui cau hoi va goi y toi client thu: "<<i<<"\n";
			network->Send(Clients[i].Socket, Answer.length());
			network->Send(Clients[i].Socket, Hints[q]);
		}

		//Bat dau vong choi

		int turn = 1;				// Luot ban dau la 1
		int m_opencount = 0;		// So o chu da mo
		int gameevent = 0;
		string GuessCharacter;		// Chua chu cai doan
		char GuessChar;
		string GuessString;			// Chua cum tu doan

		int pos = 0;				// Luot choi ban dau cho nguoi thu 0
		while (true)
		{
			// Bat dau luot choi
			// Đã quá số người chơi thì quay trở lại người chơi đầu tiên và turn tăng 1
			if (pos >= iConnection)
			{
				pos = 0;
				turn++;
				continue;
			}
			else if (Clients[pos].Enable == true) // Neu nguoi choi pos van con duoc phep choi
			{
				cout<<"Luot choi thu " << turn << ", nguoi choi thu " << pos << "\n";
				SendAll(network, Clients, iConnection, pos);

				// SendAll luot choi cua nguoi hien tai
				SendAll(network, Clients, iConnection, turn);
				// Kiem tra luot choi
				if (turn > 5)
				{
					SendAll(network, Clients, iConnection, gameevent = GAME_TURNOUT);
					break;
				}
				else
				{
					SendAll(network, Clients, iConnection, GAME_CON);
				}

				GuessCharacter = network->ReadString(Clients[pos].Socket); // Nhan chu cai doan
				GuessChar = toupper(GuessCharacter[0]);
				GuessString = network->ReadString(Clients[pos].Socket);  // Nhan cum tu doan
				transform(GuessString.begin(), GuessString.end(), GuessString.begin(), toupper);

				if (GuessCharacter == "" && GuessString == "")
				{
					// Kiem tra neu nguoi choi da roi cuoc choi
					iNotPlay++;
					Clients[pos].Enable = false;
					Clients[pos].NickName = "";
					Clients[pos].No = -1;
					Clients[pos].Score = 0;
					Clients[pos].Socket = INVALID_SOCKET;
					SendAll(network, Clients, iConnection, GAME_GSS_STR, pos);
					SendAll(network, Clients, iConnection, gameevent = GAME_RESIGN, pos);
					continue;
				}

				if (GuessString.length() != 0)  // Neu len !=0 thi nhan cum tu doan
				{
					if (turn > 2) // Neu qua 2 turn roi moi bat dau kiem tra
					{
						SendAll(network, Clients, iConnection, GAME_GSS_STR, pos);

						if (Answer == GuessString)		// Kiem tra cum tu doan
						{
							SendAll(network, Clients, iConnection, gameevent = GAME_WIN);

							cout << "Nguoi choi thu " << pos << " da doan dung cum tu khoa la : " << Answer << endl;
							Clients[pos].Score += 5;
							break;
						}
						else
						{
							SendAll(network, Clients, iConnection, GAME_LOSE);
							
							cout << "Nguoi choi thu " << pos << " da doan sai cum tu khoa va bi loai khoi tro choi: " << GuessString << endl;
							Clients[pos].Enable = false; // Loai bo nguoi choi ra khoi danh sach choi
							iNotPlay++;
							pos++; // Chuyen sang nguoi choi moi
							continue; // Quay lai vong lap voi nguoi choi moi
						}
					}
				}

				SendAll(network, Clients, iConnection, GAME_GSS_CHR, pos);

				int count = 0;	// Bien dem so luong chu cai dung
				int _p[30];		// Mang luu vi tri chu cai dung

				for (int i = 0; i < Answer.length(); i++)
					if (Answer[i] == GuessChar)
						_p[count++] = i;

				// Kiem tra o chu da duoc doan chua
				if (OpenedChars.size() != 0 && find(OpenedChars.begin(), OpenedChars.end(), GuessChar) !=  OpenedChars.end())
					// Da duoc doan
					count = -1; 
				else
					// Chua lat
					// Tang so luong o chu da mo
					m_opencount += count;

				// SendAll so chu doan dung
				for (int i = 0; i < iConnection; i++)
				{
					network->Send(Clients[i].Socket, GuessChar);	// Gui chu cai da doan
					network->Send(Clients[i].Socket, count);	// Gui so luong

					for (int j = 0; j < count; j++)				//gui toi tung client vi tri cua chu cai do
						network->Send(Clients[i].Socket, _p[j]);
				}

				// Cong diem
				if (count > 0)
				{
					OpenedChars.push_back(GuessChar);
					Clients[pos].Score += 1;
					cout << "Nguoi choi thu " << pos << " da doan dung chu cai " << GuessChar << endl;
					cout << "Co " << count << " chu cai trong cum tu can tim"<< endl;
				}
				else if (count <= 0)
				{
					cout << "Khong co chu cai " << GuessChar << " trong cum tu can tim" << endl;
					pos++;	// Doan sai, chuyen quyen cho nguoi choi ke
					// Gui so thu tu nguoi choi ke
					SendAll(network, Clients, iConnection, pos%iConnection);
				}

				// Da lat het tat ca cac o chu
				if (m_opencount == Answer.length())
				{
					// Chien thang
					SendAll(network, Clients, iConnection, gameevent = GAME_WIN);
					break;
				}
				else
				{
					// Tiep tuc choi
					SendAll(network, Clients, iConnection, GAME_CON);
				}

				// Update current player's score
				if (count > 0)
					SendAll(network, Clients, iConnection, Clients[pos].Score);
			}
			else if (iNotPlay >= iConnection)
			{
				// Khong con ai de choi vi da het luot
				SendAll(network, Clients, iConnection, pos);
				SendAll(network, Clients, iConnection, turn);
				SendAll(network, Clients, iConnection, gameevent = GAME_END);
				break;
			}
			else 
			{
				pos++;
			}
		} //Thoat khoi vong while sau 5 turn hoac gap cau lenh break o doan dung cum tu

		if (gameevent != GAME_END && gameevent != GAME_TURNOUT)
		{
			SendAll(network, Clients, iConnection, Clients[pos].Score);
			SendAll(network, Clients, iConnection, Answer);
		}

		cout << "Tro choi ket thuc, diem tong ket chung cuoc" << endl;
		for (int i = 0; i < iConnection; i++)
			cout << "Diem nguoi choi thu" << i << " Nickname: " << Clients[i].NickName << "la: " << Clients[i].Score << endl;

		int check;
		cout<<"Tiep tuc tao tro choi? (1-yes, #-no)";
		cin>>check;
		if (check != 1)
		{
			break;
		}
		// Refresh danh sach  nguoi choi
		Refresh(Clients, iConnection);
	}

	delete network;
}