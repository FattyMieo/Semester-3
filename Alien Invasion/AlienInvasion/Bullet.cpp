#include "AllHeaderFiles.h"

Bullet::Bullet(BULLET_TYPE type) : GameObject()
{
	if(type == STRAIGHT)
	{
		row = 1;
		column = 1;
		
		char tempCharArray[1][1] = 
		{
			{'!'}
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
		
		colour = LIGHTCYAN;
		x = 20;
		y = 15;
		damage = 2;
	}
	else if(type == DUAL)
	{
		row = 3;
		column = 9;
		
		char tempCharArray[3][9] = 
		{
			{' ',' ',' ',' ','I',' ',' ',' ',' '},
			{' ','_','/',']','=','[','\\','_',' '},
			{'/',' ',' ',' ',' ',' ',' ',' ','\\'}
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
		
		colour = CYAN;
		x = 20;
		y = 15;
		damage = 2;
	}
	
	this->type = type;
	isAlive = true;
}

EnemyBullet::EnemyBullet(BULLET_TYPE type) : Bullet(type)
{
	if(colour == CYAN)
	{
		colour = MAGENTA;
	}
	else if(colour == LIGHTCYAN)
	{
		colour = LIGHTMAGENTA;
	}
}

void Bullet::Update()
{
	if(!CheckTopBoundary())
		MoveUp();
	else
		isAlive = false;
}

void EnemyBullet::Update()
{
	if(!CheckBottomBoundary())
		MoveDown();
	else
		isAlive = false;
}
