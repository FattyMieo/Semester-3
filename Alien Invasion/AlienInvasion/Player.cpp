#include "AllHeaderFiles.h"

Player::Player() : GameObject() //Calling the base class
{
	row = 3;
	column = 5;
	
	char tempCharArray[3][5] = 
	{
		{' ','|','!','|',' '},
		{'\\','#','#','#','/'},
		{' ','|','#','|',' '}
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
	y = 20;
	
	health = 200;
	armor = 100;
	score = 0;
	shootDelayTimer = 0;
	shootDelayDuration = 2;
	
	regenTime = 0;
	regenDelayTimer = 0;
	regenDelayDuration = 5;
	
	penetrateAmmo = 0;
	
	bulletType = STRAIGHT;
}
             
void Player::Update()
{
	shootDelayTimer++;
	
	if(GetAsyncKeyState('W'))
	{
		if(health <= 100 && armor <= 0 && rand() % 5 == 0)
		{
			if(!CheckBottomBoundary()) MoveDown();
		}
		else
		{
			if(!CheckTopBoundary()) MoveUp();
		}
	}
	if(GetAsyncKeyState('S'))
	{
		if(health <= 100 && armor <= 0 && rand() % 5 == 0)
		{
			if(!CheckTopBoundary()) MoveUp();
		}
		else
		{
			if(!CheckBottomBoundary()) MoveDown();
		}
	}
	if(GetAsyncKeyState('A'))
	{
		if(health <= 100 && armor <= 0 && rand() % 5 == 0)
		{
			if(!CheckRightBoundary()) MoveRight();
		}
		else
		{
			if(!CheckLeftBoundary()) MoveLeft();
		}
	}
	if(GetAsyncKeyState('D'))
	{
		if(health <= 100 && armor <= 0 && rand() % 5 == 0)
		{
			if(!CheckLeftBoundary()) MoveLeft();
		}
		else
		{
			if(!CheckRightBoundary()) MoveRight();
		}
	}
	if(GetAsyncKeyState(VK_SHIFT))
	{
		if(shootDelayTimer > 5)
		{
			SwitchWeapon();
			shootDelayTimer = 0;
		}
	}
	if(GetAsyncKeyState(VK_SPACE))
	{
		if(shootDelayTimer > shootDelayDuration)
		{
			Shoot();
			shootDelayTimer = 0;
		}
	}
	
	for(int i = 0; i < bulletList.size(); i++)
	{
		bulletList[i].Update();
		
		if(!bulletList[i].isAlive)
		{
			bulletList[i].DrawTrail();
			bulletList.erase(bulletList.begin() + i);
		}
		else
			bulletList[i].Draw();
	}
	
	if(health < 0) health = 0;
	
	if(regenTime > 0)
	{
		regenDelayTimer++;
		if(regenDelayTimer > regenDelayDuration)
		{
			regenTime--;
			
			if(health < 200)
			{
				health += 5;
			}
			else
			{
				health = 200;
			}
			
			regenDelayTimer = 0;
		}
	}
}

void Player::SwitchWeapon()
{
	bulletType = (BULLET_TYPE)(bulletType + 1);
	if(bulletType >= TOTAL_BULLET_TYPE)
		bulletType = STRAIGHT;
		
	switch(bulletType)
	{
		case STRAIGHT:
			shootDelayDuration = 1;
			break;
		case DUAL:
			shootDelayDuration = 10;
			break;
	}
}

void Player::Shoot()
{
	if(health <= 50 && armor <= 0)
		return;
	if(bulletList.size() < MAX_BULLET)
	{
		if(bulletType == STRAIGHT) //Machine Gun mode :D
		{
			for(int i = 0; i < column / 2; i++)
			{
				Bullet newBullet(bulletType);
				newBullet.x = x - (newBullet.column / 2) + rand() % column;
				newBullet.y = y - (newBullet.row / 2) + (row / 2);
				bulletList.push_back(newBullet);
			}
		}
		else
		{
			if(bulletType == DUAL && penetrateAmmo > 0)
				penetrateAmmo--;
			else
				return;
			
			Bullet newBullet(bulletType);
			newBullet.x = x - (newBullet.column / 2) + (column / 2);
			newBullet.y = y - (newBullet.row / 2) + (row / 2);
			bulletList.push_back(newBullet);
		}
	}
}
