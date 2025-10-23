#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <cctype>
#include <fstream>
#include "InputsLib.h"
#include "UtilsLib.h"
#include "StringLib.h"
using namespace std;

const string FileName = "Clients.txt";

enum enATMMainMenueOptions {
	eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3,
	eCheckBalance = 4, Logout = 5
};

void MainMenueScreen();
void LogInScreen();
void QuickWithdrawScreen();
void NormalWithdrawScreen();

struct sClient
{
	string AccNumber = "", PinCode = "", Name = "", Phone = "";
	float AccBalance = 0;
	bool MarkForDelete = false;
};
sClient Client;
sClient ConvertLineToRecord(string Line, string Delim)
{
	sClient Client;
	vector<string>vClientRecord;
	vClientRecord = StringLib::SplitString(Line, Delim);

	Client.AccNumber = vClientRecord[0];
	Client.PinCode = vClientRecord[1];
	Client.Name = vClientRecord[2];
	Client.Phone = vClientRecord[3];
	Client.AccBalance = stof(vClientRecord[4]);

	return Client;
}
string ConvertRecordToLine(sClient Client, string Delim)
{
	string stClientRecord = "";

	stClientRecord += Client.AccNumber + Delim;
	stClientRecord += Client.PinCode + Delim;
	stClientRecord += Client.Name + Delim;
	stClientRecord += Client.Phone + Delim;
	stClientRecord += to_string(Client.AccBalance);

	return stClientRecord;
}
vector <sClient> LoadClientDataFromFile()
{
	fstream StringFile;
	vector <sClient> vClients;

	StringFile.open(FileName, ios::in);

	if (StringFile.is_open())
	{
		sClient Client;
		string Line;

		while (getline(StringFile, Line))
		{
			Client = ConvertLineToRecord(Line, "#//#");
			vClients.push_back(Client);
		}
		StringFile.close();
	}
	return vClients;
}
vector < sClient > SaveClientsDataToFile(vector<sClient>vClients)
{
	fstream MyFile;
	string DataLine;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (sClient& C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C, "#//#");
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to back to main menue... ";
	system("pause>0");
	MainMenueScreen();
}
bool FindClientWithAccNumber(string AccountNumber, vector <sClient> vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
bool DepositBalanceToClientByAccNumber(vector <sClient>& vClients, float Amount)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want Perform This Transaction ? Y/N : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccNumber == Client.AccNumber)
			{
				C.AccBalance += Amount;
				SaveClientsDataToFile(vClients);
				cout << "\n\nDone successfully , New balance is : " << C.AccBalance << endl;

				return true;
			}
		}
	}
	return false;
}

short ReadQuickWithdrawOptions()
{
	short Answer = 0;
	while (Answer < 1 || Answer>9) {
		cout << "\nChoose what to Witharaw ? [1 to 9] : ";
		cin >> Answer;
	}

	return Answer;
}
int ReadWithdrawAmount()
{
	int Amount = 0;
	do {
		cout << "\nEnter an amount multiple of 5's : ";
		cin >> Amount;
	} while (Amount % 5 != 0);

	return Amount;
}
int ReadDepositAmount()
{
	int Amount = 0;
	do {
		cout << "\nEnter a positive amount : ";
		cin >> Amount;
	} while (Amount < 0);

	return Amount;
}
short GetQuickWithdrawAmount(short QuickWithdrawOption)
{
	switch (QuickWithdrawOption)
	{
	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}
void PerformQuickWithdrawOptions(short QuickWithdrawOption)
{
	if (QuickWithdrawOption == 9)
		return;

	short WithdrawBalance = GetQuickWithdrawAmount(QuickWithdrawOption);

	if (WithdrawBalance > Client.AccBalance)
	{
		cout << "\nThe amount exceeds your balance,Make another choice.\n";
		cout << "Press any key to continue ...";
		system("pause>0");
		QuickWithdrawScreen();
		return;
	}
	vector <sClient> vClients;
	vClients = LoadClientDataFromFile();
	DepositBalanceToClientByAccNumber(vClients, WithdrawBalance * -1);
	Client.AccBalance -= WithdrawBalance;

}
void PerformNormalWithdrawOption()
{
	int WithdrawBalance = ReadWithdrawAmount();
	if (WithdrawBalance > Client.AccBalance)
	{
		cout << "\nAmount exceeds the balance , You can withdraw up to : " << Client.AccBalance << endl;
		cout << "Press any key to continue ...";
		system("pause>0");
		NormalWithdrawScreen();
		return;
	}
	vector <sClient> vClients = LoadClientDataFromFile();
	DepositBalanceToClientByAccNumber(vClients, WithdrawBalance * -1);
	Client.AccBalance -= WithdrawBalance;
}
void PerformDepositOption()
{
	double DepositAmount = ReadDepositAmount();

	vector <sClient> vClients = LoadClientDataFromFile();
	DepositBalanceToClientByAccNumber(vClients, DepositAmount);
	Client.AccBalance += DepositAmount;

}

void QuickWithdrawScreen()
{
	system("cls");
	cout << "===============================================\n";
	cout << "\t\tQuick Withdraw\n";
	cout << "===============================================\n";
	cout << "\t[1] 20.\t" << "\t[2] 50\n";
	cout << "\t[3] 100\t" << "\t[4] 200\n";
	cout << "\t[5] 400\t" << "\t[6] 600\n";
	cout << "\t[7] 800\t" << "\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "===============================================\n";
	cout << "\nYour Balance is " << Client.AccBalance << endl;

	PerformQuickWithdrawOptions(ReadQuickWithdrawOptions());

}
void DepositScreen()
{
	cout << "-----------------------------------------------\n";
	cout << "\t\tDeposit Screen\n";
	cout << "-----------------------------------------------\n";

	PerformDepositOption();
}
void NormalWithdrawScreen()
{
	cout << "-----------------------------------------------\n";
	cout << "\t\tNormal Withdraw Screen\n";
	cout << "-----------------------------------------------\n";

	PerformNormalWithdrawOption();
}
void CheckBalanceScreen()
{
	cout << "-----------------------------------------------\n";
	cout << "\t\tCheck Balance Screen\n";
	cout << "-----------------------------------------------\n";
	cout << "\nYour Balance is " << Client.AccBalance << endl;
}

short ReadMainMenueOptions()
{
	short Answer;
	cout << "\nChoose what do you want to do? [1 to 5] : ";
	cin >> Answer;

	return Answer;
}
void PerformMainMenueOptions(enATMMainMenueOptions MainMenueOptions)
{
	switch (MainMenueOptions)
	{
	case enATMMainMenueOptions::eQuickWithdraw:
		system("cls");
		QuickWithdrawScreen();
		GoBackToMainMenue();
		break;

	case enATMMainMenueOptions::eNormalWithdraw:
		system("cls");
		NormalWithdrawScreen();
		GoBackToMainMenue();
		break;

	case enATMMainMenueOptions::eDeposit:
		system("cls");
		DepositScreen();
		GoBackToMainMenue();
		break;

	case enATMMainMenueOptions::eCheckBalance:
		system("cls");
		CheckBalanceScreen();
		GoBackToMainMenue();
		break;

	case enATMMainMenueOptions::Logout:
		system("cls");
		LogInScreen();
		break;
	}
}

void MainMenueScreen()
{
	system("cls");
	cout << "===============================================\n";
	cout << "\t\tATM Main Menue Screen \n";
	cout << "===============================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "===============================================\n";

	PerformMainMenueOptions((enATMMainMenueOptions)ReadMainMenueOptions());
}

bool FindClientWithAccNumberAndPinCode(string AccountNumber, string PinCode, sClient& CurrentClient)
{
	vector <sClient> vClients = LoadClientDataFromFile();
	for (sClient& C : vClients)
	{
		if (C.AccNumber == AccountNumber && C.PinCode == PinCode)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientWithAccNumberAndPinCode(AccountNumber, PinCode, Client))
		return true;
	else
		return false;
}

void LogInScreen()
{

	string AccountNumber, PinCode;
	bool LoginFailed = false;

	do {
		system("cls");

		cout << "-----------------------------------------------\n";
		cout << "\t\tLogin Screen\n";
		cout << "-----------------------------------------------\n";

		if (LoginFailed)
			cout << "Invalid Account Number/PinCode!\n\n";

		cout << "Enter Account Number : ";
		getline(cin >> ws, AccountNumber);
		cout << "Enter PinCode : ";
		getline(cin, PinCode);

		LoginFailed = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFailed);

	MainMenueScreen();
}

int main()
{
	LogInScreen();

	system("pause>0");
	return 0;
}

