//Deprecation definition
#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

#include <iostream>
#include <vector>
#include <limits>
#include <windows.h>
#include <algorithm>

using namespace std;

enum Player
{
	PLAYER1 = 0,
	PLAYER2,
	TOTAL_PLAYERS
};

enum BoardType
{
	CLASSIC = 0,
	NOTAKO,
	WILD,
	ULTIMATE,
	QUANTUM,
	TOTAL_BOARD_TYPE
};

enum Token
{
	TOKEN_X = 0,
	TOKEN_O,
	BLANK
};

enum WinningCondition
{
	WIN_X = 0,
	WIN_O,
	DRAW,
	NO_WIN
};

//Utilities
void GoToXY(int x, int y)
{
	COORD coordinate;

	coordinate.X = x;
	coordinate.Y = y ;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

int GetCursorPositionX()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	return info.dwCursorPosition.X;
}

int GetCursorPositionY()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	return info.dwCursorPosition.Y;
}

int Choose(int min, int max)
{
	int choice;
	bool exit = false;
	
	do
	{
		cout << " > ";
		cin >> choice;
		cout << endl;
		
		exit = cin.good() && choice >= min && choice <= max;
		
		if(!exit)
			cout << " @ Wrong Input!" << endl;
		
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	while(!exit);
	
	return choice;
}

//Board Classes
class Board;
class ClassicBoard;
class NotakoBoard;
class UltimateBoard;
class QuantumBoard;

class Board
{
friend void ResetBoard();

private:
	static string playerNames[TOTAL_PLAYERS];
	static int score[TOTAL_PLAYERS];
	static Player firstHandPlayer;

protected:
	static const int SIZE = 3;
	
	virtual bool IsDraw() = 0;
	
	virtual bool NonDiagonalCheck(Token player, int i) = 0;
	
	virtual bool DiagonalCheck(Token player) = 0;
	
	virtual bool PlaceToken(int mark) = 0;
	
	virtual WinningCondition CheckWinningCondition() = 0;
	
public:
	virtual void Draw() = 0;
	
	virtual void GetInput() = 0;
	
	virtual bool IsGameOver() = 0;
	
	//Displays title - Inline
	inline static void Title()
	{
		cout << "        ==========================" << endl;
		cout << "                Tic Tac Toe       " << endl;
		cout << "        ==========================" << endl;
	}
	
	static char GetTokenChar(Token token)
	{
		switch(token)
		{
			case TOKEN_X:
				return 'X';
			case TOKEN_O:
				return 'O';
			case BLANK:
			default:
				return ' ';
		}
	}
	
	//May introduce custom player name
	static string GetPlayerName(Player player)
	{
		switch(player)
		{
			case PLAYER1:
			case PLAYER2:
				return playerNames[(int)player];
				break;
			default:
				return "";
		}
	}
	
	static void AddScore(Player player)
	{
		score[(int)player]++;
	}
	
	static void ShowScore(bool showSymbol = true)
	{
		cout
		//Show player's name
		<< GetPlayerName(PLAYER1) << " " 
		//If showing symbols, place brackets
		<< (showSymbol ? "(" : "")
		//If showing symbols, show the symbol
		<< (showSymbol ? (PLAYER1 == GetFirstHand() ? GetTokenChar(TOKEN_X) : GetTokenChar(TOKEN_O)) : ' ')
		//If showing symbols, place brackets
		<< (showSymbol ? ")" : "")
		//Show player's score
		<< ": " << score[0] << endl;
		
		cout
		//Show player's name
		<< GetPlayerName(PLAYER2) << " "
		//If showing symbols, place brackets
		<< (showSymbol ? "(" : "")
		//If showing symbols, show the symbol
		<< (showSymbol ? (PLAYER2 == GetFirstHand() ? GetTokenChar(TOKEN_X) : GetTokenChar(TOKEN_O)) : ' ')
		//If showing symbols, place brackets
		<< (showSymbol ? ")" : "")
		//Show player's score
		<< ": " << score[1] << endl;
	}
	
	static Player GetFirstHand()
	{
		return firstHandPlayer;
	}
	
	static Player GetSecondHand()
	{
		switch(firstHandPlayer)
		{
			case PLAYER1:
				return PLAYER2;
				break;
			case PLAYER2:
				return PLAYER1;
				break;
			default:
				return GetFirstHand();
		}
	}
	
	static void SwapFirstHand()
	{
		switch(firstHandPlayer)
		{
			case PLAYER1:
				firstHandPlayer = PLAYER2;
				break;
			case PLAYER2:
				firstHandPlayer = PLAYER1;
				break;
			default:
				break;
		}
	}
	
	static void SwapFirstHand(Player player)
	{
		firstHandPlayer = player;
	}
	
	static void ShowTurn(Token player)
	{
		switch(player)
		{
			case TOKEN_X:
				cout << GetPlayerName(GetFirstHand()) << "\'s Turn" << endl;
				break;
			case TOKEN_O:
				cout << GetPlayerName(GetSecondHand()) << "\'s Turn" << endl;
				break;
			default:
				cout << "Anonymous Turn" << endl;
				break;
		}
	}
	
	static void ShowTurn(Player player)
	{
		cout << GetPlayerName(player) << "\'s Turn" << endl;
	}
	
	Board()
	{
		system("CLS");
		Board::Title();
		cout << endl;
		cout << "A new Board has been created!" << endl;
		cout << "Enjoy the game!" << endl;
		system("PAUSE");
	}
	
	~Board()
	{
		system("CLS");
		Board::Title();
		cout << endl;
		cout << "The Board has been destroyed!" << endl;
		cout << "All data are lost T_T" << endl;
		system("PAUSE");
	}
};

//Global Initialization for static variables
string Board::playerNames[TOTAL_PLAYERS] = {"", ""};
int Board::score[TOTAL_PLAYERS] = {0, 0};
Player Board::firstHandPlayer = PLAYER1;

class ClassicBoard : public virtual Board
{
//Share protected data with UltimateBoard
friend class UltimateBoard;

private:
	int OneExtraMemberVar;

protected:
	Token** matrix;
	Token player;
	
	virtual void InitializeMatrix()
	{
		matrix = new Token*[SIZE];
		for(int i = 0; i < SIZE; i++)
		{
			matrix[i] = new Token[SIZE];
			for(int j = 0; j < SIZE; j++)
			{
				matrix[i][j] = BLANK;
			}
		}
	}
	
	virtual bool PlaceToken(int mark)
	{
		int x = (mark - 1) % SIZE;
		int y = (mark - 1) / SIZE;
		
		if(matrix[y][x] == BLANK)
		{
			matrix[y][x] = player;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	virtual bool IsDraw()
	{
		for(int i = 0; i < SIZE; i++)
		{
			for(int j = 0; j < SIZE; j++)
			{
				if(matrix[i][j] == BLANK)
				{
					return false;
				}
			}
		}
		return true;
	}
	
	virtual bool NonDiagonalCheck(Token player, int i)
	{
		int count[2] = {0, 0};
		for(int j = 0; j < SIZE; j++)
		{
			//Horizontal Check
			if(matrix[i][j] == player) count[0]++;
			
			//Vertical Check
			if(matrix[j][i] == player) count[1]++;
		}
		
		//Is there a winner?
		if(count[0] >= SIZE || count[1] >= SIZE)
			return true;
		else
			return false;
	}
	
	virtual bool DiagonalCheck(Token player)
	{
		int count[2] = {0, 0};
		for(int i = 0; i < SIZE; i++)
		{
			//Diagonal Check 1
			if(matrix[i][i] == player) count[0]++;
			
			//Diagonal Check 2
			if(matrix[SIZE-1-i][i] == player) count[1]++;
		}
		
		//Is there a winner?
		if(count[0] >= SIZE || count[1] >= SIZE)
			return true;
		else
			return false;
	}
	
	virtual WinningCondition CheckWinningCondition()
	{
		if(player == BLANK) return NO_WIN;
		
		//For every row and column
		for(int i = 0; i < SIZE; i++)
		{
			//Do Non-Diagonal Check
			if(NonDiagonalCheck(TOKEN_X, i))
				return (WinningCondition)TOKEN_X;
			else if(NonDiagonalCheck(TOKEN_O, i))
				return (WinningCondition)TOKEN_O;
		}
		
		//Do Diagonal Check
		if(DiagonalCheck(TOKEN_X))
			return (WinningCondition)TOKEN_X;
		else if(DiagonalCheck(TOKEN_O))
			return (WinningCondition)TOKEN_O;
		
		//Check draw
		if(IsDraw()) return DRAW; else return NO_WIN;
	}
	
public:
	ClassicBoard()
	{
		player = TOKEN_X;
		InitializeMatrix();
	}
	
	ClassicBoard(Token first)
	{
		player = first;
		InitializeMatrix();
	}
	
	~ClassicBoard()
	{
		cout << "Classic Game ended." << endl;
		system("PAUSE");
	}
	
	virtual void SwapPlayer()
	{
		switch(player)
		{
			case TOKEN_X:
				player = TOKEN_O;
				break;
			case TOKEN_O:
				player = TOKEN_X;
				break;
		}
	}
	
	virtual void Draw()
	{
		int startX = GetCursorPositionX();
		int startY = GetCursorPositionY();
		
		GoToXY(startX, startY + 0);
		cout << "-------" << endl;
		for(int i = 0; i < SIZE; i++)
		{
			GoToXY(startX, startY + i + 1);
			for(int j = 0; j < SIZE; j++)
			{
				cout << "|" << Board::GetTokenChar(matrix[i][j]);
			}
			cout << "|";
		}
		GoToXY(startX, startY + 4);
		cout << "-------";
	}
	
	virtual void GetInput()
	{
		cout << endl << endl;
		Board::ShowTurn(player);
		cout << " @ Mark " << Board::GetTokenChar(player) << " (1-9)" << endl;
		
		while(!PlaceToken(Choose(1, 9)))
			cout << " @ The space is occupied!" << endl;
	}
	
	void GetInputSpecial()
	{
		cout << " @ Mark " << Board::GetTokenChar(player) << " (1-9)" << endl;
		
		while(!PlaceToken(Choose(1, 9)))
			cout << " @ The space is occupied!" << endl;
	}
	
	virtual bool IsGameOver()
	{
		WinningCondition condition = CheckWinningCondition();
		
		if(condition == NO_WIN)
		{
			SwapPlayer();
			return false;
		}
		
		system("cls");
		Title();
		cout << endl;
		Draw();
		cout << endl;
		switch(condition)
		{
			case NO_WIN:
				break;
			case WIN_X:
				Board::AddScore(Board::GetFirstHand());
				cout << "X WIN!!!" << endl;
				break;
			case WIN_O:
				Board::AddScore(Board::GetSecondHand());
				cout << "O WIN!!!" << endl;
				break;
			case DRAW:
				cout << "DRAW!!!" << endl;
				break;
		}
		// Show Score
		Board::ShowScore(true);
		//Swap players' hand
		Board::SwapFirstHand();
		system("PAUSE");
		return true;
	}
};

class NotakoBoard : public ClassicBoard
{
protected:
	Player player;
	
	// InitializeMatrix()
	
	// PlaceToken(int mark)
	
	// IsDraw()
	
	bool NonDiagonalCheck(int i)
	{
		int count[2] = {0, 0};
		for(int j = 0; j < SIZE; j++)
		{
			//Horizontal Check
			if(matrix[i][j] == TOKEN_X) count[0]++;
			
			//Vertical Check
			if(matrix[j][i] == TOKEN_X) count[1]++;
		}
		
		//Is there a winner?
		if(count[0] >= SIZE || count[1] >= SIZE)
			return true;
		else
			return false;
	}
	
	bool DiagonalCheck()
	{
		int count[2] = {0, 0};
		for(int i = 0; i < SIZE; i++)
		{
			//Diagonal Check 1
			if(matrix[i][i] == TOKEN_X) count[0]++;
			
			//Diagonal Check 2
			if(matrix[SIZE-1-i][i] == TOKEN_X) count[1]++;
		}
		
		//Is there a winner?
		if(count[0] >= SIZE || count[1] >= SIZE)
			return true;
		else
			return false;
	}
	
	WinningCondition CheckWinningCondition()
	{		
		//For every row and column
		for(int i = 0; i < SIZE; i++)
		{
			//Do Non-Diagonal Check
			if(NonDiagonalCheck(i))
				return WIN_X;
		}
		
		//Do Diagonal Check
		if(DiagonalCheck())
			return WIN_X;
		
		//Check draw
		if(IsDraw()) return DRAW; else return NO_WIN;
	}
	
public:
	NotakoBoard()
	{
		player = Board::GetFirstHand();
		InitializeMatrix();
	}
	
	NotakoBoard(Player first)
	{
		player = first;
		InitializeMatrix();
	}
	
	~NotakoBoard()
	{
		cout << "Notako Game ended." << endl;
		system("PAUSE");
	}
	
	void SwapPlayer()
	{
		switch(player)
		{
			case PLAYER1:
				player = PLAYER2;
				break;
			case PLAYER2:
				player = PLAYER1;
				break;
		}
	}
	
	void Draw()
	{
		int startX = GetCursorPositionX();
		int startY = GetCursorPositionY();
		
		GoToXY(startX, startY + 0);
		cout << "-------" << endl;
		for(int i = 0; i < SIZE; i++)
		{
			GoToXY(startX, startY + i + 1);
			for(int j = 0; j < SIZE; j++)
			{
				cout << "|" << Board::GetTokenChar(matrix[i][j]);
			}
			cout << "|";
		}
		GoToXY(startX, startY + 4);
		cout << "-------";
	}
	
	void GetInput()
	{
		cout << endl << endl;
		Board::ShowTurn(player);
		cout << " @ Mark " << Board::GetTokenChar(TOKEN_X) << " (1-9)" << endl;
		
		while(!PlaceToken(Choose(1, 9)))
			cout << " @ The space is occupied!" << endl;
	}
	
	bool IsGameOver()
	{
		WinningCondition condition = CheckWinningCondition();
		
		if(condition == NO_WIN)
		{
			SwapPlayer();
			return false;
		}
		
		system("cls");
		Title();
		cout << endl;
		Draw();
		cout << endl;
		switch(condition)
		{
			case NO_WIN:
				break;
			case WIN_O:
			case WIN_X:
				Board::AddScore((player == PLAYER1 ? PLAYER2 : PLAYER1));
				cout << Board::GetPlayerName((player == PLAYER1 ? PLAYER2 : PLAYER1)) << " WIN!!!" << endl;
				break;
			case DRAW:
				cout << "DRAW!!!" << endl;
				break;
		}
		// Show Score
		Board::ShowScore(true);
		//Swap players' hand
		Board::SwapFirstHand();
		system("PAUSE");
		return true;
	}
};

class WildBoard : public ClassicBoard
{
protected:
	// Token** matrix;
	
	Player turn;
	
	// InitializeMatrix()
	
	// PlaceToken(int mark)
	
	// IsDraw()
	
	// NonDiagonalCheck(Token player, int i)
	
	// DiagonalCheck(Token player)
	
	// CheckWinningCondition()
	
public:
	WildBoard()
	{
		turn = Board::GetFirstHand();
		InitializeMatrix();
	}
	
	WildBoard(Player first)
	{
		turn = first;
		InitializeMatrix();
	}
	
	~WildBoard()
	{
		cout << "Wild Game ended." << endl;
		system("PAUSE");
	}
	
	void SwapPlayer()
	{
		switch(turn)
		{
			case PLAYER1:
				turn = PLAYER2;
				break;
			case PLAYER2:
				turn = PLAYER1;
				break;
		}
	}
	
	void Draw()
	{
		int startX = GetCursorPositionX();
		int startY = GetCursorPositionY();
		
		GoToXY(startX, startY + 0);
		cout << "-------" << endl;
		for(int i = 0; i < SIZE; i++)
		{
			GoToXY(startX, startY + i + 1);
			for(int j = 0; j < SIZE; j++)
			{
				cout << "|" << Board::GetTokenChar(matrix[i][j]);
			}
			cout << "|";
		}
		GoToXY(startX, startY + 4);
		cout << "-------";
	}
	
	void GetInput()
	{
		cout << endl << endl;
		Board::ShowTurn(turn);
		
		char marker;
		Token markToken;
		bool exit = false;
		
		cout << " @ Choose Marker (X or O)" << endl;
		
		do
		{
			markToken = BLANK;
			cout << " > ";
			cin >> marker;
			cout << endl;
			
			
			if(marker == 'x' || marker == 'X') markToken = TOKEN_X;
			else if(marker == 'o' || marker == 'O') markToken = TOKEN_O;
			else markToken = BLANK;
			
			exit = cin.good() && markToken != BLANK;
			
			if(!exit)
				cout << " @ Wrong Input!" << endl;
			
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		while(!exit);
		
		player = markToken;
		
		cout << " @ Mark " << Board::GetTokenChar(player) << " (1-9)" << endl;
		
		while(!PlaceToken(Choose(1, 9)))
			cout << " @ The space is occupied!" << endl;
	}
	
	bool IsGameOver()
	{
		WinningCondition condition = CheckWinningCondition();
		
		if(condition == NO_WIN)
		{
			SwapPlayer();
			return false;
		}
		
		system("cls");
		Title();
		cout << endl;
		Draw();
		cout << endl;
		switch(condition)
		{
			case NO_WIN:
				break;
			case WIN_X:
			case WIN_O:
				Board::AddScore(turn);
				cout << Board::GetPlayerName(turn) << " WIN!!!" << endl;
				break;
			case DRAW:
				cout << "DRAW!!!" << endl;
				break;
		}
		// Show Score
		Board::ShowScore(true);
		//Swap players' hand
		Board::SwapFirstHand();
		system("PAUSE");
		return true;
	}
};

class UltimateBoard : public ClassicBoard
{
protected:
	ClassicBoard ultimateMatrix[SIZE][SIZE];
	
	bool IsDraw()
	{
		for(int i = 0; i < SIZE; i++)
		{
			for(int j = 0; j < SIZE; j++)
			{
				if(ultimateMatrix[i][j].CheckWinningCondition() == NO_WIN)
				{
					return false;
				}
			}
		}
		return true;
	}
	
	bool NonDiagonalCheck(Token player, int i)
	{
		int count[2] = {0, 0};
		for(int j = 0; j < SIZE; j++)
		{
			//Horizontal Check
			if(ultimateMatrix[i][j].CheckWinningCondition() == (WinningCondition)player) count[0]++;
			
			//Vertical Check
			if(ultimateMatrix[j][i].CheckWinningCondition() == (WinningCondition)player) count[1]++;
		}
		
		//Is there a winner?
		if(count[0] >= SIZE || count[1] >= SIZE)
			return true;
		else
			return false;
	}
	
	bool DiagonalCheck(Token player)
	{
		int count[2] = {0, 0};
		for(int i = 0; i < SIZE; i++)
		{
			//Diagonal Check 1
			if(ultimateMatrix[i][i].CheckWinningCondition() == (WinningCondition)player) count[0]++;
			
			//Diagonal Check 2
			if(ultimateMatrix[SIZE-1-i][i].CheckWinningCondition() == (WinningCondition)player) count[1]++;
		}
		
		//Is there a winner?
		if(count[0] >= SIZE || count[1] >= SIZE)
			return true;
		else
			return false;
	}
	
	//Make sure the player variable follows the ultimate board's
	void SyncPlayers()
	{
		for(int i = 0; i < SIZE; i++)
		{
			for(int j = 0; j < SIZE; j++)
			{
				ultimateMatrix[i][j].player = this->player;
			}
		}
	}

	// No changes for CheckWinningCondition() & GetInput()
public:
	bool SelectBox(int box)
	{
		int x = (box - 1) % SIZE;
		int y = (box - 1) / SIZE;
		
		if(ultimateMatrix[y][x].CheckWinningCondition() == NO_WIN)
		{
			ultimateMatrix[y][x].GetInputSpecial();
			return true;
		}
		else
		{
			return false;
		}
	}
	
	UltimateBoard() : ClassicBoard(){}
	
	UltimateBoard(Token first) : ClassicBoard(first){}
	
	~UltimateBoard()
	{
		cout << "Ultimate Game ended." << endl;
		system("PAUSE");
	}
	
	//No changes for GetTokenChar()
	
	//No changes for SwapPlayer()
	
	void Draw()
	{
		int startX = GetCursorPositionX();
		int startY = GetCursorPositionY();
		
		for(int i = 0; i < SIZE; i++)
		{
			for(int j = 0; j < SIZE; j++)
			{
				GoToXY(startX + j * 6, startY + i * 4);
				WinningCondition condition = ultimateMatrix[i][j].CheckWinningCondition();
				if(condition == NO_WIN || condition == DRAW)
				{
					ultimateMatrix[i][j].Draw();
				}
				else
				{
					int x = GetCursorPositionX();
					int y = GetCursorPositionY();
					GoToXY(startX, startY + 0);
					cout << "-------";
					GoToXY(startX, startY + 1);
					cout << "|     |";
					GoToXY(startX, startY + 2);
					cout << "|  " << Board::GetTokenChar((Token)condition) << "  |";
					GoToXY(startX, startY + 3);
					cout << "|     |";
					GoToXY(startX, startY + 4);
					cout << "-------";
				}
			}
		}
	}
	
	void GetInput()
	{
		SyncPlayers();
		cout << endl << endl;
		Board::ShowTurn(player);
		cout << " @ Select Box (1-9)" << endl;
		
		while(!SelectBox(Choose(1, 9)))
			cout << " @ The box has been completed!" << endl;
	}
	
	//No changes for IsGameOver()
};

class QuantumBoard : public ClassicBoard
{
protected:
	int sequence;
	// Token** matrix;
	vector<Token>** quantumMatrix;
	vector<int>** quantumSequence;
	
	void InitializeMatrix()
	{
		sequence = 0;
		
		matrix = new Token*[SIZE];
		for(int i = 0; i < SIZE; i++)
		{
			matrix[i] = new Token[SIZE];
			for(int j = 0; j < SIZE; j++)
			{
				matrix[i][j] = BLANK;
			}
		}
		
		quantumMatrix = new vector<Token>*[SIZE];
		for(int i = 0; i < SIZE; i++)
		{
			quantumMatrix[i] = new vector<Token>[SIZE];
		}
		
		quantumSequence = new vector<int>*[SIZE];
		for(int i = 0; i < SIZE; i++)
		{
			quantumSequence[i] = new vector<int>[SIZE];
		}
	}
	
	//No Changes for IsDraw()
	
	//No Changes for NonDiagonalCheck()
	
	//No Changes for DiagonalCheck()
	
	//No Changes for CheckWinningCondition()
	
	//Deprecated - Don't use this function
	DEPRECATED virtual bool PlaceToken(int mark)
	{
		return false;
	}
	
	virtual bool PlaceToken(int firstMark, int secondMark)
	{
		if(firstMark == secondMark)
			return false;
		
		int firstX = (firstMark - 1) % SIZE;
		int firstY = (firstMark - 1) / SIZE;
		
		if(matrix[firstY][firstX] != BLANK)
			return false;
		
		int secondX = (secondMark - 1) % SIZE;
		int secondY = (secondMark - 1) / SIZE;
		
		if(matrix[secondY][secondX] != BLANK)
			return false;
		
		quantumMatrix[firstY][firstX].push_back(player);
		quantumSequence[firstY][firstX].push_back(sequence);
		
		quantumMatrix[secondY][secondX].push_back(player);
		quantumSequence[secondY][secondX].push_back(sequence);
		
		return true;
	}
	
	//virtual void CheckCylic(int seq);
	
	//virtual void Observe();
public:
	QuantumBoard() : ClassicBoard()
	{
		InitializeMatrix();
	}
	
	QuantumBoard(Token first) : ClassicBoard(first)
	{
		InitializeMatrix();
	}
	
	~QuantumBoard()
	{
		cout << "Quantum Game ended." << endl;
		system("PAUSE");
	}
	
	//No Changes for GetTokenChar()
	
	//No Changes for SwapPlayer()
	
	virtual void Draw()
	{
		int startX = GetCursorPositionX();
		int startY = GetCursorPositionY();
		
		for(int i = 0; i < SIZE; i++)
		{
			for(int j = 0; j < SIZE; j++)
			{				
				int x = startX + j * 9;
				int y = startY + i * 4;
				
				GoToXY(x, y + 0);
				cout << "----------";
				
				if(matrix[i][j] == BLANK)
				{
					for(int m = 0; m < SIZE; m++)
					{
						GoToXY(x, y + m + 1);
						for(int n = 0; n < SIZE; n++)
						{
							cout << "|";
							if(m*3 + n < quantumMatrix[i][j].size())
								cout << Board::GetTokenChar(quantumMatrix[i][j][m*3 + n]) << quantumSequence[i][j][m*3 + n];
							else
								cout << "  ";
						}
						cout << "|";
					}
				}
				else
				{
					GoToXY(x, y + 1);
					cout << "|        |";
					GoToXY(x, y + 2);
					cout << "|   " << Board::GetTokenChar(matrix[i][j]) << "    |";
					GoToXY(x, y + 3);
					cout << "|        |";
				}
				
				GoToXY(x, y + 4);
				cout << "----------";
			}
		}
	}
	
	virtual void GetInput()
	{
		cout << endl << endl;
		Board::ShowTurn(player);
		
		cout << " @ Mark 2 " << Board::GetTokenChar(player) << "s (1-9)" << endl;
		
		while(!PlaceToken(Choose(1, 9), Choose(1, 9)))
			cout << " @ Both markers cannot be the same!" << endl;
		
		sequence++;
	}
	
	//No Changes for IsGameOver()
};

Board* CreateNewBoard(BoardType type)
{
	switch(type)
	{
		case CLASSIC:
			return new ClassicBoard();
			break;
		case NOTAKO:
			return new NotakoBoard();
			break;
		case WILD:
			return new WildBoard();
			break;
		case ULTIMATE:
			return new UltimateBoard();
			break;
		case QUANTUM:
			return new QuantumBoard();
			break;
		default:
			return NULL;
			break;
	}
}

void ResetBoard()
{
	system("CLS");
	Board::Title();
	cout << endl;
	cout << "Initializing Board..." << endl << endl;
	Board::firstHandPlayer = PLAYER1;
	for(int i = 0; i < TOTAL_PLAYERS; i++)
	{
		Board::score[i] = 0;
		cout << "Player " << i + 1 << "\'s Name" << endl;
		cout << "> ";
		getline(cin, Board::playerNames[i]);
		cout << endl;
	}
	cout << "Board initialized!" << endl;
	system("PAUSE");
}

void PlayGame(Board* &board)
{
	do
	{
		system("CLS");
		Board::Title();
		cout << endl;
		board->Draw();
		board->GetInput();
	}
	while(!board->IsGameOver());
}

void DoChoice(int choice, Board* &board)
{
	switch(choice)
	{
		case 1:
			board = CreateNewBoard(CLASSIC);
			break;
		case 2:
			board = CreateNewBoard(NOTAKO);
			break;
		case 3:
			board = CreateNewBoard(WILD);
			break;
		case 4:
			board = CreateNewBoard(ULTIMATE);
			break;
		case 5:
			board = CreateNewBoard(QUANTUM);
			break;
		case 0:
		default:
			return;
			break;
	}
	PlayGame(board);
}

int main()
{
	int choice = -1;
	do
	{
		Board* board;
		ResetBoard();
		do
		{
			system("CLS");
			Board::Title();
			cout << endl;
			Board::ShowScore(false);
			cout << endl;
			cout << "==========================================" << endl;
			cout << "  1. Classic" << endl;
			cout << "  2. Notako" << endl;
			cout << "  3. Wild" << endl;
			cout << "  4. Ultimate" << endl;
			cout << "  5. Quantum (Deprecated)" << endl;
			cout << endl;
			cout << "  0. Reset Board" << endl;
			cout << " -1. Exit Program" << endl;
			cout << "==========================================" << endl;
			cout << endl;
			cout << " @ What would you like to play?" << endl;
			choice = Choose(-1, TOTAL_BOARD_TYPE);
			
			DoChoice(choice, board);
		}
		while(choice > 0);
	}
	while(choice != -1);
	
	return 0;
}
