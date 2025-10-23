#pragma once
#include <iostream>

using namespace std;

namespace InputsLib 
{
	float ReadPositiveNumber(string Message)
	{
		float number;
		do
		{
			cout << Message;
			cin >> number;

			while (cin.fail())
			{
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cout << "Invalid Number, Please try again : ";
				cin >> number;
			}

		} while (number <= 0);

		return number;
	}
	float ReadNumber(string Message)
	{
		float number;
		cout << Message;
		cin >> number;
		while (cin.fail()) 
		{  
			cin.clear();      
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Invalid Number, Please try again : ";
			cin >> number;   
		} 

		return number;
	}
	string ReadString(string message)
	{
		string text;
		cout << message;

		getline(cin>>ws, text);
	
		return text;
	}
	char ReadChar(string message)
	{
		char character;

		cout << message;
		cin >> character;

		return character;
	}
	int ReadNumberInRange(string Message, int From, int To)
	{
		int number;
		do
		{
			cout << Message;
			cin >> number;

			while (cin.fail())
			{
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cout << "Invalid Number, Please try again : ";
				cin >> number;
			}

		} while (number < From || number> To);

		return number;
	}
}