/*
	Utility Library
	by FattyMieo
	Last update : 22/03/2017
	Version 1.0
*/

#include "util.h"

int Utilities::HexToInt(char hex)
{
	if(hex == '0') return 0;
	if(hex == '1') return 1;
	if(hex == '2') return 2;
	if(hex == '3') return 3;
	if(hex == '4') return 4;
	if(hex == '5') return 5;
	if(hex == '6') return 6;
	if(hex == '7') return 7;
	if(hex == '8') return 8;
	if(hex == '9') return 9;
	if(hex == 'A') return 10;
	if(hex == 'B') return 11;
	if(hex == 'C') return 12;
	if(hex == 'D') return 13;
	if(hex == 'E') return 14;
	if(hex == 'F') return 15;
}

char Utilities::IntToHex(int i)
{
	if(i == 0) return '0';
	if(i == 1) return '1';
	if(i == 2) return '2';
	if(i == 3) return '3';
	if(i == 4) return '4';
	if(i == 5) return '5';
	if(i == 6) return '6';
	if(i == 7) return '7';
	if(i == 8) return '8';
	if(i == 9) return '9';
	if(i == 10) return 'A';
	if(i == 11) return 'B';
	if(i == 12) return 'C';
	if(i == 13) return 'D';
	if(i == 14) return 'E';
	if(i == 15) return 'F';
}

int Utilities::Rand_Int(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

int Utilities::cin_int()
{
	int choice;
	bool exit = false;
	
	do
	{
		cout << " > ";
		cin >> choice;
		cout << endl;
		
		if(!cin.good())
			cout << " @ Wrong Input!" << endl;
		else exit = true;
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while(!exit);
	
	return choice;
}

int Utilities::cin_int(int min, int max)
{
	int choice;
	bool exit = false;
	
	do
	{
		cout << " > ";
		cin >> choice;
		cout << endl;
		
		if(!cin.good())
			cout << " @ Wrong Input!" << endl;
		else if(choice < min || choice > max)
			cout << " @ Input does not match the given range!" << endl;
		else exit = true;
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while(!exit);
	
	return choice;
}

char Utilities::cin_char()
{
	char choice;
	bool exit = false;
	
	do
	{
		cout << " > ";
		cin >> choice;
		cout << endl;
		
		if(!cin.good())
			cout << " @ Wrong Input!" << endl;
		else exit = true;
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while(!exit);
	
	return choice;
}

char Utilities::cin_char(char* charList, int charAmount)
{
	char choice;
	bool exit = false;
	
	do
	{
		cout << " > ";
		cin >> choice;
		cout << endl;
		
		if(!cin.good())
			cout << " @ Wrong Input!" << endl;
		else
		{
			for(int i = 0; i < charAmount; i++)
				if(choice == charList[i])
					exit = true;
			
			if(!exit)
				cout << " @ Input does not match the given options!" << endl;
		}
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while(!exit);
	
	return choice;
}

string Utilities::cin_string()
{
	string choice;
	bool exit = false;
	
	do
	{
		cout << " > ";
		getline(cin, choice);
		cout << endl;
		
		if(!cin.good())
			cout << " @ Wrong Input!" << endl;
		else exit = true;
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while(!exit);
	
	return choice;
}

string Utilities::cin_string(int min, int max)
{
	string choice;
	bool exit = false;
	
	do
	{
		cout << " > ";
		getline(cin, choice);
		cout << endl;
		
		if(!cin.good())
			cout << " @ Wrong Input!" << endl;
		else if(choice.length() < min)
			cout << " @ Input does not match minimum length!" << endl;
		else if(choice.length() > max)
			cout << " @ Input does not match maximum length!" << endl;
		else exit = true;
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while(!exit);
	
	return choice;
}
