#include <iostream>
#include <string>
#include <queue>
#include <limits>

using namespace std;

class Node
{
public:
	int value;
	
	//Node left; // You cannot define something while defining it >_<
	Node* left;
	Node* right;
	
	Node()
	{
		left = NULL;
		right = NULL;
	}
};

class BinarySearchTree
{
public:
	Node* root;
	
	BinarySearchTree()
	{
		root = NULL;
	}
	
	void Insert(int value)
	{
		Insert(root, value);
	}
	
	void Insert(Node* &node, int value) // "&" Reference of
	{
		// ! If the node is NULL, create a new node
		if(node == NULL)
		{
			node = new Node();
			node->value = value;
			
			if(node == root)
				cout << "Original";
			cout << endl;
			cout << "Node address: " << &node << endl;
			cout << "Node value: " << node->value << endl;
		}
		else if(value < node->value)
		{
			cout << "Left ";
			Insert(node->left, value); // Recursive Function - The function that calls itself
		}
		else
		{
			cout << "Right ";
			Insert(node->right, value);
		}
	}
	
    void DisplayTree(Node* node, string prefix, bool isTail)
	{
		cout << prefix << (isTail ? (char)192 : (char)195) << (char)196 << (char)196 << " " << node->value << endl;
		
		//if(node->left != NULL) DisplayTree(node->left, prefix + (isTail ? ' ' : (char)179) + "   ", node->right == NULL);
		//if(node->right != NULL) DisplayTree(node->right, prefix + (isTail ? ' ' : (char)179) + "   ", true);
		
		//Reversed display order (Fits the directions)
		if(node->right != NULL) DisplayTree(node->right, prefix + (isTail ? ' ' : (char)179) + "   ", node->left == NULL);
		if(node->left != NULL) DisplayTree(node->left, prefix + (isTail ? ' ' : (char)179) + "   ", true);
    }
	
	void DisplaySortedTree(Node* node)
	{
		DisplayDFSInOrder(node);
	}
	
	/* First Prototype
	void DisplayBFSLevelOrder(Node* node)
	{
		vector<Node*> currentLevelNodes;
		
		currentLevelNodes.push_back(node);
		
		while(!currentLevelNodes.empty())
		{
			int nodes = currentLevelNodes.size();
			for(int i = 0; i < nodes; i++) // This for loop is redundant and should be removed
			{
				cout << " " << currentLevelNodes.front()->value;
				if(currentLevelNodes.front()->left != NULL) currentLevelNodes.push_back(currentLevelNodes.front()->left);
				if(currentLevelNodes.front()->right != NULL) currentLevelNodes.push_back(currentLevelNodes.front()->right);
				currentLevelNodes.erase(currentLevelNodes.begin());
			}
		}
	}
	*/
	void DisplayBFSLevelOrder(Node* node)
	{
		queue<Node*> nodeQueue;
		
		nodeQueue.push(node);
		
		while(!nodeQueue.empty())
		{
			cout << " " << nodeQueue.front()->value;
			
			if(nodeQueue.front()->left != NULL)
				nodeQueue.push(nodeQueue.front()->left);
			if(nodeQueue.front()->right != NULL)
				nodeQueue.push(nodeQueue.front()->right);
			
			nodeQueue.pop();
		}
	}
	
	void DisplayDFSPreOrder(Node* node)
	{
		if(node != NULL)
		{
			cout << " " << node->value;
			
			DisplaySortedTree(node->left);
			
			DisplaySortedTree(node->right);
		}
	}
	
	void DisplayDFSInOrder(Node* node)
	{
		if(node != NULL)
		{
			DisplaySortedTree(node->left);
		
			cout << " " << node->value;
		
			DisplaySortedTree(node->right);
		}
	}
	
	void DisplayDFSPostOrder(Node* node)
	{
		if(node != NULL)
		{
			DisplaySortedTree(node->left);
		
			DisplaySortedTree(node->right);
			
			cout << " " << node->value;
		}
	}
};

int InsertNew()
{
	int i;
	while(true)
	{
		cout << " @ Please input an integer" << endl;
		cout << "> ";
		cin >> i;
		
		if(cin.good())
		{
			cout << endl;
			return i;
		}
		else
		{
			cout << "Wrong Input!" << endl << endl;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

void DoChoice(BinarySearchTree* tree, int choice)
{
	switch(choice)
	{
		case 1:
			tree->Insert(InsertNew());
			break;
		case 2:
			cout << "SortedTree:";
			tree->DisplaySortedTree(tree->root);
			break;
		case 3:
			cout << "BFSLevelOrder:";
			tree->DisplayBFSLevelOrder(tree->root);
			break;
		case 4:
			cout << "DFSPreOrder:";
			tree->DisplayDFSPreOrder(tree->root);
			break;
		case 5:
			cout << "DFSInOrder:";
			tree->DisplayDFSInOrder(tree->root);
			break;
		case 6:
			cout << "DFSPostOrder:";
			tree->DisplayDFSPostOrder(tree->root);
			break;
		default:
			break;
	}
}

int main()
{
	BinarySearchTree* tree = new BinarySearchTree();
	
	cout << "Initializing Binary Search Tree..." << endl << endl;
	
	tree->Insert(43);
	cout << endl;
	tree->Insert(17);
	cout << endl;
	tree->Insert(52);
	cout << endl;
	tree->Insert(8);
	cout << endl;
	tree->Insert(12);
	cout << endl;
	tree->Insert(29);
	cout << endl;
	tree->Insert(65);
	cout << endl;
	tree->Insert(93);
	cout << endl;
	tree->Insert(58);
	cout << endl;
	tree->Insert(77);
	cout << endl;
	tree->Insert(2);
	cout << endl;
	tree->Insert(84);
	cout << endl;
	
	system("PAUSE");
	
	int choice = -1;
	do
	{
		system("CLS");
		
		cout << "############################" << endl;
		cout << "|    Binary Search Tree    |" << endl;
		cout << "############################" << endl << endl;
		
		tree->DisplayTree(tree->root, "", true);
		cout << endl;
		
		cout << "==========================================" << endl;
		cout << "  1. Insert()" << endl;
		cout << "  2. DisplaySortedTree() //Same as DisplayDFSInOrder()" << endl;
		cout << "  3. DisplayBFSLevelOrder()" << endl;
		cout << "  4. DisplayDFSPreOrder()" << endl;
		cout << "  5. DisplayDFSInOrder()" << endl;
		cout << "  6. DisplayDFSPostOrder()" << endl;
		cout << endl;
		cout << " -1. Exit Program" << endl;
		cout << "==========================================" << endl;
		cout << " @ What would you like to do?" << endl;
		cout << "> ";
		cin >> choice;
		cout << endl;
		
		if(cin.good())
		{
			DoChoice(tree, choice);
		}
		else
		{
			cout << "Wrong Input!" << endl;
		}
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		cout << endl;
		system("PAUSE");
	}
	while(choice != -1);
	
	return 0;
}
