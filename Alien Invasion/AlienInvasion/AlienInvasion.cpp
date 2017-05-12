#include "AllHeaderFiles.h" //Headers = declaration , Sources = implementation

void SetQuickEditMode(bool b)
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hIn, &dwMode);
	if(!b)
		dwMode &= ~ENABLE_QUICK_EDIT_MODE;
	else
		dwMode |= ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hIn, dwMode | ENABLE_EXTENDED_FLAGS);
}

int enemySpawnTimer[TOTAL_ENEMY_TYPE] = {0, 0, 0};

void DrawBoundary()
{
    textcolor(WHITE);
    for(int i=BOUNDARY_LEFT; i<=BOUNDARY_RIGHT; i++)
    {
        gotoxy(i,BOUNDARY_TOP);
        cout <<"=";
        gotoxy(i,BOUNDARY_BOTTOM);
        cout <<"=";
    }
    for(int i=BOUNDARY_TOP + 1; i<BOUNDARY_BOTTOM; i++)
    {
        gotoxy(BOUNDARY_LEFT,i);
        cout <<"|";
        gotoxy(BOUNDARY_RIGHT,i);
        cout <<"|";
    }
}

void DrawHUD(Player& player, Boss* boss)
{
    textcolor(LIGHTGRAY);
    gotoxy(BOUNDARY_LEFT, BOUNDARY_BOTTOM + 2);
    cout << "Health                                         ";
    gotoxy(BOUNDARY_LEFT + 7, BOUNDARY_BOTTOM + 2);
    player.health > 100 ? textcolor(LIGHTGREEN) : player.health > 50 ? textcolor(YELLOW) : textcolor(LIGHTRED);
    for(int i = 0; i < player.health / 10; i++)
    	cout << char(219);
    textcolor(WHITE);
    for(int i = 0; i < player.armor / 10; i++)
    	cout << char(178);
    
    gotoxy(BOUNDARY_LEFT, BOUNDARY_BOTTOM + 3);
    if(player.bulletType == STRAIGHT)  cout << "RAILGUN  ";
    else if(player.bulletType == DUAL) cout << "POWERSHOT";
    cout << "            ";
    gotoxy(BOUNDARY_LEFT + 10, BOUNDARY_BOTTOM + 3);
	if(player.health <= 50 && player.armor <= 0)
	{
		textcolor(LIGHTRED);
		cout << "MALFUNCTION";
	}
    else if(player.bulletType == STRAIGHT)
	{
    	textcolor(LIGHTGREEN);
		cout << char(236) << " Charges";
	}
    else if(player.bulletType == DUAL)
    {
    	if(player.penetrateAmmo <= 0)
    	{
    		textcolor(LIGHTRED);
			cout << "X Charges";
		}
		else
		{
	    	textcolor(LIGHTCYAN);
		    for(int i = 0; i < player.penetrateAmmo / 5 + 1; i++)
		    	cout << char(254);
		}
	}
	textcolor(WHITE);
    
    gotoxy(BOUNDARY_LEFT, BOUNDARY_BOTTOM + 4);
    cout << "Score: " << player.score;
    
    if(boss != NULL && boss->isAlive)
    {
	    textcolor(LIGHTGRAY);
	    gotoxy(BOUNDARY_LEFT, BOUNDARY_BOTTOM + 5);
	    cout << "Boss                               ";
	    gotoxy(BOUNDARY_LEFT + 5, BOUNDARY_BOTTOM + 5);
	    boss->health > 1000 ? textcolor(LIGHTCYAN) : boss->health > 500 ? textcolor(CYAN) : textcolor(LIGHTBLUE);
	    for(int i = 0; i < boss->health / 100; i++)
	    	cout << char(219);
	    
	    textcolor(LIGHTGRAY);
	    gotoxy(BOUNDARY_LEFT, BOUNDARY_BOTTOM + 6);
	    cout << "PHASE - " << "   ";
	    gotoxy(BOUNDARY_LEFT + 8, BOUNDARY_BOTTOM + 6);
	    if(boss->currentAction == HAM)  cout << "HAM";
	    else if(boss->currentAction == RAM)  cout << "RAM";
	    else if(boss->currentAction == BAM)  cout << "BAM";
	    
		textcolor(WHITE);
	}
}

bool RectangleIntersect(int obj1_x, int obj1_y, int obj1_height, int obj1_width, int obj2_x, int obj2_y, int obj2_height, int obj2_width)
{
	if
	(
		obj1_x > obj2_x + obj2_width ||
		obj1_x + obj1_width < obj2_x ||
		obj1_y > obj2_y + obj2_height ||
		obj1_y + obj1_height < obj2_y
	)
	{
		return false; //No collision
	}
	return true; //Collision occured
}

void EnemyOverlappingCheck(Enemy& enemy, vector<Enemy>& enemyList)
{
	bool isOverlapping = false;
	do
	{
		isOverlapping = false;
		enemy.x = rand() % (BOUNDARY_RIGHT - BOUNDARY_LEFT - enemy.column) + BOUNDARY_LEFT + 1;
		enemy.y = -1 * rand() % 10;
		
		for(int i = 0; i < enemyList.size(); i++)
		{
			//if(&enemy != &enemyList[i])
			if
			(
				RectangleIntersect
				(
					enemy.x, enemy.y, enemy.row, enemy.column,
					enemyList[i].x, enemyList[i].y, enemyList[i].row, enemyList[i].column
				)
			)
			{
				isOverlapping = true;
				break;
			}
		}
	}
	while(isOverlapping);
}

void SpawnPowerup(Player& player, vector<Powerup>& powerupList, int& powerScore)
{
	if(player.score - powerScore > 500)
	{
		powerScore += 1000;
		POWERUP_TYPE random = (POWERUP_TYPE)(rand() % TOTAL_POWERUP_TYPE);
		if(player.health <= 75)
		{
			if(player.armor <= 50 && rand() % 2 == 0)
				random = ARMOR;
			else
				random = REGEN;
		}
		Powerup newPowerup((POWERUP_TYPE)random);
		newPowerup.x = player.x + (player.column / 2) - (newPowerup.column / 2);
		newPowerup.y = BOUNDARY_TOP - 5;
		powerupList.push_back(newPowerup);
	}
}

void SpawnEnemy(Player& player, vector<Enemy>& enemyList, vector<EnemyBullet>* enemyBulletList)
{
	for(int i = 0; i < TOTAL_ENEMY_TYPE; i++)
	{
		if(++enemySpawnTimer[i] > ENEMY_SPAWN_DURATION * pow(2, i))
		{
			switch(i)
			{
				case 0:
					for(int i = -2; i <= 2; i++)
					{
						Enemy newEnemy(CRASHER);
						newEnemy.bulletList = enemyBulletList;
						newEnemy.player = &player;
						newEnemy.x = (player.x + (player.column / 2) - (newEnemy.column / 2)) + 5 * (i);
						newEnemy.y = (BOUNDARY_TOP - 5) - abs(i);
						enemyList.push_back(newEnemy);
					}
					break;
				case 1:
					{
						Enemy newEnemy(SHOOTER);
						newEnemy.bulletList = enemyBulletList;
						newEnemy.player = &player;
						newEnemy.x = player.x + (player.column / 2) - (newEnemy.column / 2);
						newEnemy.y = BOUNDARY_TOP - newEnemy.row;
						enemyList.push_back(newEnemy);
					}
					break;
				default:
					break;
			}
			enemySpawnTimer[i] = 0;
		}
	}
}

void MoveBullets(vector<EnemyBullet>& enemyBulletList)
{
	for(int i = 0; i < enemyBulletList.size(); i++)
	{
		enemyBulletList[i].Update();
		
		if(!enemyBulletList[i].isAlive)
		{
			enemyBulletList[i].DrawTrail();
			enemyBulletList.erase(enemyBulletList.begin() + i);
		}
		else
			enemyBulletList[i].Draw();
	}
}

void CollisionCheck(Player& player, vector<Enemy>& enemyList, vector<EnemyBullet>& enemyBulletList, vector<Powerup>& powerupList, Boss* boss)
{
	//! Enemies vs Player stuff
	for(int i = 0; i < enemyList.size(); i++)
	{
		if
		(
			RectangleIntersect
			(
				player.x, player.y, player.row, player.column,
				enemyList[i].x, enemyList[i].y, enemyList[i].row, enemyList[i].column
			)
		)
		{
			player.armor -= enemyList[i].health;
			if(player.armor < 0)
			{
				player.health += player.armor;
				player.armor = 0;
			}
			player.score += 10 * pow(5, enemyList[i].enemyType + 1);
			enemyList[i].isAlive = false;
		}
		
		for(int j = 0; j < player.bulletList.size(); j++)
		{
			if
			(
				RectangleIntersect
				(
					player.bulletList[j].x, player.bulletList[j].y, player.bulletList[j].row, player.bulletList[j].column,
					enemyList[i].x, enemyList[i].y, enemyList[i].row, enemyList[i].column
				)
			)
			{
				if(player.bulletList[j].type != DUAL)
					player.bulletList[j].isAlive = false;
				enemyList[i].health -= player.bulletList[j].damage;
				if(enemyList[i].health <= 0)
				{
					enemyList[i].isAlive = false;
					player.score += 10 * pow(5, enemyList[i].enemyType + 1);
				}
			}
		}
	}
	//! EnemyBullet vs Player
	for(int i = 0; i < enemyBulletList.size(); i++)
	{
		if
		(
			RectangleIntersect
			(
				player.x, player.y, player.row, player.column,
				enemyBulletList[i].x, enemyBulletList[i].y, enemyBulletList[i].row, enemyBulletList[i].column
			)
		)
		{
			if(enemyBulletList[i].type != DUAL)
				enemyBulletList[i].isAlive = false;
			player.armor -= enemyList[i].health;
			if(player.armor < 0)
			{
				player.health += player.armor;
				player.armor = 0;
			}
		}
	}
	//! Powerup vs Player
	for(int i = 0; i < powerupList.size(); i++)
	{
		if
		(
			RectangleIntersect
			(
				player.x, player.y, player.row, player.column,
				powerupList[i].x, powerupList[i].y, powerupList[i].row, powerupList[i].column
			)
		)
		{
			switch(powerupList[i].type)
			{
				case REGEN:
					player.regenTime = 20;
					break;
				case ARMOR:
					player.armor = 100;
					break;
				case PENETRATE:
					player.penetrateAmmo += 25;
					break;
			}
			powerupList[i].isAlive = false;
		}
	}
	if(boss != NULL && boss->isAlive)
	{
		//! Boss vs PlayerBullet
		for(int i = 0; i < player.bulletList.size(); i++)
		{
			if
			(
				RectangleIntersect
				(
					player.bulletList[i].x, player.bulletList[i].y, player.bulletList[i].row, player.bulletList[i].column,
					boss->x, boss->y, boss->row, boss->column
				)
			)
			{
				if(player.bulletList[i].type != DUAL)
					player.bulletList[i].isAlive = false;
				boss->health -= player.bulletList[i].damage;
				if(boss->health <= 0)
				{
					boss->isAlive = false;
					player.score += 10 * pow(5, boss->enemyType + 1);
				}
			}
		}
		
		//Boss vs Player
		if
		(
			RectangleIntersect
			(
				player.x, player.y, player.row, player.column,
				boss->x, boss->y, boss->row, boss->column
			)
		)
		{
			player.armor--;
			if(player.armor < 0)
			{
				player.health += player.armor;
				player.armor = 0;
			}
			boss->health--;
			if(boss->health <= 0)
			{
				boss->isAlive = false;
				player.score += 10 * pow(5, boss->enemyType + 1);
			}
		}
	}
}

int main()
{
    //! it all begins here!!!    
    
    //! console window size    
    system("mode con: cols=60 lines=40");
    SetQuickEditMode(false);
    
    //! seed the random function with current system time
    srand( time(NULL) );
    
    DrawBoundary();
    
    hideCursor();

    char button;
    
    Player player;
    vector<Enemy> enemyList;
    Boss* boss = NULL;
    vector<EnemyBullet> enemyBulletList;
	int powerScore = 0;
    vector<Powerup> powerupList;
    
    do
	{
		MoveBullets(enemyBulletList);
		
		if(boss == NULL)
		{
			if(player.score >= BOSS_SPAWN_SCORE)
			{
				boss = new Boss();
				boss->bulletList = &enemyBulletList;
				boss->player = &player;
				boss->x = player.x + (player.column / 2) - (boss->column / 2);
				boss->y = BOUNDARY_TOP - boss->row;
			}
			else
				SpawnEnemy(player, enemyList, &enemyBulletList);
		}
		else
		{
			boss->Update();
			if(!boss->isAlive)
			{
				boss->DrawTrail();
				boss = NULL;
			}
			else
			{
				boss->Draw();
				
				if(boss->vulnerable)
					SpawnEnemy(player, enemyList, &enemyBulletList);
			}
		}
		
		for(int i = 0; i < enemyList.size(); i++)
		{
			enemyList[i].Update();
			if(!enemyList[i].isAlive)
			{
				enemyList[i].DrawTrail();
				enemyList.erase(enemyList.begin() + i);
			}
			else
			{
				enemyList[i].Draw();
			}
		}
		SpawnPowerup(player, powerupList, powerScore);
		for(int i = 0; i < powerupList.size(); i++)
		{
			powerupList[i].Update();
			if(!powerupList[i].isAlive)
			{
				powerupList[i].DrawTrail();
				powerupList.erase(powerupList.begin() + i);
			}
			else
			{
				powerupList[i].Draw();
			}
		}
		
		player.Update();
		player.Draw();
		
		CollisionCheck(player, enemyList, enemyBulletList, powerupList, boss);
		DrawHUD(player, boss);
        
        if(boss != NULL && !boss->isAlive)
        {
		    gotoxy(18, 20);
		    cout << "BOSS IS DEAD! YOU WIN!";
		    break;
		}
		else if(player.health > 0)
		{
		    for(int i = 0; i < player.row; i++)
		    for(int j = 0; j < player.column; j++)
		    {
		    	gotoxy(player.x + j, player.y + i);
		    	cout << char(167);
			}
		    gotoxy(18, 20);
		    cout << "YOU EXPLODED! GAME OVER!";
		    break;
		}
		
        Sleep(20);
    }
	while(true);
    
    Sleep(1000);
    gotoxy(16, 21);
    system("PAUSE");
    return 0;
}
