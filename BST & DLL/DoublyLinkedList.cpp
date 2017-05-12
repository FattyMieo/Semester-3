#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
//#include <list>

using namespace std;

class Node
{
public:
	int value;
	Node* prev;
	Node* next;

	Node()
	{
		//Always initialize pointers to NULL
		prev = NULL;
		next = NULL;
	}
};

class DoublyLinkedList
{
private:
	bool IsValid(int index)
	{
		return index >= 0;
	}
	
public:
	Node* head; // or first
	
	DoublyLinkedList()
	{
		head = NULL;
	}
	
	bool IsEmpty()
	{
		return head == NULL;
	}
	
	//The most "next" element
	Node* GetBack()
	{
		if(IsEmpty())
			return head;
		
		Node* current = head; //If head is NULL, current will point to NULL instead of head (Do predict above)
		
		while(current->next != NULL)
		{
			current = current->next; //Keep searching
		}
		
		return current;
	}
	
	void PushToBack(int value)
	{
		if(IsEmpty())
		{
			//Create new node
			Node* newNode = new Node();
			
			//Configure new node's value
			newNode->value = value;
			
			//Relink
			head = newNode;
			
			return;
		}
		
		Node* back = GetBack();
		
		//Create new node
		Node* newNode = new Node();
		
		//Configure new node's value
		newNode->value = value;
		
		//Configure new node's link
		newNode->prev = back;
		
		//Relink
		back->next = newNode;
	}
	
	void PopFromBack()
	{
		if(IsEmpty())
			return; //Nothing to pop
		
		Node* back = GetBack();
		
		Node* previous = back->prev;
		
		if(previous == NULL) //If previous is NULL, it means back is the frontmost element of the list (head)
			head = NULL;
		else
			previous->next = NULL; //Set the previous' next to NULL (break the relationship with the last element)
	}
	
	Node* GetNode(int index)
	{
		if(!IsValid(index)) //Invalid index
			return NULL;
		
		if(IsEmpty())
			return head;
		
		Node* current = head;
		
		for(int i = 0; i < index; i++) // i = 1 for head->next
		{
			if(current != NULL)
			{
				current = current->next;
			}
			else
			{
				return NULL; //The index is out of bound
			}
		}
		
		return current;
	}
	
	void Insert(int index, int value)
	{
		if(!IsValid(index)) //Invalid index
			return;
		
		if(IsEmpty())
			return;
		
		//Create new node
		Node* newNode = new Node();
		
		//Configure new node's value
		newNode->value = value;
		
		if(index == 0) //Special case, index 0 indicates that inserting to head
		{
			//Configure new node's link
			newNode->next = head;
			newNode->next->prev = newNode;
			
			//Relink
			head = newNode;
			
			return;
		}
		
		Node* previous = GetNode(index - 1);
		
		if(previous == NULL) //Index is out of bound
			return;
		
		Node* current = previous->next; //Temporarily saves the node at the selected index (no matter it's NULL(non-existance) or not)
		
		// @ newNode->prev node settings
		//Configure new node's link
		newNode->prev = previous;
		
		//Relink
		previous->next = newNode;
		
		// @ newNode->next node settings
		if(current != NULL) //If index is not the end of the list, newNode->next exists (!= NULL)
		{
			//Configure new node's link
			newNode->next = current;
			
			//Relink
			current->prev = newNode;
		}
	}
	
	void Remove(int index)
	{
		if(!IsValid(index)) //Invalid index
			return;
		
		if(IsEmpty())
			return;
		
		Node* current = GetNode(index);
		
		if(current == NULL) //Index is out of bound
			return;
		
		Node* previous = current->prev;
		Node* next = current->next;
		
		if(previous == NULL) //If previous is NULL, it means current is the frontmost element of the list (head)
		{
			head = next;
			head->prev = NULL;
		}
		else
		{
			previous->next = next;
			if(next != NULL) //If next is not NULL, it means current->next exists (!= NULL)
			{
				next->prev = previous;
			}
		}
	}
	
	void Swap(int index1, int index2)
	{
		if(index1 == index2) //Same indices swapping, nothing changes
			return;
			
		if(!IsValid(index1) || !IsValid(index2)) //Invalid index/indices
			return;
		
		if(IsEmpty())
			return;
		
		if(index1 > index2) //Esnures index1 < index2 (For alternative smethod to work)
		{
			int tmp = index1;
			index1 = index2;
			index2 = tmp;
		}
		
		Node* node1 = GetNode(index1);
		Node* node2 = GetNode(index2);
		
		if(node1 == NULL || node2 == NULL) //One or more of the indices are out of bound
			return;
		
		/* The following method will cause self looping if 2 nodes are neighbouring each other
		//Create a temporary node for swapping
		Node* tempNode = new Node();
		
		//Swapping
		tempNode->prev = node1->prev;
		tempNode->next = node1->next;
		
		node1->prev = node2->prev;
		node1->next = node2->next;
		
		node2->prev = tempNode->prev;
		node2->next = tempNode->next;
		
		//Relink
		if(node1->prev != NULL) node1->prev->next = node1;
		if(node1->next != NULL) node1->next->prev = node1;
		
		if(node2->prev != NULL) node2->prev->next = node2;
		if(node2->next != NULL) node2->next->prev = node2;
		
		//Head check
		if(index1 == 0) head = node2;
		else if(index2 == 0) head = node1;
		*/
		
		// Alternate Method (Safer, IF Insert() and Remove() is safe too :P)
		// Require indices to be arranged (index1 < index2)
		Remove(index1);
		Insert(index2 - 1, node1->value);
		
		Remove(index2);
		Insert(index1, node2->value);
	}
	
	void Display()
	{
		int counter = 0;
		
		Node* current = head;
		
		while(current != NULL)
		{
			cout << "[" << counter++ << "] " << current->value << endl;
			current = current->next;
		}
		
		cout << endl;
		cout << "Total numbers in the linked list is " << counter << endl;
	}
};

int GetValue()
{
	int i;
	
	cout << " @ Please input an integer" << endl;
	while(true)
	{
		cout << "> ";
		cin >> i;
		
		if(cin.good())
		{
			cout << endl;
			return i;
		}
		else
		{
			cout << endl << "Wrong Input!" << endl;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

int GetIndex()
{
	int i;
	
	cout << " @ Please input the index" << endl;
	while(true)
	{
		cout << "> ";
		cin >> i;
		
		if(cin.good() && i >= 0)
		{
			cout << endl;
			return i;
		}
		else
		{
			cout << endl << "Wrong Input!" << endl;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

void DoChoice(DoublyLinkedList* list, int choice)
{
	int index;
	switch(choice)
	{
		case 0:
			cout << "First element\'s value: " << list->head->value << endl;
			cout << endl;
			system("PAUSE");
			break;
		case 1:
			cout << "Last element\'s value: " << list->GetBack()->value << endl;
			cout << endl;
			system("PAUSE");
			break;
		case 2:
			list->PushToBack(GetValue());
			break;
		case 3:
			list->PopFromBack();
			break;
		case 4:
			index = GetIndex();
			cout << index << (index % 10 == 1 ? "st" : (index % 10 == 2 ? "nd" : (index % 10 == 3 ? "rd" : "th"))) << " element\'s value: " << list->GetNode(index)->value << endl;
			cout << endl;
			system("PAUSE");
			break;
		case 5:
			list->Insert(GetIndex(), GetValue());
			break;
		case 6:
			list->Remove(GetIndex());
			break;
		case 7:
			list->Swap(GetIndex(), GetIndex());
			break;
		default:
			break;
	}
}

int main()
{
	srand(time(NULL));
	
	DoublyLinkedList* numberList = new DoublyLinkedList();
	
	cout << "############################" << endl;
	cout << "|    Doubly Linked List    |" << endl;
	cout << "############################" << endl;
	
	cout << "Initializing Doubly Linked List..." << endl;
	
	//for(int i = 0; i < 10; i++)
	//	numberList->PushToBack(i);
	for(int i = 1; i <= 10; i++)
		numberList->PushToBack(rand() % i + 1);
	
	numberList->Display();
	cout << endl;
	
	system("PAUSE");
	
	int choice = -1;
	do
	{
		system("CLS");
		
		cout << "############################" << endl;
		cout << "|    Doubly Linked List    |" << endl;
		cout << "############################" << endl << endl;
		
		numberList->Display();
		cout << endl;
		
		cout << "==========================================" << endl;
		cout << "  0. GetHead()" << endl;
		cout << "  1. GetBack()" << endl;
		cout << "  2. PushToBack()" << endl;
		cout << "  3. PopFromBack()" << endl;
		cout << "  4. GetNode()" << endl;
		cout << "  5. Insert()" << endl;
		cout << "  6. Remove()" << endl;
		cout << "  7. Swap()" << endl;
		cout << endl;
		cout << " -1. Exit Program" << endl;
		cout << "==========================================" << endl;
		cout << " @ What would you like to do?" << endl;
		cout << "> ";
		cin >> choice;
		cout << endl;
		
		if(cin.good())
		{
			DoChoice(numberList, choice);
		}
		else
		{
			cout << "Wrong Input!" << endl;
		}
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while(choice != -1);
	
	return 0;
}
