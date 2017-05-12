#include "AllHeaderFiles.h"

Enemy::Enemy(ENEMY_TYPE enemyType) : GameObject()
{
	if(enemyType == CRASHER)
	{
		row = 3;
		column = 3;
		
		char tempCharArray[3][3] = 
		{
			{'#','X','#'},
			{'X','X','X'},
			{'0','|','0'}
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
		
		colour = LIGHTRED;
		x = 20;
		y = 10;
		
		updateDelayDuration = 5;
		shootDelayDuration = -1;
		
		bulletType = STRAIGHT;
		
		health = 10;
	}
	else if(enemyType == SHOOTER)
	{
		row = 3;
		column = 7;
		
		char tempCharArray[3][7] = 
		{
			{'=','0','X','^','X','0','='},
			{'|','=','<','@','>','=','|'},
			{' ','|','0','v','0','|',' '}
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
		
		colour = RED;
		x = 20;
		y = 10;
		
		updateDelayDuration = 10;
		shootDelayDuration = 0;
		
		bulletType = STRAIGHT;
		
		health = 50;
	}
	else if(enemyType == BOSS)
	{
		row = 5;
		column = 5;
		
		char tempCharArray[5][5] = 
		{
			{'#','X','X','X','#'},
			{'0','X','^','X','0'},
			{' ','<','@','>',' '},
			{' ','0','v','0',' '},
			{' ',' ','|',' ',' '}
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
		
		updateDelayDuration = 10;
		shootDelayDuration = 5;
		
		bulletType = DUAL;
		
		health = 2000;
	}
	
	updateDelayTimer = 0;
	shootDelayTimer = 0;
	this->enemyType = enemyType;
	isAlive = true;
}

void Enemy::Update()
{
	updateDelayTimer++;
	
	if(updateDelayTimer < updateDelayDuration)
		return;
		
	updateDelayTimer = 0;
	
	if(!CheckBottomBoundary())
		MoveDown();
	else
		isAlive = false;
	
	if(shootDelayDuration >= 0 && y + row + 1 >= BOUNDARY_TOP)
	{
		shootDelayTimer++;
		if(shootDelayTimer > shootDelayDuration)
		{
			if(player->x + player->column >= x && player->x <= x + column)
				Shoot();
			shootDelayTimer = 0;
		}
	}
}

void Enemy::Shoot()
{
	int bulletCount = (bulletType == STRAIGHT ? 2 : 1);
	for(int i = 0; i < bulletCount; i++)
	{
		EnemyBullet newBullet(bulletType);
		newBullet.x = x - (newBullet.column / 2) + rand() % column;
		newBullet.y = y - (newBullet.row / 2) + rand() % row;
		bulletList->push_back(newBullet);
	}
}

Boss::Boss() : Enemy(BOSS)
{
	panningLeft = true;
	vulnerable = false;
	ramming = false;
	
	currentAction = BAM;
	
	actionTimer = 0;
	actionDuration = 50;
}

void Boss::Update()
{
	updateDelayTimer++;
	
	if(updateDelayTimer < updateDelayDuration)
		return;
		
	updateDelayTimer = 0;
	
	if(y < BOUNDARY_TOP + 5)
		MoveDown();
	else
	{
		if(CheckLeftBoundary())
			panningLeft = false;
		else if(CheckRightBoundary())
			panningLeft = true;
		
		if(!ramming)
		{
			if(panningLeft)
				MoveLeft();
			else
				MoveRight();
		}
	}
	
	if(isAlive)
		ChooseRandomAction();
}

void Boss::ChooseRandomAction()
{
	if(++actionTimer > actionDuration)
	{
		if(ramming)
		{
			Ram();
			return;
		}
		else
		{
			if(y > BOUNDARY_TOP + 5)
			{
				MoveUp();
				return;
			}
			else
			{
				updateDelayDuration = 10;
				actionDuration = 50;
			}
		}
		currentAction = (BossAction)(currentAction + 1);
		if(currentAction >= TOTAL_BOSS_ACTIONs) currentAction = BAM;
		vulnerable = (currentAction == HAM);
		actionTimer = 0;
	}
	else
	{
		switch(currentAction)
		{
			case RAM:
				Ram();
				break;
			case BAM:
				Bam();
				break;
			case HAM:
			default:
				break;
		}
	}
}

void Boss::Ram()
{
	updateDelayDuration = 2;
	actionDuration = 250;
	if(ramming)
	{
		if(y + row < BOUNDARY_BOTTOM)
		{
			MoveDown();
		}
		else
		{
			ramming = false;
		}
	}
	else
	{
		if(y > BOUNDARY_TOP + 5)
		{
			MoveUp();
		}
		else
		{
			if(player->x + player->column >= x && player->x <= x + column)
				ramming = true;
		}
	}
}

void Boss::Bam()
{
	if(shootDelayTimer < shootDelayDuration)
	{
		if(y >= BOUNDARY_TOP + 5)
		{
			Shoot();
			shootDelayTimer++;
		}
	}
	else
	{
		shootDelayTimer++; 
		if(shootDelayTimer > 2 * shootDelayDuration)
		{
			shootDelayTimer = 0;
		}
	}
}
