#pragma once
#include <iostream>
#include<string>
using namespace std;

namespace StringLib
{
	string LowerAllString(string Text)
	{

		for (short i = 0; i < Text.length(); i++)
		{
			Text[i] = tolower(Text[i]);
		}
		return Text;
	}
	string UpperAllString(string Text)
	{

		for (short i = 0; i < Text.length(); i++)
		{
			Text[i] = toupper(Text[i]);
		}
		return Text;
	}
	string UpperFirstLetterOfEachWard(string Text)
	{
		bool IsFirstLetter = true;

		for (short i = 0; i <= Text.length(); i++)
		{
			if (Text[i] != ' ' && IsFirstLetter)
				Text[i] = toupper(Text[i]);

			IsFirstLetter = (Text[i] == ' ') ? true : false;
		}
		return Text;
	}
	string LowerFirstLetterOfEachWard(string Text)
	{
		bool IsFirstLetter = true;

		for (short i = 0; i <= Text.length(); i++)
		{
			if (Text[i] != ' ' && IsFirstLetter)
				Text[i] = tolower(Text[i]);

			IsFirstLetter = (Text[i] == ' ') ? true : false;
		}
		return Text;
	}
	char InvertLetterCase(char Character)
	{
		return isupper(Character) ? tolower(Character) : toupper(Character);
	}
	string InvertAllStringLetterCase(string Text)
	{
		for (short i = 0; i < Text.length(); i++)
		{
			Text[i] = StringLib::InvertLetterCase(Text[i]);
		}
		return Text;
	}
	short  CountLetter(string Text, char Letter, bool MatchCase = true)
	{
		short Counter = 0;

		for (short i = 0; i < Text.length(); i++)
		{
			if (MatchCase)
			{
				if (Text[i] == Letter)
					Counter++;
			}
			else
			{
				if (tolower(Text[i]) == tolower(Letter))
					Counter++;
			}
		}
		return Counter;
	}
	bool IsVowel(char Character)
	{
		Character = tolower(Character);

		return ((Character == 'a') || (Character == 'e') || (Character == 'i') || (Character == 'o') || (Character == 'u'));
	}
	short CountVowels(string Text)
	{
		short Counter = 0;
		for (short i = 0; i < Text.length(); i++)
		{
			if (StringLib::IsVowel(Text[i]))
				Counter++;
		}
		return Counter;
	}
	void PrintVowels(string Text)
	{
		;	for (short i = 0; i < Text.length(); i++)
		{
			if (StringLib::IsVowel(Text[i]))
				cout << Text[i] << "  ";
		}
	}
	short CountWordsInString(string Text)
	{
		string delim = " ";
		short pos = 0, counter = 0;
		string sWord;

		while ((pos = Text.find(delim)) != std::string::npos)
		{
			sWord = Text.substr(0, pos);
			if (sWord != "")
			{
				counter++;
			}
			Text.erase(0, pos + delim.length());
		}
		if (Text != "")
		{
			counter++;
		}
		return counter;
	}
	vector <string> SplitString(string Text, string Delim)
	{
		vector <string> vToken;
		short pos = 0;
		string sWord;

		while ((pos = Text.find(Delim)) != std::string::npos)
		{
			sWord = Text.substr(0, pos);
			if (sWord != "")
			{
				vToken.push_back(sWord);
			}
			Text.erase(0, pos + Delim.length());
		}
		if (Text != "")
		{
			vToken.push_back(Text);
		}
		return vToken;
	}
	string ReverseString(string S1)
	{
		vector <string> vString;
		string S2 = "";

		vString = SplitString(S1, " ");

		vector <string> ::iterator iter = vString.end();

		while (iter != vString.begin())
		{
			--iter;
			S2 += *iter + " ";
		}
		S2 = S2.substr(0, S2.length() - 1);
		return S2;
	}
	string ReplaceWordInString(string S1, string ChangedWord, string SwitchedWord)
	{
		short Pos = S1.find(ChangedWord);

		while (Pos != std::string::npos)
		{
			S1 = S1.replace(Pos, ChangedWord.length(), SwitchedWord);
			Pos = S1.find(ChangedWord);
		}
		return S1;
	}
	string JoinString(vector<string>vString, string Separator)
	{
		string S1 = "";
		for (string& s : vString)
		{
			S1 = S1 + s + Separator;
		}
		return S1.substr(0, S1.length() - Separator.length());
	}
	string RemovePunctuationsFromString(string S1)
	{
		string S2 = "";
		for (short i = 0; i < S1.length(); i++)
		{
			if (!ispunct(S1[i]))
				S2 += S1[i];
		}
		return S2;
	}
}
