#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

#pragma region Utilities
#include <windows.h>

enum COLOR
{
	BLACK,
	DARKBLUE,
	DARKGREEN,
	DARKCYAN,
	DARKRED,
	DARKMAGENTA,
	DARKYELLOW,
	GRAY,
	DARKGRAY,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	YELLOW,
	WHITE
};

static COLOR foregroundColor = GRAY;
static COLOR backgroundColor = BLACK;

void SetFontColor(COLOR color)
{
	foregroundColor = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color + (backgroundColor << 4));
}

void SetBackgroundColor(COLOR color)
{
	backgroundColor = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), foregroundColor + (color << 4));
}
#pragma endregion Utilities

using namespace std;

vector<char> charList;
bool alphaView = true;

void DisplayVector(int h1, COLOR c1, int h2, COLOR c2, int begin, int end, int pivot, COLOR pC)
{
	for(int i = 0; i < charList.size(); i++)
	{
		if(i == h1)
			SetFontColor(c1);
		else if(i == h2)
			SetFontColor(c2);
		else if(i == pivot)
			SetFontColor(pC);
		if(i < begin || i > end)
			SetFontColor(BLACK);
		if(alphaView)
		{
			cout << "[" << i << "]: " << charList[i] << endl;
		}
		else
		{
			cout << "[" << i << "]: ";
			for(int j = 65; j <= (int)(charList[i]); j++)
				cout << (char)(176+(i%2)) << (char)(176+(i%2));
			cout << endl;
		}
		SetFontColor(GRAY);
	}
}

void DisplayVector()
{
	DisplayVector(-1, GRAY, -1, GRAY, 0, charList.size() - 1, -1, GRAY);
}

void ToggleView()
{
	alphaView = !alphaView;
}

void Swap(char &a, char &b)
{
	char tmp = a;
	a = b;
	b = tmp;
}

void Swap(int a, int b)
{
	return Swap(charList[a], charList[b]);
}

//Simulation
#include <unistd.h>
const float TIME = 0.5f;
void Simulate(int h1, COLOR c1, int h2 = -1, COLOR c2 = GRAY, int begin = 0, int end = charList.size() - 1, int pivot = -1, COLOR pC = GRAY)
{
	system("CLS");
	DisplayVector(h1, c1, h2, c2, begin, end, pivot, pC);
	usleep(TIME * 1000 * 1000);
}

void RandomizeOrder()
{
	//First Prototype
	/*
	for(int i = 0; i < charList.size(); i++) 
	{
		int first = rand() % charList.size();
		int second = rand() % charList.size();
		
		char tmp = charList[first];
		charList[first] = charList[second];
		charList[second] = tmp;
	}
	*/
	for(int i = 0; i < charList.size(); i++) 
	{
		int random;
		
		do random = rand() % charList.size();
		while(random == i);
		
		Swap(i, random);
	}
}

void BubbleSort(const bool isAscending = true)
{
	const int INCREMENT = (isAscending ? 1 : -1);
	const int BEGIN = (isAscending ? 0 : charList.size() - 1);
	const int END = (isAscending ? charList.size() - 1 : 0);
	
	for(int i = END - INCREMENT; i == BEGIN || (i > BEGIN) == isAscending; i -= INCREMENT)
	{
		//Optimise: If all comparisons are false, the array is sorted and no further checks required.
		bool sorted = true;
		
		for(int j = BEGIN; i == j || (i > j) == isAscending; j += INCREMENT)
		{
			if(charList[j] > charList[j + INCREMENT])
			{
				sorted = false;
				
				Simulate(j, RED, j + INCREMENT, YELLOW);
				
				Swap(j, j + INCREMENT);
				
				Simulate(j, MAGENTA, j + INCREMENT, RED);
			}
			else
			{
				Simulate(j, RED, j + INCREMENT, GREEN);
			}
		}
		
		if(sorted) break;
	}
}

void InsertionSort(const bool isAscending = true)
{
	const int INCREMENT = (isAscending ? 1 : -1);
	const int BEGIN = (isAscending ? 0 : charList.size() - 1);
	const int END = (isAscending ? charList.size() - 1 : 0);
	
	for(int i = BEGIN; i == END || (i < END) == isAscending; i += INCREMENT)
	{
		int j = i;
		
		bool sorted = false;
		
		do
		{
			if(j == BEGIN) break;
			
			if(charList[j] < charList[j - INCREMENT])
			{
				Simulate(j, RED, j - INCREMENT, YELLOW);
				
				Swap(j, j - INCREMENT);
				
				Simulate(j, MAGENTA, j - INCREMENT, RED);
				
				j -= INCREMENT;
			}
			else
			{
				Simulate(j, RED, j - INCREMENT, GREEN);
				sorted = true;
			}
		}
		while(!sorted);
	}
}

void ShellSort(const bool isAscending = true)
{
	const int INCREMENT = (isAscending ? 1 : -1);
	const int BEGIN = (isAscending ? 0 : charList.size() - 1);
	const int END = (isAscending ? charList.size() - 1 : 0);
	
	int gap = (int)(charList.size() / 2);
	
	while(gap >= 1)
	{
		for(int i = BEGIN; i + (gap * INCREMENT) == END || (i + (gap * INCREMENT) < END) == isAscending; i += INCREMENT)
		{
			for(int j = i; j == BEGIN || (j > BEGIN) == isAscending; j -= gap * INCREMENT)
			{
				if(charList[j] > charList[j + (gap * INCREMENT)])
				{
					Simulate(j, RED, j + (gap * INCREMENT), YELLOW);
					
					Swap(j, j + (gap * INCREMENT));
					
					Simulate(j, MAGENTA, j + (gap * INCREMENT), RED);
				}
				else
				{
					Simulate(j, RED, j + (gap * INCREMENT), GREEN);
					break;
				}
			}
		}
		gap = (int)(gap / 2);
	}
}

//Random Pivot method
void QuickSort_RP(const bool isAscending = true, const int start = 0, const int end = charList.size() - 1)
{
	//Improvement for eliminating poor performance for repeated elements
	if(start < end)
	{
		const int INCREMENT = (isAscending ? 1 : -1);
		const int BEGIN = (isAscending ? start : end);
		const int END = (isAscending ? end : start);
		
		//Random Pivot: Eliminates most of the worst cases to improve performance
		int pivot = rand() % (end - start + 1) + start;
		Simulate(pivot, CYAN, -1, GRAY, start, end);
		
		Swap(pivot, BEGIN);
		Simulate(pivot, MAGENTA, BEGIN, CYAN, start, end);
		
		pivot = BEGIN;
		
		for(int i = pivot; i == END || (i < END) == isAscending; i += INCREMENT)
		{
			Simulate(pivot, CYAN, -1, GRAY, start, end);
			if(charList[i] < charList[BEGIN])
			{
				Simulate(i, RED, BEGIN, YELLOW, start, end, pivot, CYAN);
				
				pivot += INCREMENT;
				Simulate(pivot, CYAN, -1, GRAY, start, end);
				
				Swap(i, pivot);
				Simulate(i, MAGENTA, pivot, CYAN, start, end);
			}
			else
			{
				Simulate(i, RED, BEGIN, GREEN, start, end, pivot, CYAN);
			}
		}
		
		Swap(BEGIN, pivot);
		Simulate(BEGIN, MAGENTA, pivot, CYAN, start, end);
	
		QuickSort_RP(isAscending, start, pivot - 1);
		QuickSort_RP(isAscending, pivot + 1, end);
	}
	else return;
}

//Median-of-three method
void QuickSort(const bool isAscending = true, const int start = 0, const int end = charList.size() - 1)
{
	//Improvement for eliminating poor performance for repeated elements
	if(start < end)
	{
		const int INCREMENT = (isAscending ? 1 : -1);
		const int BEGIN = (isAscending ? start : end);
		const int END = (isAscending ? end : start);
		
		//Median-of-three rule: Eliminates most of the worst cases to improve performance
		const int FIRST = (int)charList[start];
		const int MIDDLE = (int)charList[(start + end) / 2];
		const int LAST = (int)charList[end];
		const float MEDIAN = (FIRST + MIDDLE + LAST) / 3;
		
		int pivot = BEGIN;
		
		for(int i = pivot; i == END || (i < END) == isAscending; i += INCREMENT)
		{
			Simulate(i, RED, pivot, CYAN, start, end);
			if(charList[i] <= MEDIAN)
			{
				Simulate(i, YELLOW, pivot, CYAN, start, end);
				
				//If pivot location is "i" itself, skip the swapping
				if(i != pivot && (i == pivot || (i > pivot) == isAscending))
				{
					Swap(i, pivot);
					Simulate(i, MAGENTA, pivot, RED, start, end);
				}
				
				pivot += INCREMENT;
				Simulate(pivot, CYAN, -1, GRAY, start, end);
			}
			else
			{
				Simulate(i, GREEN, pivot, CYAN, start, end);
			}
		}
		
		QuickSort(isAscending, start, pivot - ((INCREMENT + 1) / 2));
		QuickSort(isAscending, pivot - ((INCREMENT - 1) / 2), end);
	}
	else return;
}

/*
void HeapSort(const bool isAscending = true)
{
	
}
*/

void DoChoice(int choice)
{
	switch(choice)
	{
		case 0:
			ToggleView();
			break;
		case 1:
			RandomizeOrder();
			break;
		case 2:
			BubbleSort(true);
			break;
		case 3:
			BubbleSort(false);
			break;
		case 4:
			InsertionSort(true);
			break;
		case 5:
			InsertionSort(false);
			break;
		case 6:
			ShellSort(true);
			break;
		case 7:
			ShellSort(false);
			break;
		case 8:
			QuickSort(true);
			break;
		case 9:
			QuickSort_RP(false);
			break;
		default:
			break;
	}
}

int main()
{
	srand(time(NULL));
	
	const int SIZE = 10;
	
	for(int i = 0; i < SIZE; i++)
	{
		charList.push_back(i + 65);
	}
	
	int choice = -1;
	
	do
	{
		system("CLS");
		DisplayVector();
		cout << "==========================================" << endl;
		cout << "  0. Toggle View" << endl;
		cout << "  1. Randomize order" << endl;
		cout << "  2. Bubble Sort\t(Ascending)" << endl;
		cout << "  3. Bubble Sort\t(Descending)" << endl;
		cout << "  4. Insertion Sort\t(Ascending)" << endl;
		cout << "  5. Insertion Sort\t(Descending)" << endl;
		cout << "  6. Shell Sort \t(Ascending)" << endl;
		cout << "  7. Shell Sort \t(Descending)" << endl;
		cout << "  8. Quick Sort \t(Ascending)" << endl;
		cout << "  9. Quick Sort \t(Descending)" << endl;
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
    
