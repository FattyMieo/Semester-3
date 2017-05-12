#include <iostream>
#include <cstdlib>	// same as stdlib.h (old)
#include <ctime>	// same as time.h (old)
#include <vector>
#include <limits>

using namespace std;

vector<char> charVector;

void DisplayVector()
{
	for(int i = 0; i < charVector.size(); i++)
	{
		cout << "[" << (i < 10 ? "0" : "") << i << "] " << charVector[i] << endl;
	}
}

void PushToBackVector()
{
	char character;
	cout << " @ Please input a character" << endl;
	cout << "> ";
	cin >> character;
	cout << endl;
	
	charVector.push_back(character);
}

void PushToFrontVector()
{
	char character;
	cout << " @ Please input a character" << endl;
	cout << "> ";
	cin >> character;
	cout << endl;
	
	charVector.insert(charVector.begin(), character);
}

void PushToVector()
{
	char character;
	cout << " @ Please input a character" << endl;
	cout << "> ";
	cin >> character;
	cout << endl;
	
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	
	int i = -1;
	while(true)
	{
		cout << " @ Where do you want to place it?" << endl;
		cout << "> ";
		cin >> i;
		
		if(cin.good() && i >= 0)
		{
			if(i >= charVector.size())
			{
				charVector.resize(i + 1, ' ');
			}
			charVector.insert(charVector.begin() + i, character);
			break;
		}
		else
		{
			cout << "Wrong Input!" << endl;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl;
	}
}

void PopFromBackVector()
{
	if(!charVector.empty())
	{
		charVector.pop_back();
	}
	else
	{
		cout << "Vector is empty!" << endl;
		system("PAUSE");
	}
}

void PopFromFrontVector()
{
	if(!charVector.empty())
	{
		charVector.erase(charVector.begin());
	}
	else
	{
		cout << "Vector is empty!" << endl;
		system("PAUSE");
	}
}

void PopFromVector()
{
	if(!charVector.empty())
	{
		int i = -1;
		while(true)
		{
			cout << " @ Where do you want to erase?" << endl;
			cout << "> ";
			cin >> i;
			
			if(cin.good() && i >= 0)
			{
				if(i < charVector.size())
				{
					charVector.erase(charVector.begin() + i);
				}
				else
				{
					cout << "Nothing to erase!" << endl;
					system("PAUSE");
				}
				break;
			}
			else
			{
				cout << "Wrong Input!" << endl;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << endl;
		}
	}
	else
	{
		cout << "Vector is empty!" << endl;
		system("PAUSE");
	}
}

void SwapCharacters()
{
	int i = -1;
	int j = -1;
	while(true)
	{
		cout << " @ Which characters to be swapped?" << endl;
		cout << "> ";
		cin >> i >> j;
		
		if(cin.good() && i >= 0 && j >= 0)
		{
			if(i == j)
			{
				cout << "Cannot swap with the same index." << endl;
				system("PAUSE");
			}
			else if(i < charVector.size() && j < charVector.size())
			{
				char tmp = charVector[i];
				charVector[i] = charVector[j];
				charVector[j] = tmp;
				break;
			}
			else if(i < charVector.size())
			{
				charVector.resize(j + 1, ' ');
				charVector[j] = charVector[i];
				charVector[i] = ' ';
				break;
			}
			else if(j < charVector.size())
			{
				charVector.resize(i + 1, ' ');
				charVector[i] = charVector[j];
				charVector[j] = ' ';
				break;
			}
			else
			{
				cout << "Both characters are not existing!" << endl;
				system("PAUSE");
			}
			break;
		}
		else
		{
			cout << "Wrong Input!" << endl;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl;
	}
}

void ReverseOrder()
{
	int i = 0, j = charVector.size() - 1;
	
	while(i < j)
	{
		char tmp = charVector[i];
		charVector[i] = charVector[j];
		charVector[j] = tmp;
		
		i++; j--;
	}
}

void DeleteEmptyCharacters()
{
	for(int i = 0; i < charVector.size(); i++)
	{
		if(charVector[i] == ' ')
			charVector.erase(charVector.begin() + i--);
	}
}

void ClearVector()
{
	charVector.clear();
}

void PopEmptyBehindVector()
{
	for(int i = charVector.size() - 1; i >= 0; i--)
	{
		if(charVector[i] == ' ')
			charVector.pop_back();
		else
			break;
	}
}

void DoChoice(int choice)
{
	switch(choice)
	{
		case 1:
			PushToBackVector();
			break;
		case 2:
			PushToFrontVector();
			break;
		case 3:
			PushToVector();
			break;
		case 4:
			PopFromBackVector();
			break;
		case 5:
			PopFromFrontVector();
			break;
		case 6:
			PopFromVector();
			break;
		case 7:
			SwapCharacters();
			break;
		case 8:
			ReverseOrder();
			break;
		case 9:
			DeleteEmptyCharacters();
			break;
		case 10:
			ClearVector();
			break;
		default:
			break;
	}
}

int main()
{
	srand(time(NULL));
	
	int randomSize = rand() % 7 + 3;
	for(int i = 0; i < randomSize; i++)
	{
		charVector.push_back(i + 65);
	}
	
	int choice = -1;
	
	do
	{
		system("CLS");
		DisplayVector();
		cout << "==========================================" << endl;
		cout << "  1. Add character to the back" << endl;
		cout << "  2. Add character to the front" << endl;
		cout << "  3. Add character at specific index" << endl;
		cout << "  4. Remove character from the back" << endl;
		cout << "  5. Remove character from the front" << endl;
		cout << "  6. Remove character at specific index" << endl;
		cout << "  7. Swap character at specific indexes" << endl;
		cout << "  8. Reverse order" << endl;
		cout << "  9. Delete all empty characters" << endl;
		cout << " 10. Delete all characters" << endl;
		cout << endl;
		cout << " -1. Exit Program" << endl;
		cout << "==========================================" << endl;
		cout << " @ What would you like to do?" << endl;
		cout << "> ";
		cin >> choice;
		cout << endl;
		
		if(cin.good())
		{
			DoChoice(choice);
			PopEmptyBehindVector();
		}
		else
		{
			cout << "Wrong Input!" << endl;
			system("PAUSE");
		}
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while(choice != -1);
	
	return 0;
}
