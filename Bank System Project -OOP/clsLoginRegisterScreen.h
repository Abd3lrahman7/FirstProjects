#pragma once
#include <iostream>
#include "clsScreen.h"
#include "clsUser.h"
#include "clsGlobal.h"
#include <iomanip>

class clsLoginRegisterScreen:protected clsScreen
{
private:
    static void _PrintLoginRegisterRecordLine(clsUser::stLoginRegister LoginRegister)
    {

        cout << setw(8) << left << "" << "| " << setw(30) << left << LoginRegister.DateTime;
        cout << "| " << setw(20) << left << LoginRegister.Username;
        cout << "| " << setw(20) << left << LoginRegister.Password;
        cout << "| " << setw(12) << left << LoginRegister.Permission;

    }

public:

    static void ShowRegisterLoginListScreen()
    {
        if (!CheckAccessRights(clsUser::enPermissions::pListClients))
        {
            return;// this will exit the function and it will not continue
        }

        vector <clsUser::stLoginRegister> vLoginRegister = clsUser::GetLoginRegisterList();

        string Title = "\tLogin Register List Screen";
        string SubTitle = "\t  (" + to_string(vLoginRegister.size()) + ") Record(s).";

        _DrawScreenHeader(Title, SubTitle);

        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "______________________________________________\n" << endl;

        cout << setw(8) << left << "" << "| " << left << setw(30) << "Date/Time";
        cout << "| " << left << setw(20) << "Username";
        cout << "| " << left << setw(20) << "Password";
        cout << "| " << left << setw(12) << "Permissions";
        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "______________________________________________\n" << endl;

        if (vLoginRegister.size() == 0)
            cout << "\t\t\t\tNo Logins Available In the System!";
        else

            for (clsUser::stLoginRegister Record : vLoginRegister)
            {

                _PrintLoginRegisterRecordLine(Record);
                cout << endl;
            }

        cout << setw(8) << left << "" << "\n\t_______________________________________________________";
        cout << "______________________________________________\n" << endl;
    }
};

