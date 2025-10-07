#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <cctype>
#include <fstream>
#include "ArraysLib.h"
#include "MathLib.h"
#include "GenerateLib.h"
#include "InputsLib.h"
#include "StringLib.h"
using namespace std;
const string FileName = "Clients.txt";

enum enMainMenueOptions { ShowClientlist = 1, AddClient = 2, DeleteClient = 3, Updateinfo = 4, FindClient = 5, Transaction = 6, Exit = 7 };

enum enTransactionOptions { eDeposit = 1, eWithdraw = 2, eTotalBalance = 3, eMainMenue = 4 };

struct sClient
{
	string AccNumber = "", PinCode = "", Name = "", Phone = "";
	float AccBalance = 0;
	bool MarkForDelete = false;
};

void MainMenueScreen(vector <sClient>& vClients);
void TransactionScreen(vector <sClient>& vClients);

sClient ChangeClientData(string AccountNumber)
{
	sClient Client;

	Client.AccNumber = AccountNumber;

	cout << "Enter Pin Code : ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Your Name : ";
	getline(cin, Client.Name);

	cout << "Enter Phone Number : ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance : ";
	cin >> Client.AccBalance;

	return Client;
}

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

void PrintClientData(sClient Client)
{
	cout << "The Following is Client details : \n\n";
	cout << "-----------------------------------------------\n";
	cout << "Account Number : " << Client.AccNumber << endl;
	cout << "PIN code : " << Client.PinCode << endl;
	cout << "Name : " << Client.Name << endl;
	cout << "Phone number : " << Client.Phone << endl;
	cout << "Account balance : " << Client.AccBalance << endl;
	cout << "-----------------------------------------------\n";
}

void PrintClientRecord(sClient Client)
{
	cout << "|" << left << setw(15) << Client.AccNumber;
	cout << "|" << left << setw(10) << Client.PinCode;
	cout << "|" << left << setw(30) << Client.Name;
	cout << "|" << left << setw(20) << Client.Phone;
	cout << "|" << left << setw(20) << Client.AccBalance;
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

bool FindClientWithAccNumber(sClient& Client, string AccountNumber, vector <sClient> vClients)
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

sClient AddNewClient(vector <sClient>& vClients)
{
	sClient Client;

	cout << "Enter Account Number : ";
	getline(cin >> ws, Client.AccNumber);

	while ((FindClientWithAccNumber(Client, Client.AccNumber, vClients)))
	{
		cout << "Client with [" << Client.AccNumber << "] already exist , Enter another Account Number : ";
		getline(cin >> ws, Client.AccNumber);
	}

	cout << "Enter Pin Code : ";
	getline(cin, Client.PinCode);

	cout << "Enter Your Name : ";
	getline(cin, Client.Name);

	cout << "Enter Phone Number : ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance : ";
	cin >> Client.AccBalance;


	return Client;
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

void ClientListScreen(vector <sClient> vClients)
{
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s)";
	cout << "\n______________________________________________________________________________________________________________\n" << endl;

	cout << "|" << left << setw(15) << "Account Number";
	cout << "|" << left << setw(10) << "PIN code";
	cout << "|" << left << setw(30) << "Name";
	cout << "|" << left << setw(20) << "Phone";
	cout << "|" << left << setw(20) << "Account Balance";
	cout << "\n______________________________________________________________________________________________________________\n" << endl;


	for (sClient Client : vClients)
	{
		PrintClientRecord(Client);
		cout << endl;
	}
	cout << "\n______________________________________________________________________________________________________________\n" << endl;

}

bool MarkClientForDeleteByAccNumber(string AccountNumber, vector<sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool UpdateClientDataByAccNumber(vector <sClient>& vClients, string AccountNumber)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientWithAccNumber(Client, AccountNumber, vClients))
	{
		PrintClientData(Client);
		cout << "\n\nAre you sure you want to Update this client ? Y/N : ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			for (sClient& C : vClients)
			{
				if (C.AccNumber == AccountNumber)
				{
					C = ChangeClientData(AccountNumber);
					break;
				}
			}

			SaveClientsDataToFile(vClients);
			cout << "\n\nClient Updated successfully.";
			return true;
		}
	}
	else
	{
		cout << "\n\nClient with Account Number (" << AccountNumber << ") not found.";
		return false;
	}
}

bool DeleteClientDataByAccNumber(vector <sClient>& vClients, string AccountNumber)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientWithAccNumber(Client, AccountNumber, vClients))
	{
		PrintClientData(Client);
		cout << "\n\nAre you sure you want to delete this client ? Y/N : ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			MarkClientForDeleteByAccNumber(AccountNumber, vClients);
			SaveClientsDataToFile(vClients);

			vClients = LoadClientDataFromFile();
			cout << "\n\nClient Deleted successfully.";
			return true;
		}
	}
	else
	{
		cout << "\n\nClient with Account Number (" << AccountNumber << ") not found.";
		return false;
	}
}

void AddClientsScreen(vector<sClient>vClients)
{
	sClient Client;
	char Answer = 'n';
	system("cls");
	cout << "-----------------------------------------------\n";
	cout << "\t\tAdd New Clients Screen\n";
	cout << "-----------------------------------------------\n";

	do
	{
		cout << "\nAdd new client :-\n\n";
		Client = AddNewClient(vClients);
		vClients.push_back(Client);
		SaveClientsDataToFile(vClients);

		cout << "Client Added successfully , do you want to add more clients ? Y/N :";
		cin >> Answer;

	} while (Answer == 'Y' || Answer == 'y');

}

void DeleteClientScreen(vector <sClient>& vClients)
{
	sClient Client;

	cout << "-----------------------------------------------\n";
	cout << "\t\tDelete Client Screen\n";
	cout << "-----------------------------------------------\n";
	string AccountNumber = InputsLib::ReadString("\nEnter Account Number : ");

	DeleteClientDataByAccNumber(vClients, AccountNumber);
}

void UpdateClientInfoScreen(vector <sClient>& vClients)
{
	cout << "-----------------------------------------------\n";
	cout << "\t\tUpdate Client info Screen\n";
	cout << "-----------------------------------------------\n";
	string AccountNumber = InputsLib::ReadString("\nEnter Account Number : ");

	UpdateClientDataByAccNumber(vClients, AccountNumber);
}

void FindClientScreen(vector <sClient>& vClients)
{
	sClient Client;

	cout << "-----------------------------------------------\n";
	cout << "\t\tFind Client Screen\n";
	cout << "-----------------------------------------------\n";
	string AccountNumber = InputsLib::ReadString("\nEnter Account Number : ");

	if (FindClientWithAccNumber(Client, AccountNumber, vClients))
		PrintClientData(Client);
	else
		cout << "Client with account number (" << AccountNumber << ") not found !";
}

void ExitScreen()
{
	cout << "-----------------------------------------------\n";
	cout << "\t\tProgram Ends :-)\n";
	cout << "-----------------------------------------------\n";
}

bool DepositBalanceToClientByAccNumber(vector <sClient>& vClients, string AccountNumber, float Amount)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want Perform This Transaction ? Y/N : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccNumber == AccountNumber)
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

void DepositScreen(vector <sClient>& vClients)
{
	sClient Client;
	cout << "-----------------------------------------------\n";
	cout << "\t\tDeposit Screen\n";
	cout << "-----------------------------------------------\n";

	string AccountNumber = InputsLib::ReadString("\nEnter Account Number : ");

	while (!FindClientWithAccNumber(Client, AccountNumber, vClients))
	{
		cout << "Client with [" << AccountNumber << "] does not exist .";
		AccountNumber = InputsLib::ReadString("\nEnter Account Number : ");
	}
	PrintClientData(Client);

	float Amount = 0;
	cout << "\nEnter deposit amount : ";
	cin >> Amount;

	DepositBalanceToClientByAccNumber(vClients, AccountNumber, Amount);
}

void WithdrawScreen(vector <sClient>& vClients)
{
	sClient Client;
	cout << "-----------------------------------------------\n";
	cout << "\t\tWithdraw Screen\n";
	cout << "-----------------------------------------------\n";

	string AccountNumber = InputsLib::ReadString("\nEnter Account Number : ");

	while (!FindClientWithAccNumber(Client, AccountNumber, vClients))
	{
		cout << "Client with [" << AccountNumber << "] does not exist .";
		AccountNumber = InputsLib::ReadString("\nEnter Account Number : ");
	}
	PrintClientData(Client);

	float Amount = 0;
	cout << "\nEnter withdraw amount : ";
	cin >> Amount;

	while (Amount > Client.AccBalance)
	{
		cout << "\nAmount exceeds the balance , You can withdraw up to : " << Client.AccBalance << endl;
		cout << "Enter another amount : ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccNumber(vClients, AccountNumber, Amount * -1);
}

void PrintClientRecordBalanceLine(sClient Client)
{
	cout << "|" << left << setw(15) << Client.AccNumber;
	cout << "|" << left << setw(30) << Client.Name;
	cout << "|" << left << setw(20) << Client.AccBalance;
}

void TotalBalanceScreen(vector <sClient> vClients)
{
	cout << "\n\t\t\t\tBalances List (" << vClients.size() << ") Client(s)";
	cout << "\n______________________________________________________________________________________________\n" << endl;
	cout << "|" << left << setw(15) << "Account Number";
	cout << "|" << left << setw(30) << "Name";
	cout << "|" << left << setw(20) << "Account Balance";
	cout << "\n______________________________________________________________________________________________\n" << endl;

	float TotalBalance = 0;

	if (vClients.size() == 0)
	{
		cout << "\t\t\tNoClients available in the system !\n";
	}
	else
	{
		for (sClient Client : vClients)
		{
			PrintClientRecordBalanceLine(Client);
			TotalBalance += Client.AccBalance;

			cout << endl;
		}
	}
	cout << "\n______________________________________________________________________________________________\n" << endl;
	cout << "\t\t\t\tTotal Balance : " << TotalBalance << endl;
}

void GoBackToMainMenue(vector <sClient>& vClients)
{
	cout << "\n\nPress any key to back to main menue... ";
	system("pause>0");
	MainMenueScreen(vClients);
}

void GoBackToTransactionScreen(vector <sClient>& vClients)
{
	cout << "\n\nPress any key to back to Transaction Screen... ";
	system("pause>0");
	TransactionScreen(vClients);
}

short ReadMainMenueOptions()
{
	short Answer;
	cout << "\nChoose what do you want to do? [1 to 7] : ";
	cin >> Answer;

	return Answer;
}

void PerformMainMenueOptions(enMainMenueOptions MainMenueOptions, vector <sClient>& vClients)
{
	switch (MainMenueOptions)
	{
	case enMainMenueOptions::ShowClientlist:
		system("cls");
		ClientListScreen(vClients);
		GoBackToMainMenue(vClients);
		break;

	case enMainMenueOptions::AddClient:
		system("cls");
		AddClientsScreen(vClients);
		GoBackToMainMenue(vClients);
		break;

	case enMainMenueOptions::DeleteClient:
		system("cls");
		DeleteClientScreen(vClients);
		GoBackToMainMenue(vClients);
		break;

	case enMainMenueOptions::Updateinfo:
		system("cls");
		UpdateClientInfoScreen(vClients);
		GoBackToMainMenue(vClients);
		break;

	case enMainMenueOptions::FindClient:
		system("cls");
		FindClientScreen(vClients);
		GoBackToMainMenue(vClients);
		break;

	case enMainMenueOptions::Transaction:
		system("cls");
		TransactionScreen(vClients);
		break;

	case enMainMenueOptions::Exit:
		system("cls");
		ExitScreen();
		GoBackToMainMenue(vClients);
		break;
	}
}

void PerformTransactionOptions(enTransactionOptions TransactionOptions, vector <sClient>& vClients)
{
	switch (TransactionOptions)
	{
	case enTransactionOptions::eDeposit:
		system("cls");
		DepositScreen(vClients);
		GoBackToTransactionScreen(vClients);
		break;

	case enTransactionOptions::eWithdraw:
		system("cls");
		WithdrawScreen(vClients);
		GoBackToTransactionScreen(vClients);
		break;

	case enTransactionOptions::eTotalBalance:
		system("cls");
		TotalBalanceScreen(vClients);
		GoBackToTransactionScreen(vClients);
		break;

	case enTransactionOptions::eMainMenue:

		MainMenueScreen(vClients);
	}
}

short ReadTransactionOptions()
{
	short Answer;
	cout << "\nChoose what do you want to do? [1 to 4] : ";
	cin >> Answer;

	return Answer;
}

void TransactionScreen(vector <sClient>& vClients)
{
	system("cls");
	cout << "===============================================\n";
	cout << "\t\tTransaction Screen \n";
	cout << "===============================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balance.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===============================================\n";

	PerformTransactionOptions((enTransactionOptions)ReadTransactionOptions(), vClients);
}

void MainMenueScreen(vector <sClient>& vClients)
{
	system("cls");
	cout << "===============================================\n";
	cout << "\t\tMain Menue Screen \n";
	cout << "===============================================\n";
	cout << "\t[1] Show Clients List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "===============================================\n";

	PerformMainMenueOptions((enMainMenueOptions)ReadMainMenueOptions(), vClients);
}

int main()
{
	vector <sClient> vClients;
	vClients = LoadClientDataFromFile();

	MainMenueScreen(vClients);

	system("pause>0");
	return 0;
}