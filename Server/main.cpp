#include <iostream>
#include <WinSock2.h>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <ctime>

#include "Network.h"
#include "utils.h"

#define REG_SUCC 13
#define REG_DUP 12

#define GAME_WIN 20
#define GAME_LOSE 21
#define GAME_END 22

#define PORT_APP 30000

using namespace std;

#define MAX_CLIENT 10

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

		if (clients[i].NickName != "")
			continue;

		// Nhan nickname
		string nick = network->ReadString(clients[i].Socket );

		int code;

		// Kiem tra co trung nickname khong
		for (int j = i; j < nClient; j++)
			if (nick == clients[j].NickName)
			{
				code = REG_DUP;
				break;
			}
		
		if (code == REG_DUP)
		{
			// Neu trung
			cout<<"Client thu "<< i <<" dang ki da dang ki trung nickname";

			// Thong bao cho client da trung
			network->Send(clients[i].Socket, code);
		}
		else
		{
			clients[i].No = i;
			clients[i].NickName = nick;
			clients[i].Score = 0;
			clients[i].Enable = true;

			cout<<"Client thu " << i << " dang ki da dang ki thanh cong voi nickname: " << clients[i].NickName;

			// Thong bao cho client da thanh cong
			code = REG_SUCC;
			network->Send(clients[i].Socket, code);

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
		getline(Database, ans);
		getline(Database, hint);
		
		answ.push_back(ans);
		hints.push_back(hint);
	}

	return true;
}

int GetRandomQuestion(int nques)
{
	srand(time(NULL));
	return rand() % (nques);
}

void main()
{
	int iConnection = 2;
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


		//Gui do dai dap an va chuoi goi y toi nguoi choi
		for (int i = 0; i < iConnection; i++)
		{
			cout<<"Gui cau hoi va goi y toi client thu: "<<i<<" \n";
			network->Send(Clients[i].Socket, Answer.length());
			network->Send(Clients[i].Socket, Hints[q]);
		}

		//Bat dau vong choi

		int turn = 1;				// Luot ban dau la 1
		string GuessCharacter;		// Chua chu cai doan
		string GuessString;			// Chua cum tu doan

		int pos = 0;				// Luot choi ban dau cho nguoi thu 0
		while (turn <= 5)
		{
			// Bat dau luot choi
			if (pos >= iConnection)		//Đã quá số người chơi thì quay trở lại người chơi đầu tiên và turn tăng 1
			{
				pos = 0;
				turn++;
				continue;
			}
			else
			if (Clients[pos].Enable == true) // Neu nguoi choi pos van con duoc phep choi
			{
				cout<<"Luot choi thu " << turn << ", nguoi choi thu " << pos << " \n";
				for (int i = 0; i < iConnection; i++)
				{
					network->Send(Clients[i].Socket, pos);
				}

				GuessCharacter = network->ReadString(Clients[pos].Socket); // Nhan chu cai doan
				GuessString = network->ReadString(Clients[pos].Socket);  // Nhan cum tu doan

				if (GuessString.length() != 0)  // Neu len !=0 thi nhan cum tu doan
				{
					if (turn > 2) // Neu qua 2 turn roi moi bat dau kiem tra
					{
						if (Answer == GuessString)		// Kiem tra cum tu doan
						{
							int code = GAME_WIN;
							for (int i = 0; i < iConnection; i++)
							{
								network->Send(Clients[i].Socket, code); // Gui ma code thang toi all
							}
							cout << "Nguoi choi thu " << pos << " da doan dung cum tu khoa la : " << Answer << endl;
							Clients[pos].Score += 5;
							break;
						}
						else
						{
							int code = GAME_LOSE;
							for (int i = 0; i < 3; i++)
							{
								network->Send(Clients[i].Socket, code); // Gui ma code thang toi all
							}
							cout << "Nguoi choi thu " << pos << " da doan sai cum tu khoa va bi loai khoi tro choi: " << GuessString << endl;
							Clients[pos].Enable = false; // Loai bo nguoi choi ra khoi danh sach choi
							pos++; // Chuyen sang nguoi choi moi
							continue; // Quay lai vong lap voi nguoi choi moi
						}
					}
				}

				int count = 0;	// Bien dem so luong chu cai dung
				int _p[30];		// Mang luu vi tri chu cai dung

				for (int i = 0; i < Answer.length(); i++)
					if (Answer[i] == GuessCharacter[0])
						_p[count++] = i;

				for (int i = 0; i < iConnection; i++)			// Gui all so chu doan dung
				{
					network->Send(Clients[i].Socket, count);	// Gui so luong
					for (int j = 0; j < count; j++)				//gui toi tung client vi tri cua chu cai do
						network->Send(Clients[i].Socket, _p[j]);
				}

				// Cong diem
				if (count != 0)
				{
					Clients[pos].Score += 1;
					cout << "Nguoi choi thu " << pos << " da doan dung chu cai " << GuessCharacter << endl;
					cout << "Co " << count << " chu cai trong cum tu can tim";
				}
				else if (count == 0)
				{
					cout << "Khong co chu cai " << GuessCharacter << " trong cum tu can tim" << endl;
					pos++;	// Doan sai, chuyen quyen cho nguoi choi ke
				}

			}			
		} //Thoat khoi vong while sau 5 turn hoac gap cau lenh break o doan dung cum tu

		int code = GAME_END;
		cout << "Tro choi ket thuc, diem tong ket chung cuoc" << endl;
		for (int i = 0; i < 3; i++)
		{
			cout << "Diem nguoi choi thu" << i << " Nickname: " << Clients[i].NickName << "la: " << Clients[i].Score << endl;
			network->Send(Clients[i].Socket, code);
			network->Send(Clients[i].Socket, Clients[i].Score);
		}

		int check;
		cout<<"Tiep tuc tao tro choi? (1-yes, #-no)";
		cin>>check;
		if (check != 1)
		{
			break;
		}
	}

	delete network;
}