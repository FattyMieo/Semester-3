#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include "conio_nt.h"

using namespace std;

enum GRID_TYPE
{
	G_EMPTY = 0,
	G_SOURCE,
	G_TARGET,
	G_VERT,
	G_HORI,
	G_L_0,
	G_L_90,
	G_L_180,
	G_L_270,
	G_T_0,
	G_T_90,
	G_T_180,
	G_T_270
};

const int GRID_SIZE_X = 9;
const int GRID_SIZE_Y = 6;
const int SPRITE_SIZE = 3;

enum GameState
{
	RUNNING = 0,
	WIN,
	LOSE
};

enum NodeType
{
	EMPTY = 0,
	SOURCE,
	TARGET,
	WIRE
};

enum Direction
{
	EAST = 0,
	SOUTH,
	WEST,
	NORTH,
	TOTAL_DIRECTIONS,
	NO_DIR
};

Direction ReverseDirection(Direction dir)
{
	switch(dir)
	{
		case EAST:
			return WEST;
		case WEST:
			return EAST;
		case NORTH:
			return SOUTH;
		case SOUTH:
			return NORTH;
		default:
			return NO_DIR;
	}
}

class Node
{
public:
	Node** neighbours;
	bool* connected;
	bool powered;
	int bombTimer; //-1 for no bomb, 0 for exploded, > 0 for remaining turns for explosion
	
	NodeType type;
	char** sprite;
	
	void Initialize()
	{
		neighbours = new Node*[TOTAL_DIRECTIONS];
		connected = new bool[TOTAL_DIRECTIONS];
		for(int i = 0; i < TOTAL_DIRECTIONS; i++)
		{
			neighbours[i] = NULL;
			connected[i] = false;
		}
		
		powered = false;
		
		bombTimer = -1;
		
		type = EMPTY;
		
		sprite = new char*[SPRITE_SIZE];
		for(int i = 0; i < SPRITE_SIZE; i++)
		{
			sprite[i] = new char[SPRITE_SIZE * 2];
		}
		
		SetSprite();
	}
	
	void SetSprite()
	{
		char tmpSprite[SPRITE_SIZE][SPRITE_SIZE] =
		{
			{' ',' ',' '},
			{' ',' ',' '},
			{' ',' ',' '}
		};
		
		switch(type)
		{
			case EMPTY:
				break;
			case WIRE:
				tmpSprite[SPRITE_SIZE / 2][SPRITE_SIZE / 2] = char(219);
				if(connected[NORTH]) tmpSprite[0][1] = char(219);
				if(connected[SOUTH]) tmpSprite[2][1] = char(219);
				if(connected[EAST]) tmpSprite[1][2] = char(219);
				if(connected[WEST]) tmpSprite[1][0] = char(219);
				break;
			case SOURCE:
				for(int i = 0; i < SPRITE_SIZE; i++)
				{
					for(int j = 0; j < SPRITE_SIZE; j++)
					{
						tmpSprite[i][j] = char(219);
					}
				}
				break;
			case TARGET:
				for(int i = 0; i < SPRITE_SIZE; i++)
				{
					for(int j = 0; j < SPRITE_SIZE; j++)
					{
						tmpSprite[i][j] = char(177);
					}
				}
				break;
			default:
				type = EMPTY;
				break;
		}
		
		for(int i = 0; i < SPRITE_SIZE; i++)
		{
			for(int j = 0; j < SPRITE_SIZE; j++)
			{
				sprite[i][j * 2] = tmpSprite[i][j];
				sprite[i][j * 2 + 1] = tmpSprite[i][j];
			}
		}
	}
	
	bool isExploded()
	{
		return bombTimer == 0;
	}
	
	void RotateNode(bool isClockwise = true)
	{
		if(type == WIRE)
		{
			// Clockwise
			if(isClockwise)
			{
				bool tmp = connected[TOTAL_DIRECTIONS - 1];
				
				for(int i = TOTAL_DIRECTIONS - 1; i > 0; i--)
				{
					connected[i] = connected[i - 1];
				}
				
				connected[0] = tmp;
			}
			// Counterclockwise
			else
			{
				bool tmp = connected[0];
				
				for(int i = 0; i < TOTAL_DIRECTIONS - 1; i++)
				{
					connected[i] = connected[i + 1];
				}
				
				connected[TOTAL_DIRECTIONS - 1] = tmp;
			}
			
			SetSprite();
		}
	}
	
	Node()
	{
		Initialize();
	}
};

void InjectPower(Node* node, Direction source)
{
	node->powered = true;
	
	for(int i = 0; i < TOTAL_DIRECTIONS; i++)
	{
		//Conditions:	1. Direction is not source
		//				2. Node can connect
		if((Direction)i != source && node->connected[i])
		{
			Node* neighbour = node->neighbours[i];
			
			//Conditions:	1. Neighbour is avaialble
			//				2. Neighbour is not powered
			//				3. Neighbour can connect
			if(neighbour != NULL && !neighbour->powered && neighbour->connected[ReverseDirection((Direction)i)])
			{
				InjectPower(neighbour, ReverseDirection((Direction)i));
			}
		}
	}
}

GameState RelinkPower(Node** &grid)
{
	for(int cycle = 0; cycle < 3; cycle++)
	{
		for(int i = 0; i < GRID_SIZE_Y; i++)
		{
			for(int j = 0; j < GRID_SIZE_X; j++)
			{
				switch(cycle)
				{
					case 0: //Power off the system
						grid[i][j].powered = false;
						break;
					case 1: //Inject power into the system
						if(grid[i][j].type == SOURCE)
							InjectPower(&grid[i][j], NO_DIR);
						break;
					case 2: //Check whether all targets are powered
						if(grid[i][j].type == TARGET && !grid[i][j].powered)
							//One of the targets is unpowered, relink unsuccessful
							return RUNNING;
						break;
				}
			}
		}
	}
	//System is online, relink successful
	return WIN;
}

void InitializeGrid(Node** &grid)
{
	grid = new Node*[GRID_SIZE_Y];
	for(int i = 0; i < GRID_SIZE_Y; i++)
	{
		grid[i] = new Node[GRID_SIZE_X];
	}
	
	for(int i = 0; i < GRID_SIZE_Y; i++)
	{
		for(int j = 0; j < GRID_SIZE_X; j++)
		{
			if(i - 1 >= 0) grid[i][j].neighbours[NORTH] = &grid[i - 1][j];
			if(i + 1 < GRID_SIZE_Y) grid[i][j].neighbours[SOUTH] = &grid[i + 1][j];
			if(j - 1 >= 0) grid[i][j].neighbours[WEST] = &grid[i][j - 1];
			if(j + 1 < GRID_SIZE_X) grid[i][j].neighbours[EAST] = &grid[i][j + 1];
		}
	}
}

void ImportGrid(Node** &grid, GRID_TYPE tempGrid[GRID_SIZE_Y][GRID_SIZE_X], int bombGrid[GRID_SIZE_Y][GRID_SIZE_X])
{
	InitializeGrid(grid);
	for(int i = 0; i < GRID_SIZE_Y; i++)
	{
		for(int j = 0; j < GRID_SIZE_X; j++)
		{
			grid[i][j].bombTimer = bombGrid[i][j];
			switch(tempGrid[i][j])
			{
				case G_EMPTY:
					grid[i][j].type = EMPTY;
					break;
				case G_SOURCE:
					grid[i][j].type = SOURCE;
					break;
				case G_TARGET:
					grid[i][j].type = TARGET;
					break;
				default:
					grid[i][j].type = WIRE;
					break;
			}
			switch(grid[i][j].type)
			{
				case WIRE:
					switch(tempGrid[i][j])
					{
						case G_VERT:
							grid[i][j].connected[NORTH] = true;
							grid[i][j].connected[SOUTH] = true;
							break;
						case G_HORI:
							grid[i][j].connected[EAST] = true;
							grid[i][j].connected[WEST] = true;
							break;
						case G_L_0:
							grid[i][j].connected[NORTH] = true;
							grid[i][j].connected[EAST] = true;
							break;
						case G_L_90:
							grid[i][j].connected[EAST] = true;
							grid[i][j].connected[SOUTH] = true;
							break;
						case G_L_180:
							grid[i][j].connected[SOUTH] = true;
							grid[i][j].connected[WEST] = true;
							break;
						case G_L_270:
							grid[i][j].connected[WEST] = true;
							grid[i][j].connected[NORTH] = true;
							break;
						case G_T_0:
							grid[i][j].connected[EAST] = true;
							grid[i][j].connected[SOUTH] = true;
							grid[i][j].connected[WEST] = true;
							break;
						case G_T_90:
							grid[i][j].connected[SOUTH] = true;
							grid[i][j].connected[WEST] = true;
							grid[i][j].connected[NORTH] = true;
							break;
						case G_T_180:
							grid[i][j].connected[WEST] = true;
							grid[i][j].connected[NORTH] = true;
							grid[i][j].connected[EAST] = true;
							break;
						case G_T_270:
							grid[i][j].connected[NORTH] = true;
							grid[i][j].connected[EAST] = true;
							grid[i][j].connected[SOUTH] = true;
							break;
						default:
							break;
					}
					break;
				case SOURCE:
				case TARGET:
					for(int d = 0; d < TOTAL_DIRECTIONS; d++)
					{
						grid[i][j].connected[d] = true;
					}
					break;
				default:
					break;
			}
			grid[i][j].SetSprite();
		}
	}
}

void DrawGrid(int playerPos[2], Node** &grid)
{
	for(int i = 0; i < GRID_SIZE_Y; i++)
	{
		for(int j = 0; j < GRID_SIZE_X; j++)
		{
			int currentX = j*(SPRITE_SIZE*2);
			int currentY = i*(SPRITE_SIZE);
			
			if(i == playerPos[1] && j == playerPos[0])
			{
				SetBackColor(RED);
			}
			
			switch(grid[i][j].type)
			{
				case WIRE:
					if(grid[i][j].powered) SetForeColor(YELLOW);
					else SetForeColor(CYAN);
					break;
				case SOURCE:
					if(grid[i][j].powered) SetForeColor(YELLOW);
					else SetForeColor(DARKYELLOW);
					break;
				case TARGET:
					if(grid[i][j].powered) SetForeColor(YELLOW);
					else SetForeColor(GREEN);
					break;
			}
			
	    	if(grid[i][j].bombTimer == 0)
	    	{
				SetForeColor(YELLOW);
				SetBackColor(BLACK);
			    for(int y = 0; y < SPRITE_SIZE; y++)
			    {
			    	GoToXY(currentX, currentY + y);
				    for(int x = 0; x < SPRITE_SIZE*2; x++)
				    {
			    		cout << char(rand()%(223-179) + 179);
			    	}
				}
			}
			else
			{
			    for(int y = 0; y < SPRITE_SIZE; y++)
			    {
			    	GoToXY(currentX, currentY + y);
				    for(int x = 0; x < SPRITE_SIZE*2; x++)
				    {
			    		cout << grid[i][j].sprite[y][x];
			    	}
				}
				
			    if(grid[i][j].bombTimer > 0)
			    {
			    	GoToXY(currentX + SPRITE_SIZE - 1, currentY + SPRITE_SIZE / 2);
					SetForeColor(WHITE);
			    	if(grid[i][j].bombTimer == 1)
			    	{
						SetBackColor(DARKRED);
			    		cout << " " << ".";
					}
			    	else if(grid[i][j].bombTimer == 2)
			    	{
						SetBackColor(DARKYELLOW);
			    		cout << "." << ".";
					}
			    	else if(grid[i][j].bombTimer == 3)
			    	{
						SetBackColor(DARKGREEN);
			    		cout << ":" << ".";
					}
			    	else if(grid[i][j].bombTimer == 4)
			    	{
						SetBackColor(DARKGREEN);
			    		cout << ":" << ":";
					}
			    	else
			    	{
						SetBackColor(CYAN);
						SetForeColor(BLUE);
			    		cout << "X" << grid[i][j].bombTimer;
					}
			    }
			}
			
			SetForeColor(GRAY);
			SetBackColor(BLACK);
		}
	}
}

const int borderLength = 33;
const int borderHeight = 4;

const int startX = (GRID_SIZE_X * SPRITE_SIZE) - (borderLength / 2);
const int startY = (GRID_SIZE_Y * SPRITE_SIZE / 2) - (borderHeight / 2);

void DrawBorder()
{
	GoToXY(startX, startY);
	cout << "=================================";
	GoToXY(startX, startY + 1);
	cout << "|                               |";
	GoToXY(startX, startY + 2);
	cout << "|                               |";
	GoToXY(startX, startY + 3);
	cout << "=================================";
}

void DrawWelcome()
{
	DrawBorder();
	
	GoToXY(startX + 1, startY + 1);
	cout << "          Pipe Puzzle          ";
	GoToXY(startX + 1, startY + 2);
	cout << "     Press any key to play     ";
	GoToXY(startX + 27, startY + 2);
	getch();
}

void DrawStageTutorial(int stage)
{
	DrawBorder();
	
	switch(stage)
	{
		case 0:
			GoToXY(startX + 1, startY + 1);
			cout << "  WASD to Move | QE to Rotate  ";
			GoToXY(startX + 1, startY + 2);
			cout << "    Press any key to start     ";
			GoToXY(startX + 27, startY + 2);
			getch();
			break;
		case 1:
			GoToXY(startX + 1, startY + 1);
			cout << "  Pipe with dots may explode!  ";
			GoToXY(startX + 1, startY + 2);
			cout << "    Press any key to start     ";
			GoToXY(startX + 27, startY + 2);
			getch();
			break;
		case 2:
			GoToXY(startX + 1, startY + 1);
			cout << "      Watch your steps ...     ";
			GoToXY(startX + 1, startY + 2);
			cout << "    Press any key to start     ";
			GoToXY(startX + 27, startY + 2);
			getch();
			break;
	}
}

void DrawStageClear()
{
	DrawBorder();
	
	GoToXY(startX + 1, startY + 1);
	cout << "         Stage Cleared         ";
	GoToXY(startX + 1, startY + 2);
	cout << "   Press any key to continue   ";
	GoToXY(startX + 29, startY + 2);
	getch();
}

void DrawDefeat()
{
	DrawBorder();
	
	GoToXY(startX + 1, startY + 1);
	cout << "            Defeated           ";
	GoToXY(startX + 1, startY + 2);
	cout << "   Press any key to continue   ";
	GoToXY(startX + 29, startY + 2);
	getch();
}

bool GameContinue()
{
	system("CLS");
	
	DrawBorder();
	
	GoToXY(startX + 1, startY + 1);
	cout << "           Game Over           ";
	GoToXY(startX + 1, startY + 2);
	cout << "     Play again?     Y / N     ";
	
	while(true)
	{
		GoToXY(startX + 22, startY + 3);
		char input = getch();
		if(input == 'Y' || input == 'y') return true;
		else if(input == 'N' || input == 'n') return false;
	}
}

GameState PlayerInput(int playerPos[2], Node** &grid)
{
	char input = getch();
	
	     if(input == 'W' || input == 'w' && playerPos[1] > 0) playerPos[1]--;
	else if(input == 'S' || input == 's' && playerPos[1] < GRID_SIZE_Y - 1) playerPos[1]++;
	else if(input == 'A' || input == 'a' && playerPos[0] > 0) playerPos[0]--;
	else if(input == 'D' || input == 'd' && playerPos[0] < GRID_SIZE_X - 1) playerPos[0]++;
	else if(input == 'Q' || input == 'q')
	{
		grid[playerPos[1]][playerPos[0]].RotateNode(false);
		
		grid[playerPos[1]][playerPos[0]].bombTimer--;
		if(grid[playerPos[1]][playerPos[0]].isExploded()) return LOSE;
		
		return RelinkPower(grid);
	}
	else if(input == 'E' || input == 'e')
	{
		grid[playerPos[1]][playerPos[0]].RotateNode(true);
		
		grid[playerPos[1]][playerPos[0]].bombTimer--;
		if(grid[playerPos[1]][playerPos[0]].isExploded()) return LOSE;
		
		return RelinkPower(grid);
	}
	return RUNNING;
}

const int TOTAL_LEVELS = 3;

GRID_TYPE levels[TOTAL_LEVELS][GRID_SIZE_Y][GRID_SIZE_X] =
{
	//Level 0
	{
	    {G_TARGET	,G_T_0		,G_L_270	,G_EMPTY	,G_EMPTY	,G_EMPTY	,G_L_270	,G_HORI		,G_TARGET	},
	    {G_EMPTY	,G_EMPTY	,G_HORI		,G_L_270	,G_L_0		,G_EMPTY	,G_T_0		,G_EMPTY	,G_EMPTY	},
	    {G_EMPTY	,G_L_270	,G_T_0		,G_T_0		,G_T_0		,G_T_0		,G_T_0		,G_EMPTY	,G_EMPTY	},
	    {G_TARGET	,G_T_0		,G_L_270	,G_L_270	,G_EMPTY	,G_L_270	,G_HORI		,G_L_270	,G_EMPTY	},
	    {G_EMPTY	,G_T_0		,G_L_270	,G_HORI		,G_T_0		,G_L_270	,G_L_270	,G_L_270	,G_TARGET	},
	    {G_EMPTY	,G_L_270	,G_L_270	,G_EMPTY	,G_SOURCE	,G_EMPTY	,G_EMPTY	,G_EMPTY	,G_EMPTY	}
	},
	//Level 1
	{
	    {G_TARGET	,G_T_0		,G_L_270	,G_EMPTY	,G_EMPTY	,G_EMPTY	,G_L_270	,G_HORI		,G_TARGET	},
	    {G_EMPTY	,G_EMPTY	,G_HORI		,G_L_270	,G_L_0		,G_EMPTY	,G_T_0		,G_EMPTY	,G_EMPTY	},
	    {G_EMPTY	,G_L_270	,G_T_0		,G_T_0		,G_T_0		,G_T_0		,G_T_0		,G_EMPTY	,G_EMPTY	},
	    {G_TARGET	,G_T_0		,G_L_270	,G_L_270	,G_EMPTY	,G_L_270	,G_HORI		,G_L_270	,G_EMPTY	},
	    {G_EMPTY	,G_T_0		,G_L_270	,G_HORI		,G_T_0		,G_L_270	,G_L_270	,G_L_270	,G_TARGET	},
	    {G_EMPTY	,G_L_270	,G_L_270	,G_EMPTY	,G_SOURCE	,G_EMPTY	,G_EMPTY	,G_EMPTY	,G_EMPTY	}
	},
	//Level 2
	{
	    {G_TARGET	,G_T_0		,G_L_270	,G_EMPTY	,G_EMPTY	,G_EMPTY	,G_L_270	,G_HORI		,G_TARGET	},
	    {G_EMPTY	,G_EMPTY	,G_HORI		,G_L_270	,G_L_0		,G_EMPTY	,G_T_0		,G_EMPTY	,G_EMPTY	},
	    {G_EMPTY	,G_L_270	,G_T_0		,G_T_0		,G_T_0		,G_T_0		,G_T_0		,G_EMPTY	,G_EMPTY	},
	    {G_TARGET	,G_T_0		,G_L_270	,G_L_270	,G_EMPTY	,G_L_270	,G_HORI		,G_L_270	,G_EMPTY	},
	    {G_EMPTY	,G_T_0		,G_L_270	,G_HORI		,G_T_0		,G_L_270	,G_L_270	,G_L_270	,G_TARGET	},
	    {G_EMPTY	,G_L_270	,G_L_270	,G_EMPTY	,G_SOURCE	,G_EMPTY	,G_EMPTY	,G_EMPTY	,G_EMPTY	}
	}
};

int bombs[TOTAL_LEVELS][GRID_SIZE_Y][GRID_SIZE_X] =
{
	//Level 0
	{
	    {-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			},
	    {-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			},
	    {-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			},
	    {-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			},
	    {-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			},
	    {-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			,-1			}
	},
	//Level 1
	{
	    {-1			,2			,2			,-1			,-1			,-1			,3			,2			,-1			},
	    {-1			,-1			,2			,3			,3			,-1			,2			,-1			,-1			},
	    {-1			,3			,3			,2			,2			,4			,2			,-1			,-1			},
	    {-1			,3			,2			,3			,-1			,3			,3			,2			,-1			},
	    {-1			,2			,2			,2			,3			,2			,4			,2			,-1			},
	    {-1			,3			,3			,-1			,-1			,-1			,-1			,-1			,-1			}
	},
	//Level 2
	{
	    {-1			,2			,2			,-1			,-1			,-1			,3			,2			,-1			},
	    {-1			,-1			,2			,2			,2			,-1			,2			,-1			,-1			},
	    {-1			,2			,2			,2			,2			,1			,2			,-1			,-1			},
	    {-1			,2			,1			,1			,-1			,2			,2			,2			,-1			},
	    {-1			,2			,3			,2			,2			,2			,2			,2			,-1			},
	    {-1			,2			,2			,-1			,-1			,-1			,-1			,-1			,-1			}
	}
};

int main()
{
	//Settings
	SetWindowSize(GRID_SIZE_X * (SPRITE_SIZE * 2 + 1) - 2, GRID_SIZE_Y * (SPRITE_SIZE + 1) - 4);
	SetFontSize(16);
	HideCursor();
	SetQuickEditMode(false);
	srand(time(NULL));
	
	int playerPos[2];
	Node** grid;
	
	do
	{
	    playerPos[0] = 0;
	    playerPos[1] = 0;
	    
		system("CLS");
		DrawWelcome();
	    
		for(int i = 0; i < TOTAL_LEVELS; i++)
		{
		    ImportGrid(grid, levels[i], bombs[i]);
		    RelinkPower(grid);
		    
		    GameState gameState = RUNNING;
		    
			system("CLS");
		    DrawStageTutorial(i);
		    
			do
			{
				system("CLS");
			    DrawGrid(playerPos, grid);
			    gameState = PlayerInput(playerPos, grid);
			}
			while(gameState == RUNNING);
			
			system("CLS");
			DrawGrid(playerPos, grid);
			if(gameState == LOSE)
			{
				DrawDefeat();
				break;
			}
			else if(gameState == WIN)
			{
				DrawStageClear();
			}
		}
	}
	while(GameContinue());
	
	return 0;
}
