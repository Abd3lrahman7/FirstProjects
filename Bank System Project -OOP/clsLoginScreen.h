#pragma once
#include <iostream>
#include <iomanip>
#include "clsUser.h"
#include "clsScreen.h"
#include "clsGlobal.h"
#include "clsMainScreen.h"

using namespace std;

class clsLoginScreen :protected clsScreen
{
private:
	static bool _Login()
	{
		bool LoginFaild = false;

		short FaildLoginCount = 0;

		string UserName, Password;

		do
		{
			if (LoginFaild)
			{
				FaildLoginCount++;

				cout << "\nInvalid UserName/Password!\n";

				cout << "You have " << 3-FaildLoginCount << " Trial(s) to login.\n";
			}

			if (FaildLoginCount == 3)
			{
				cout << "\nYou are locked after 3 faild trials.\n";
				return false;
			}
			cout << "\nEnter UserName : ";
			getline(cin >> ws, UserName);

			cout << "\nEnter Password : ";
			getline(cin >> ws, Password);

			CurrentUser = clsUser::Find(UserName, Password);

			LoginFaild = CurrentUser.IsEmpty();

		} while (LoginFaild);

		CurrentUser.RegisterLogin();
		clsMainScreen::ShowMainMenue();

		return true;
	}

public :
	static bool ShowLoginScreen()
	{
		system("cls");
		clsScreen::_DrawScreenHeader("\t\tLogin Screen");

		return _Login();
	}
};

