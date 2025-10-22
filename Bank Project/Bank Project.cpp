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

const string UserFileName = "Users.txt";
struct UserInfo
{
	string UserName, Password;
	short Permission = 0;
	bool MarkForDelete = false;
};
string ConvertRecordToLine(UserInfo user, string Delim)
{
	string stClientRecord = "";

	stClientRecord += user.UserName + Delim;
	stClientRecord += user.Password + Delim;
	stClientRecord += to_string(user.Permission);

	return stClientRecord;
}
void AdminSignin(string FileName)
{
	fstream file;
	file.open(FileName, ios::out);

	UserInfo user;
	string DataLine;

	user.UserName = "Admin";
	user.Password = "1234";
	user.Permission = -1;

	DataLine = ConvertRecordToLine(user, "#//#");

	if (file.is_open())
	{
		file << DataLine << endl;
		file.close();
	}
}
enum enMainMenueOptions { ShowClientlist = 1, AddClient = 2, DeleteClient = 3, UpdateInfo = 4,
	FindClient = 5, Transaction = 6, ManageUsers = 7, Logout = 8 };
enum enManageUsersOptions { ListUsers = 1, AddUser = 2, DeleteUser = 3,
	UpdateUser = 4, FindUser = 5, MainMenue = 6 };
enum enTransactionOptions { eDeposit = 1, eWithdraw = 2,
	eTotalBalance = 3, eMainMenue = 4 };
enum enMainMenuePermissions {
	eAll = -1, pListClients = 1, pAddClient = 2, pDeleteClient = 4, pUpdateClient = 8,
	pFindClient = 16, pTransactions = 32, pManageUsers = 64
};

UserInfo User;

struct sClient
{
	string AccNumber = "", PinCode = "", Name = "", Phone = "";
	float AccBalance = 0;
	bool MarkForDelete = false;
};

void MainMenueScreen(vector <sClient>& vClients, vector <UserInfo>& vUsers);
void TransactionScreen(vector <sClient>& vClients);
void LogInScreen(vector <sClient>& vClients, vector <UserInfo>& vUsers);
void PerformTransactionOptions(enTransactionOptions TransactionOptions, vector <sClient>& vClients);
void PerformManageUsersOptions(enManageUsersOptions ManageUsersOptions, vector <sClient>& vClients, vector <UserInfo>& vUsers);

short ReadPermissionsToSet(UserInfo& User, short Accessibility)
{
	char Answer = 'n';
	cout << "Do you want to give full access ? y/n : ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		return -1;

	cout << "\nDo you want give access to :-\n\n";
	cout << "Show Client List ? y/n :";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Accessibility += enMainMenuePermissions::pListClients;

	cout << "Add New Client ? y/n :";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Accessibility += enMainMenuePermissions::pAddClient;

	cout << "Delete Client ? y/n :";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Accessibility += enMainMenuePermissions::pDeleteClient;

	cout << "Ubdate Client ? y/n :";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Accessibility += enMainMenuePermissions::pUpdateClient;

	cout << "Find Client ? y/n :";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Accessibility += enMainMenuePermissions::pFindClient;

	cout << "Transactions ? y/n :";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Accessibility += enMainMenuePermissions::pTransactions;

	cout << "Manage Users ? y/n :";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Accessibility += enMainMenuePermissions::pManageUsers;


	return Accessibility;
}
short ReadTransactionOptions()
{
	short Answer;
	cout << "\nChoose what do you want to do? [1 to 4] : ";
	cin >> Answer;

	return Answer;
}
short ReadManageUsersOptions()
{
	short Answer;
	cout << "\nChoose what do you want to do? [1 to 6] : ";
	cin >> Answer;

	return Answer;
}

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
UserInfo ChangeUserData(string Username)
{
	UserInfo User;

	User.UserName = Username;

	cout << "Enter Password : ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermissionsToSet(User, User.Permission);

	return User;
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
UserInfo ConvertUserLineToRecord(string Line, string Delim)
{
	UserInfo User;
	vector<string>vUserRecord;
	vUserRecord = StringLib::SplitString(Line, Delim);

	User.UserName = vUserRecord[0];
	User.Password = vUserRecord[1];
	User.Permission = stof(vUserRecord[2]);

	return User;
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
void PrintUserData(UserInfo User)
{
	cout << "The Following is Client details : \n\n";
	cout << "-----------------------------------------------\n";
	cout << "Username : " << User.UserName << endl;
	cout << "Password : " << User.Password << endl;
	cout << "Accessibility : " << User.Permission << endl;
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
void PrintUserRecord(UserInfo User)
{
	cout << "|" << left << setw(15) << User.UserName;
	cout << "|" << left << setw(10) << User.Password;
	cout << "|" << left << setw(30) << User.Permission;
}
void ShowAccessDeniedMessagge()
{
	cout << "\n-------------------------------------\n";
	cout << "Access Denied!,\nYou dont have permission to do this,\nPlease contact with Admin.";
	cout << "\n-------------------------------------\n";
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
vector <UserInfo> LoadUserDataFromFile()
{
	fstream StringFile;
	vector <UserInfo> vUsers;

	StringFile.open(UserFileName, ios::in);

	if (StringFile.is_open())
	{
		UserInfo User;
		string Line;

		while (getline(StringFile, Line))
		{
			User = ConvertUserLineToRecord(Line, "#//#");
			vUsers.push_back(User);
		}
		StringFile.close();
	}
	return vUsers;
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
bool FindUserWithUsername(UserInfo& User, string Name, vector <UserInfo> vUsers)
{
	for (UserInfo& U : vUsers)
	{
		if (U.UserName == Name)
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool LoginWithNameAndPassword(UserInfo& User, string Name, string Password, vector <UserInfo> vUsers)
{
	for (UserInfo& U : vUsers)
	{
		if (U.UserName == Name && U.Password == Password)
		{
			User = U;
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
UserInfo AddNewUser(vector <UserInfo>& vUsers)
{
	UserInfo User;
	char Answer = 'n';

	cout << "Enter Username : ";
	getline(cin >> ws, User.UserName);

	while ((FindUserWithUsername(User, User.UserName, vUsers)))
	{
		cout << "User with [" << User.UserName << "] already exist , Enter another Username : ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter Password : ";
	getline(cin, User.Password);

	User.Permission = ReadPermissionsToSet(User, User.Permission);

	return User;
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
vector < UserInfo > SaveUsersDataToFile(vector <UserInfo> vUsers)
{
	fstream MyFile;
	string DataLine;

	MyFile.open(UserFileName, ios::out);

	if (MyFile.is_open())
	{
		for (UserInfo& U : vUsers)
		{
			if (U.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(U, "#//#");
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUsers;
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
bool MarkUserForDeleteByUsername(string Username, vector <UserInfo>vUsers)
{
	for (UserInfo& U : vUsers)
	{
		if (U.UserName == Username)
		{
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
bool DeleteUserDataByUsername(vector <UserInfo>vUsers, string Username)
{
	UserInfo User;
	char Answer = 'n';

	if (FindUserWithUsername(User, Username, vUsers))
	{
		PrintUserData(User);
		cout << "\n\nAre you sure you want to delete this User ? Y/N : ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			MarkUserForDeleteByUsername(Username, vUsers);
			SaveUsersDataToFile(vUsers);

			vUsers = LoadUserDataFromFile();
			cout << "\n\nUser Deleted successfully.";
			return true;
		}
	}
	else
	{
		cout << "\n\nUser with Username (" << Username << ") not found.";
		return false;
	}
}
bool UpdateUserDataByUsername(vector <UserInfo>vUsers, string Username)
{
	UserInfo User;
	char Answer = 'n';

	if (FindUserWithUsername(User, Username, vUsers))
	{
		PrintUserData(User);
		cout << "\n\nAre you sure you want to Update this User ? Y/N : ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			for (UserInfo& U : vUsers)
			{
				if (U.UserName == Username)
				{
					U = ChangeUserData(Username);
					break;
				}
			}

			SaveUsersDataToFile(vUsers);
			cout << "\n\nUser Updated successfully.";
			return true;
		}
	}
	else
	{
		cout << "\n\nUser with Username (" << Username << ") not found.";
		return false;
	}
}
bool CheckAccessPermision(enMainMenuePermissions Permission)
{
	if (User.Permission == enMainMenuePermissions::eAll)
		return true;
	if ((User.Permission & Permission) == Permission)
		return true;
	else
		return false;
}

void ClientListScreen(vector <sClient> vClients)
{
	if (!CheckAccessPermision(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessagge();
		return;
	}

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
void AddClientsScreen(vector<sClient>vClients)
{
	if (!CheckAccessPermision(enMainMenuePermissions::pAddClient))
	{
		ShowAccessDeniedMessagge();
		return;
	}

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
	if (!CheckAccessPermision(enMainMenuePermissions::pDeleteClient))
	{
		ShowAccessDeniedMessagge();
		return;
	}

	sClient Client;

	cout << "-----------------------------------------------\n";
	cout << "\t\tDelete Client Screen\n";
	cout << "-----------------------------------------------\n";
	string AccountNumber = InputsLib::ReadString("\nEnter Account Number : ");

	DeleteClientDataByAccNumber(vClients, AccountNumber);
}
void UpdateClientInfoScreen(vector <sClient>& vClients)
{
	if (!CheckAccessPermision(enMainMenuePermissions::pUpdateClient))
	{
		ShowAccessDeniedMessagge();
		return;
	}

	cout << "-----------------------------------------------\n";
	cout << "\t\tUpdate Client info Screen\n";
	cout << "-----------------------------------------------\n";
	string AccountNumber = InputsLib::ReadString("\nEnter Account Number : ");

	UpdateClientDataByAccNumber(vClients, AccountNumber);
}
void FindClientScreen(vector <sClient>& vClients)
{
	if (!CheckAccessPermision(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessagge();
		return;
	}

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
void TransactionScreen(vector <sClient>& vClients)
{
	if (!CheckAccessPermision(enMainMenuePermissions::pTransactions))
	{
		ShowAccessDeniedMessagge();
		return;
	}

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
void ManageUsersScreen(vector <sClient>& vClients, vector <UserInfo>& vUsers)
{
	if (!CheckAccessPermision(enMainMenuePermissions::pManageUsers))
	{
		ShowAccessDeniedMessagge();
		return;
	}

	system("cls");
	cout << "===============================================\n";
	cout << "\t\tManage Users Menue Screen \n";
	cout << "===============================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User info.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "===============================================\n";

	PerformManageUsersOptions((enManageUsersOptions)ReadManageUsersOptions(), vClients, vUsers);
}
void AddUserScreen(vector <UserInfo>& vUsers)
{
	UserInfo User;
	char Answer = 'n';
	system("cls");
	cout << "-----------------------------------------------\n";
	cout << "\t\tAdd New Users Screen\n";
	cout << "-----------------------------------------------\n";

	do
	{
		cout << "\nAdd new User :-\n\n";
		User = AddNewUser(vUsers);
		vUsers.push_back(User);
		SaveUsersDataToFile(vUsers);

		cout << "User Added successfully , do you want to add more Users ? Y/N :";
		cin >> Answer;

	} while (Answer == 'Y' || Answer == 'y');

}
void DeleteUserScreen(vector <UserInfo>& vUsers)
{
	sClient Client;

	cout << "-----------------------------------------------\n";
	cout << "\t\tDelete User Screen\n";
	cout << "-----------------------------------------------\n";
	string Username = InputsLib::ReadString("\nEnter Username : ");

	DeleteUserDataByUsername(vUsers, Username);
}
void UpdateUserInfoScreen(vector <UserInfo>& vUsers)
{
	cout << "-----------------------------------------------\n";
	cout << "\t\tUpdate User info Screen\n";
	cout << "-----------------------------------------------\n";
	string Username = InputsLib::ReadString("\nEnter Username : ");

	UpdateUserDataByUsername(vUsers, Username);
}
void FindUserScreen(vector <UserInfo>& vUsers)
{
	UserInfo User;

	cout << "-----------------------------------------------\n";
	cout << "\t\tFind User Screen\n";
	cout << "-----------------------------------------------\n";
	string Username = InputsLib::ReadString("\nEnter Username : ");

	if (FindUserWithUsername(User, Username, vUsers))
		PrintUserData(User);
	else
		cout << "User with Username (" << Username << ") not found !";
}
void UserListScreen(vector <UserInfo> vUsers)
{
	cout << "\n\t\t\t\t\tClient List (" << vUsers.size() << ") Client(s)";
	cout << "\n______________________________________________________________________________________________________________\n" << endl;

	cout << "|" << left << setw(15) << "Userame";
	cout << "|" << left << setw(10) << "Password";
	cout << "|" << left << setw(30) << "Accessibility";
	cout << "\n______________________________________________________________________________________________________________\n" << endl;

	if (vUsers.size() == 0)
		cout << "\t\t\t\t\tNo Users Avilable in the system!";
	else {
		for (UserInfo User : vUsers)
		{
			PrintUserRecord(User);
			cout << endl;
		}
	}
	cout << "\n______________________________________________________________________________________________________________\n" << endl;

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

void GoBackToMainMenue(vector <sClient>& vClients, vector <UserInfo>& vUsers)
{
	cout << "\n\nPress any key to back to main menue... ";
	system("pause>0");
	MainMenueScreen(vClients, vUsers);
}
void GoBackToManageUsersScreen(vector <sClient>& vClients, vector <UserInfo>& vUsers)
{
	cout << "\n\nPress any key to back to Manage Users Screen... ";
	system("pause>0");
	ManageUsersScreen(vClients, vUsers);
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
	cout << "\nChoose what do you want to do? [1 to 8] : ";
	cin >> Answer;

	return Answer;
}

void PerformMainMenueOptions(enMainMenueOptions MainMenueOptions, vector <sClient>& vClients, vector <UserInfo>& vUsers)
{
	switch (MainMenueOptions)
	{
	case enMainMenueOptions::ShowClientlist:
		system("cls");
		ClientListScreen(vClients);
		GoBackToMainMenue(vClients, vUsers);
		break;

	case enMainMenueOptions::AddClient:
		system("cls");
		AddClientsScreen(vClients);
		GoBackToMainMenue(vClients, vUsers);
		break;

	case enMainMenueOptions::DeleteClient:
		system("cls");
		DeleteClientScreen(vClients);
		GoBackToMainMenue(vClients, vUsers);
		break;

	case enMainMenueOptions::UpdateInfo:
		system("cls");
		UpdateClientInfoScreen(vClients);
		GoBackToMainMenue(vClients, vUsers);
		break;

	case enMainMenueOptions::FindClient:
		system("cls");
		FindClientScreen(vClients);
		GoBackToMainMenue(vClients, vUsers);
		break;

	case enMainMenueOptions::Transaction:
		system("cls");
		TransactionScreen(vClients);
		break;

	case enMainMenueOptions::ManageUsers:
		system("cls");
		ManageUsersScreen(vClients, vUsers);
		break;
	case enMainMenueOptions::Logout:
		system("cls");
		LogInScreen(vClients, vUsers);
		break;

	}
}
void PerformManageUsersOptions(enManageUsersOptions ManageUsersOptions, vector <sClient>& vClients, vector <UserInfo>& vUsers)
{
	switch (ManageUsersOptions)
	{
	case enManageUsersOptions::ListUsers:
		system("cls");
		UserListScreen(vUsers);
		GoBackToManageUsersScreen(vClients, vUsers);
		break;

	case enManageUsersOptions::AddUser:
		system("cls");
		AddUserScreen(vUsers);
		GoBackToManageUsersScreen(vClients, vUsers);
		break;

	case enManageUsersOptions::DeleteUser:
		system("cls");
		DeleteUserScreen(vUsers);
		GoBackToManageUsersScreen(vClients, vUsers);
		break;

	case enManageUsersOptions::UpdateUser:
		system("cls");
		UpdateUserInfoScreen(vUsers);
		GoBackToManageUsersScreen(vClients, vUsers);
		break;

	case enManageUsersOptions::FindUser:
		system("cls");
		FindUserScreen(vUsers);
		GoBackToManageUsersScreen(vClients, vUsers);
		break;

	case enManageUsersOptions::MainMenue:
		system("cls");
		MainMenueScreen(vClients, vUsers);
		break;

	}
}
void PerformTransactionOptions(enTransactionOptions TransactionOptions, vector <sClient>& vClients)
{
	vector <UserInfo> vUsers;
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

		MainMenueScreen(vClients, vUsers);
	}
}

void MainMenueScreen(vector <sClient>& vClients, vector <UserInfo>& vUsers)
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
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===============================================\n";

	PerformMainMenueOptions((enMainMenueOptions)ReadMainMenueOptions(), vClients, vUsers);
}

bool LoadUserInfo(string Username, string Password, vector <UserInfo>& vUsers)
{
	if (LoginWithNameAndPassword(User, Username, Password, vUsers))
		return true;
	else
		return false;
}
void LogInScreen(vector <sClient>& vClients, vector <UserInfo>& vUsers)
{
	string UserName, Password;
	bool LoginFailed = false;

	do {
		system("cls");

		cout << "-----------------------------------------------\n";
		cout << "\t\tLogin Screen\n";
		cout << "-----------------------------------------------\n";

		if (LoginFailed)
			cout << "Invalid Username/Password!\n\n";

		cout << "Enter Username : ";
		getline(cin >> ws, UserName);
		cout << "Enter Password : ";
		getline(cin, Password);

		LoginFailed = !LoadUserInfo(UserName, Password, vUsers);

	} while (LoginFailed);

	MainMenueScreen(vClients, vUsers);
}

int main()
{
	vector <sClient> vClients;
	vector <UserInfo> vUsers;
	vClients = LoadClientDataFromFile();
	vUsers = LoadUserDataFromFile();

	LogInScreen(vClients, vUsers);

	system("pause>0");
	return 0;
}

