#include <iostream>//<-- Created during C++
#include <stdlib.h>//<-- Created during C		/* Including: srand(), rand() */
#include <time.h>								/* Including: time() */
#include <stack>								//LIFO - Last in, first out

using namespace std;

int main()
{
	// ! Seeding the Random function with system clock's time as the parameter
	srand(time(NULL));
	// ! Seeding the Random function with 8888 as the parameter, always the same number
	//srand(8888);
	
	stack<int> numberStack;
	
	int randomSize = rand() % 10 + 3;
	
	for(int i = 0; i < randomSize; i++)
	{
		numberStack.push(i);
	}
	
	stack<int> numberStackReverse;
	
	for(int i = 0; i < randomSize; i++)
	{
		numberStackReverse.push(numberStack.top());
		numberStack.pop();
	}
	
	for(int i = 0; i < randomSize; i++)
	{
		cout << numberStackReverse.top() << endl;
		numberStackReverse.pop();
	}
	
	system("PAUSE");
	return 0;
}
