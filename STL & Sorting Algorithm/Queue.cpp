#include <iostream>//<-- Created during C++
#include <stdlib.h>//<-- Created during C		/* Including: srand(), rand() */
#include <time.h>								/* Including: time() */
#include <queue>								//FIFO - First in, first out

using namespace std;

class MyClass
{
	string name;
};

int main()
{
	// ! Seeding the Random function with system clock's time as the parameter
	srand(time(NULL));
	// ! Seeding the Random function with 8888 as the parameter, always the same number
	//srand(8888);
	
	queue<int> numberQueue;			// ! Abstract Data Type "xxxx<>", it doesn't matter the form //<> = Container
	queue<MyClass> myClassQueue;	// ! Whatever datatype can fit in
	
	int randomSize = rand() % 10 + 3;
	
	for(int i = 0; i < randomSize; i++)
	{
		numberQueue.push(i);
	}
	
	for(int i = 0; i < randomSize; i++)
	{
		cout << numberQueue.front() << endl;
		numberQueue.pop();
	}
	
	system("PAUSE");
	return 0;
}
