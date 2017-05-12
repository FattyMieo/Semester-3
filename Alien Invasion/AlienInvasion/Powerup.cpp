#include "AllHeaderFiles.h"

Powerup::Powerup(POWERUP_TYPE type) : GameObject()
{
	if(type == REGEN)
	{
		row = 3;
		column = 3;
		
		char tempCharArray[3][3] = 
		{
			{'/','|','\\'},
			{'=','0','='},
			{'\\','|','/'}
		};
		
		charArray = new char*[row];
		for(int i = 0; i < row; i++)
		{
			charArray[i] = new char[column];
			for(int j = 0; j < column; j++)
			{
				charArray[i][j] = tempCharArray[i][j];
			}
		}
		
		colour = LIGHTGREEN;
		x = 20;
		y = 10;
	}
	else if(type == ARMOR)
	{
		row = 3;
		column = 3;
		
		char tempCharArray[3][3] = 
		{
			{' ','^',' '},
			{'<','@','>'},
			{' ','v',' '}
		};
		
		charArray = new char*[row];
		for(int i = 0; i < row; i++)
		{
			charArray[i] = new char[column];
			for(int j = 0; j < column; j++)
			{
				charArray[i][j] = tempCharArray[i][j];
			}
		}
		
		colour = LIGHTGRAY;
		x = 20;
		y = 10;
	}
	else if(type == PENETRATE)
	{
		row = 1;
		column = 5;
		
		char tempCharArray[1][5] = 
		{
			{'<','<','@','>','>'}
		};
		
		charArray = new char*[row];
		for(int i = 0; i < row; i++)
		{
			charArray[i] = new char[column];
			for(int j = 0; j < column; j++)
			{
				charArray[i][j] = tempCharArray[i][j];
			}
		}
		
		colour = YELLOW;
		x = 20;
		y = 10;
	}
	
	updateDelayDuration = 5;
	updateDelayTimer = 0;
	
	this->type = type;
	isAlive = true;
}

void Powerup::Update()
{
	updateDelayTimer++;
	
	if(updateDelayTimer < updateDelayDuration)
		return;
		
	updateDelayTimer = 0;
	
	if(!CheckBottomBoundary())
		MoveDown();
	else
		isAlive = false;
}
