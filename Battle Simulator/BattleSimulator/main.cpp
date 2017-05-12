#include "main.h"

enum HeroType
{
	T_FOOTMAN = 0,
	T_ARCHER,
	T_KNIGHT,
	T_ROGUE,				
	TOTAL_HERO_TYPE,
	T_NONE
};

enum Direction
{
	D_UP = 0,
	D_DOWN,
	D_LEFT,
	D_RIGHT,
	TOTAL_DIRECTION,
	D_NONE
};

int windowWidth = 120;
int windowHeight = 30;
/*
void SelectWindowSize()
{
	cout << "Select Resolution:" << endl;
	cout << "1. 240 x 120" << endl;
	cout << "2. 120 x 60" << endl;
	cout << "3. 60 x 30" << endl;
	cout << "4. 40 x 20" << endl;
	cout << "5. 20 x 10" << endl;
	cout << endl;
	cout << "0. Custom Size" << endl;
	
	switch(Utilities::cin_int(0, 5))
	{
		case 0:
			cout << "Select Window Size (Width):" << endl;
			windowWidth = Utilities::cin_int(1, 1000);
			cout << "Select Window Size (Height):" << endl;
			windowHeight = Utilities::cin_int(1, 500);
			break;
		case 1:
			windowWidth = 240;
			windowHeight = 120;
			break;
		case 2:
			windowWidth = 120;
			windowHeight = 60;
			break;
		case 3:
			windowWidth = 60;
			windowHeight = 30;
			break;
		case 4:
			windowWidth = 40;
			windowHeight = 20;
			break;
		case 5:
			windowWidth = 20;
			windowHeight = 10;
			break;
	}
	
	SetWindowSize(windowWidth, windowHeight);
}
*/
template<class SpriteClass>
class Actor
{
protected:
	int old_x, old_y;
	SpriteClass actorSprite;
	
public:
	int width, height;
	int x, y;
	
	Actor(int x, int y, int width, int height)
	{
		SetPos(x, y);
		SetDimension(width, height);
	}
	
	void Move(int x, int y)
	{
		old_x = this->x;
		old_y = this->y;
		if(this->x + x >= 0 && this->x + x < windowWidth) this->x += x;
		if(this->y + y >= 0 && this->y + y < windowHeight) this->y += y;
	}
	
	void SetPos(int x, int y)
	{
		old_x = this->x;
		old_y = this->y;
		this->x = x;
		this->y = y;
	}
	
	void SetDimension(int width, int height)
	{
		this->width = width;
		this->height = height;
		actorSprite.SetDimension(width, height);
	}
	
	void ImportSprite(const int* colorArray, const string* symbolArray);
	
	virtual void Draw()
	{
		RemoveTrail();
		actorSprite.Draw(x, y);
	}
	
	void RemoveTrail()
	{
		for(int i = 0; i < height / 2; i++)
		for(int j = 0; j < width; j++)
		{
			GoToXY(old_x + j, old_y + i);
			cout << " ";
		}
	}
	
	void DrawStandardOutline()
	{
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				GoToXY(x + j, y + i);
				if(i <= 0)
				{
					if(j <= 0)
					{
						cout << char(218);
					}
					else if(j >= width - 1)
					{
						cout << char(191);
					}
					else
					{
						cout << char(196);
					}
				}
				else if(i >= height - 1)
				{
					if(j <= 0)
					{
						cout << char(192);
					}
					else if(j >= width - 1)
					{
						cout << char(217);
					}
					else
					{
						cout << char(196);
					}
				}
				else
				{
					if(j <= 0)
					{
						cout << char(179);
					}
					else if(j >= width - 1)
					{
						cout << char(179);
					}
					else
					{
						cout << " ";
					}
				}
			}
		}
	}
};

template<> inline void Actor<Sprite>::ImportSprite(const int* colorArray, const string* symbolArray)
{
	actorSprite.Import(colorArray);
}
	
template<> inline void Actor<ASCIISprite>::ImportSprite(const int* colorArray, const string* symbolArray)
{
	actorSprite.Import(symbolArray, colorArray);
}

template<class SpriteA, class SpriteB>
bool IsColliding(Actor<SpriteA>* a, Actor<SpriteB>* b)
{
	if
	(
		a.x > b.x + b.width ||
		b.x > a.x + a.width ||
		a.y > b.y + b.height ||
		b.y > a.y + a.height
	)
		return false; // No collision
	return true; // Collision occured
}

class Button : public Actor<ASCIISprite>
{
public:
	string text;
	
	Button(int x, int y, int width, int height) : Actor<ASCIISprite>(x, y, width, height)
	{
		text = "";
	}
	
	void Draw()
	{
		RemoveTrail();
		
		DrawStandardOutline();
		if(!actorSprite.IsNull())
		{
			actorSprite.Draw(x, y);
		}
		else
		{
			GoToXY(x + width / 2 - text.size() / 2, y + height / 2);
			cout << text;
		}
		
		SetForeColor(GRAY);
	}
};

class HUD : public Actor<ASCIISprite>
{
protected:
	vector<Button> buttonContainer;
public:
	HUD(int x, int y, int width, int height) : Actor<ASCIISprite>(x, y, width, height)
	{
		
	}
	
	void AddSimpleButton(int relativeX, int relativeY, string text)
	{
		Button newButton(x + relativeX, y + relativeY, text.size() + 4, 3);
		newButton.text = text;
		
		buttonContainer.push_back(newButton);
	}
	
	void Draw()
	{
		if(buttonContainer.empty())
		{
			Actor<ASCIISprite>::Draw();
		}
		else
		{
			RemoveTrail();
			
			if(actorSprite.IsNull())
				DrawStandardOutline();
			else
				Actor<ASCIISprite>::Draw();
			
			for(int i = 0; i < buttonContainer.size(); i++)
			{
				buttonContainer[i].Draw();
			}
			
			SetForeColor(GRAY);
		}
	}
};

class Hero : public Actor<Sprite>
{
protected:
	static const int defaultHealth = 100;
	static const int defaultResource = 100;
	static const int defaultDamage = 10;
public:
	int tileX;
	int health;
	int maxHealth;
	int resource;
	int maxResource;
	int damage;
	int moveRange;
	int attackRange;
	int specialRange;
	bool weakened;
	int moveCost;
	int attackCost;
	int specialCost;
	
	vector<string> actionNameList;
	
	HeroType type;
	
	Hero(int x, int y, int width, int height) : Actor<Sprite>(x, y, width, height)
	{
		tileX = 0;
		health = maxHealth = defaultHealth;
		resource = maxResource = defaultResource;
		damage = defaultDamage;
		moveRange = 2;
		attackRange = 1;
		specialRange = 1;
		weakened = false;
		moveCost = 0;
		attackCost = 0;
		specialCost = 0;
		type = T_NONE;
	}
	
	static string GetHeroTypeName(HeroType type)
	{
		switch(type)
		{
			case T_FOOTMAN:
				return "Footman";
			case T_ARCHER:
				return "Archer";
			case T_ROGUE:
				return "Rogue";
			case T_KNIGHT:
				return "Knight";
			case T_NONE:
				return "None";
		}
	}
	
	void InitializeStats(int health, int resource)
	{
		this->health = health;
		this->resource = resource;
		this->damage = damage;
	}
	
	void DrawStats()
	{
		GoToXY(x + width + 4, y + 2);
		SetForeColor(GREEN);
		for(int i = 0; i < health / 5; i++)
			cout << char(219);
		for(int i = health / 5; i < maxHealth / 5; i++)
			cout << char(176);
		GoToXY(x + width + 4, y + 3);
		SetForeColor(CYAN);
		for(int i = 0; i < resource / 5; i++)
			cout << char(219);
		for(int i = resource / 5; i < maxResource / 5; i++)
			cout << char(176);
		SetForeColor(GRAY);
		GoToXY(x + width + 4, y + 6);
		cout << damage << " ATK";
		if(weakened)
		{
			GoToXY(x + width + 14, y + 6);
			cout << "WEAKENED";
		}
	}
	
	virtual void Attack(Hero* enemy) = 0;
	
	virtual void SpecialAttack(Hero* enemy) = 0;
};

class Footman : public Hero
{
public:
	Footman(int x, int y, int width, int height) : Hero(x, y, width, height)
	{
		health = maxHealth = 120;
		damage = 12;
		type = T_FOOTMAN;
		moveRange = 3;
		attackRange = 1;
		specialRange = 3;
		actionNameList.push_back("Dash");
		actionNameList.push_back("Swing");
		actionNameList.push_back("Shockwave");
		moveCost = 10;
		attackCost = 30;
		specialCost = 60;
	}
	
	void Attack(Hero* enemy)
	{
		enemy->health -= damage;
	}
	
	void SpecialAttack(Hero* enemy)
	{
		enemy->health -= damage / Utilities::Rand_Int(1, 3);
	}
};

class Archer : public Hero
{
public:
	Archer(int x, int y, int width, int height) : Hero(x, y, width, height)
	{
		health = maxHealth = 80;
		resource = maxResource = 80;
		damage = 15;
		type = T_ARCHER;
		moveRange = 2;
		attackRange = 5;
		specialRange = 3;
		actionNameList.push_back("Position");
		actionNameList.push_back("Pierce");
		actionNameList.push_back("Volley");
		moveCost = 15;
		attackCost = 30;
		specialCost = 45;
	}
	
	void Attack(Hero* enemy)
	{
		enemy->health -= damage * (Utilities::Rand_Int(5, 10) / 10);
	}
	
	void SpecialAttack(Hero* enemy)
	{
		enemy->health -= damage / 2;
	}
};

class Rogue : public Hero
{
public:
	Rogue(int x, int y, int width, int height) : Hero(x, y, width, height)
	{
		health = maxHealth = 100;
		resource = maxResource = 120;
		damage = 15;
		type = T_ROGUE;
		moveRange = 4;
		attackRange = 3;
		specialRange = 1;
		actionNameList.push_back("Teleport");
		actionNameList.push_back("Shuriken");
		actionNameList.push_back("Stab");
		moveCost = 10;
		attackCost = 25;
		specialCost = 40;
	}
	
	void Attack(Hero* enemy)
	{
		enemy->health -= damage;
		enemy->weakened = true;
	}
	
	void SpecialAttack(Hero* enemy)
	{
		enemy->health -= (enemy->weakened ? damage * 2 : 0);
		enemy->weakened = false;
	}
};

class Knight : public Hero
{
public:
	Knight(int x, int y, int width, int height) : Hero(x, y, width, height)
	{
		health = maxHealth = 150;
		damage = 10;
		type = T_KNIGHT;
		attackRange = 1;
		specialRange = 2;
		actionNameList.push_back("Charge");
		actionNameList.push_back("Lance");
		actionNameList.push_back("Heal");
		moveCost = 25;
		attackCost = 40;
		specialCost = 75;
	}
	
	void Attack(Hero* enemy)
	{
		enemy->health -= damage + (enemy->weakened ? damage : 0);
		enemy->weakened = false;
	}
	
	void SpecialAttack(Hero* enemy)
	{
		enemy->health += specialCost / Utilities::Rand_Int(2, 4);
		if(enemy->health > 100) enemy->health = 100;
		enemy->weakened = false;
	}
};

enum GameState
{
	GS_SELECT_HERO = 0,
	GS_SELECT_ACTION,
	GS_MOVE_HERO,
	GS_HERO_ATK,
	GS_HERO_SPECIAL,
	TOTAL_IS
};

class Indicator
{
public:
	int position;
	int length;
	COLOR color;
	
	Indicator()
	{
		position = 0;
		length = 1;
		color = WHITE;
	}
	
	void Move(Direction d)
	{
		switch(d)
		{
			case D_LEFT:
				position--;
				break;
			case D_RIGHT:
				position++;
				break;
			default:
				break;
		}
	}
	
	void Check(int lineLength)
	{
		if(position < 0) position = 0;
		if(position + length >= lineLength) position = lineLength - length;
	}
};

static class System
{
public:
	HUD* boundary;
	const int lineLength = 30;
	Hero** heroLine;
	Indicator indicator;
	Indicator selector;
	vector<Hero*> playerGroup;
	vector<Hero*> enemyGroup;
	GameState state;
	
	Hero* selectedHero;
	
	System()
	{
		heroLine = new Hero*[lineLength];
		state = GS_SELECT_HERO;
	}
	
	template<class SpriteClass>
	Direction TouchingBoundary(Actor<SpriteClass>* a)
	{
		if(boundary->x > a.x)
		{
			return D_LEFT;
		}
		else if(boundary->x + boundary->width < a->x + a->width)
		{
			return D_RIGHT;
		}
		else if(boundary->y > a->y)
		{
			return D_UP;
		}
		else if(boundary->y + boundary->height < a->y + a->height)
		{
			return D_DOWN;
		}
		else
		{
			return D_NONE;
		}
	}
	
	bool IsEmpty(int x)
	{
		for(int i = 0; i < playerGroup.size(); i++)
		{
			if(x == playerGroup[i]->tileX)
				return false;
		}
		for(int i = 0; i < enemyGroup.size(); i++)
		{
			if(x == enemyGroup[i]->tileX)
				return false;
		}
		return true;
	}
	
	bool HasHeroGroup(int x, vector<Hero*>& heroGroup)
	{
		for(int i = 0; i < heroGroup.size(); i++)
		{
			if(x == heroGroup[i]->tileX)
				return true;
		}
		return false;
	}
	
	bool WithinRange(int x, int center, int range)
	{
		return x >= center - range && x <= center + range;
	}
	
	int lineX = (windowWidth / 2) - ((lineLength * 3) / 2);
	int lineY = windowHeight / 2;
	
	void DrawAll()
	{
		switch(state)
		{
			case GS_SELECT_HERO:
				indicator.length = 1;
				indicator.color = WHITE;
				selector.length = 0;
				break;
			case GS_MOVE_HERO:
				indicator.length = 1;
				if(IsEmpty(indicator.position) && WithinRange(indicator.position, selectedHero->tileX, selectedHero->moveRange))
					indicator.color = GREEN;
				else
					indicator.color = RED;
				selector.length = 0;
				selector.color = YELLOW;
				break;
			case GS_SELECT_ACTION:
				indicator.length = 1;
				indicator.color = YELLOW;
				selector.length = 1;
				selector.color = WHITE;
				break;
		}
		
		GoToXY(lineX, lineY);
		for(int i = 0; i < lineLength; i++)
		{
			SetBackColor(BLACK);
			
			if(i >= indicator.position && i < indicator.position + indicator.length)
				SetBackColor(indicator.color);
			else if(selectedHero != NULL)
			{
				if(state == GS_MOVE_HERO)
				{
					if(i >= selectedHero->tileX - selectedHero->moveRange && i <= selectedHero->tileX + selectedHero->moveRange)
						SetBackColor(YELLOW);
				}
				else if(state == GS_HERO_ATK)
				{
					if(i >= selectedHero->tileX - selectedHero->attackRange && i <= selectedHero->tileX + selectedHero->attackRange)
						SetBackColor(CYAN);
				}
				else if(state == GS_HERO_SPECIAL)
				{
					if(i >= selectedHero->tileX - selectedHero->specialRange && i <= selectedHero->tileX + selectedHero->specialRange)
						SetBackColor(DARKCYAN);
				}
			}
			
			cout << "___";
		}
		
		if(selectedHero != NULL)
		{
			selectedHero->Draw();
			selectedHero->DrawStats();
			for(int i = 0; i < selectedHero->actionNameList.size(); i++)
			{
				GoToXY(selectedHero->x - 20, selectedHero->y + 2 + i);
				
				if(selector.position == i)
					SetBackColor(selector.color);
				
				cout << selectedHero->actionNameList[i];
				
				SetBackColor(BLACK);
			}
		}
		else
		{
			bool foundIndicator = false;
			for(int i = 0; i < playerGroup.size(); i++)
			{
				if(indicator.position == playerGroup[i]->tileX)
				{
					playerGroup[i]->Draw();
					playerGroup[i]->DrawStats();
					foundIndicator = true;
				}
				if(foundIndicator) break;
			}
			for(int i = 0; i < enemyGroup.size(); i++)
			{
				if(indicator.position == enemyGroup[i]->tileX)
				{
					enemyGroup[i]->Draw();
					enemyGroup[i]->DrawStats();
					foundIndicator = true;
				}
				if(foundIndicator) break;
			}
		}
		
		for(int i = 0; i < playerGroup.size(); i++)
		{
			SetForeColor(CYAN);
			SetBackColor(BLACK);
			GoToXY(lineX + playerGroup[i]->tileX * 3, lineY);
			if(playerGroup[i]->tileX >= indicator.position && playerGroup[i]->tileX < indicator.position + indicator.length)
			{
				SetForeColor(BLUE);
				SetBackColor(indicator.color);
			}
			else if(selectedHero != NULL)
			{
				if(state == GS_MOVE_HERO)
				{
					if(playerGroup[i]->tileX >= selectedHero->tileX - selectedHero->moveRange && playerGroup[i]->tileX <= selectedHero->tileX + selectedHero->moveRange)
					{
						SetForeColor(BLUE);
						SetBackColor(YELLOW);
					}
				}
				else if(state == GS_HERO_ATK)
				{
					if(playerGroup[i]->tileX >= selectedHero->tileX - selectedHero->attackRange && playerGroup[i]->tileX <= selectedHero->tileX + selectedHero->attackRange)
					{
						SetForeColor(BLUE);
						SetBackColor(CYAN);
					}
				}
				else if(state == GS_HERO_SPECIAL)
				{
					if(playerGroup[i]->tileX >= selectedHero->tileX - selectedHero->specialRange && playerGroup[i]->tileX <= selectedHero->tileX + selectedHero->specialRange)
					{
						SetForeColor(BLUE);
						SetBackColor(DARKCYAN);
					}
				}
			}
			switch(playerGroup[i]->type)
			{
				case T_FOOTMAN:
					cout << "!" << char(254) << "0";
					break;
				case T_ARCHER:
					cout << ":" << char(254) << "}";
					break;
				case T_KNIGHT:
					cout << char(214) << char(223) << char(157);
					break;
				case T_ROGUE:
					cout << char(237) << char(254) << char(169);
					break;
				default:
					cout << "_";
					break;
			}
		}
		
		for(int i = 0; i < enemyGroup.size(); i++)
		{
			SetForeColor(RED);
			SetBackColor(BLACK);
			GoToXY(lineX + enemyGroup[i]->tileX * 3, lineY);
			if(enemyGroup[i]->tileX >= indicator.position && enemyGroup[i]->tileX < indicator.position + indicator.length)
			{
				SetForeColor(DARKRED);
				SetBackColor(indicator.color);
			}
			else if(selectedHero != NULL)
			{
				if(state == GS_MOVE_HERO)
				{
					if(enemyGroup[i]->tileX >= selectedHero->tileX - selectedHero->moveRange && enemyGroup[i]->tileX <= selectedHero->tileX + selectedHero->moveRange)
					{
						SetForeColor(DARKRED);
						SetBackColor(YELLOW);
					}
				}
				else if(state == GS_HERO_ATK)
				{
					if(enemyGroup[i]->tileX >= selectedHero->tileX - selectedHero->attackRange && enemyGroup[i]->tileX <= selectedHero->tileX + selectedHero->attackRange)
					{
						SetForeColor(DARKRED);
						SetBackColor(CYAN);
					}
				}
				else if(state == GS_HERO_SPECIAL)
				{
					if(enemyGroup[i]->tileX >= selectedHero->tileX - selectedHero->specialRange && enemyGroup[i]->tileX <= selectedHero->tileX + selectedHero->specialRange)
					{
						SetForeColor(DARKRED);
						SetBackColor(DARKCYAN);
					}
				}
			}
			switch(enemyGroup[i]->type)
			{
				case T_FOOTMAN:
					cout << "0" << char(254) << "!";
					break;
				case T_ARCHER:
					cout << "{" << char(254) << ":";
					break;
				case T_KNIGHT:
					cout << char(157) << char(223) << char(183);
					break;
				case T_ROGUE:
					cout << char(170) << char(254) << char(237);
					break;
				default:
					cout << "_";
					break;
			}
		}
		
		SetForeColor(GRAY);
		SetBackColor(BLACK);
		/*
		for(int i = 0; i < playerGroup.size(); i++)
		{
			playerGroup[i]->Draw();
			playerGroup[i]->DrawStats();
		}
		for(int i = 0; i < enemyGroup.size(); i++)
		{
			enemyGroup[i]->Draw();
			enemyGroup[i]->DrawStats();
		}
		*/
	}
	
	void Update()
	{
		for(int i = 0; i < playerGroup.size(); i++)
			if(playerGroup[i]->resource < 100) playerGroup[i]->resource += 1;
		for(int i = 0; i < enemyGroup.size(); i++)
			if(enemyGroup[i]->resource < 100) enemyGroup[i]->resource += 1;
		if(state == GS_SELECT_HERO)
			selectedHero = NULL;
			
		indicator.Check(lineLength);
		
		if(selectedHero != NULL)
		{
			selector.Check(selectedHero->actionNameList.size());
		
			if(state == GS_MOVE_HERO)
			{
				if(selectedHero->type == T_KNIGHT)
				{
					Hero* closestEnemy = NULL;
					int range = lineLength;
					
					for(int i = 0; i < enemyGroup.size(); i++)
					{
						if(abs(selectedHero->tileX - enemyGroup[i]->tileX) < range)
						{
							range = selectedHero->tileX - enemyGroup[i]->tileX;
							closestEnemy = enemyGroup[i];
						}
					}
					
					if(closestEnemy == NULL || range == 0)
						return;
					else if(range < 0)
					{
						selectedHero->tileX = closestEnemy->tileX - 1;
						closestEnemy->weakened = true;
					}
					else if(range > 0)
					{
						selectedHero->tileX = closestEnemy->tileX + 1;
						closestEnemy->weakened = true;
					}
					
					state = GS_SELECT_HERO;
				}
			}
			else if(state == GS_HERO_SPECIAL)
			{
				if(selectedHero->type == T_ARCHER)
				{
					indicator.length = 3;
				}
				else if(selectedHero->type == T_KNIGHT)
				{
					for(int i = 0; i < playerGroup.size(); i++)
					{
						if(abs(selectedHero->tileX - playerGroup[i]->tileX) <= 3)
							selectedHero->SpecialAttack(playerGroup[i]);
					}
					state = GS_SELECT_HERO;
				}
			}
		}
		
		for(int i = 0; i < playerGroup.size(); i++)
		{
			if(playerGroup[i]->health <= 0)
			{
				playerGroup.erase(playerGroup.begin() + i);
			}
		}
		
		for(int i = 0; i < enemyGroup.size(); i++)
		{
			if(enemyGroup[i]->health <= 0)
			{
				enemyGroup.erase(enemyGroup.begin() + i);
			}
		}
	}
	
	void Confirm()
	{
		switch(state)
		{
			case GS_SELECT_HERO:
				for(int i = 0; i < playerGroup.size(); i++)
				{
					if(playerGroup[i]->tileX == indicator.position)
					{
						selectedHero = playerGroup[i];
						state = GS_SELECT_ACTION;
						return;
					}
				}
				break;
			case GS_SELECT_ACTION:
				if(selectedHero == NULL)
				{
					state = GS_SELECT_HERO;
					return;
				}
				switch(selector.position)
				{
					case 0:
						if(selectedHero->resource - selectedHero->moveCost >= 0)
						{
							state = GS_MOVE_HERO;
							selectedHero->resource -= selectedHero->moveCost;
						}
						break;
					case 1:
						if(selectedHero->resource - selectedHero->attackCost >= 0)
						{
							state = GS_HERO_ATK;
							selectedHero->resource -= selectedHero->attackCost;
						}
						break;
					case 2:
						if(selectedHero->resource - selectedHero->specialCost >= 0)
						{
							state = GS_HERO_SPECIAL;
							selectedHero->resource -= selectedHero->specialCost;
						}
						break;
				}
				break;
			case GS_MOVE_HERO:
				if(selectedHero == NULL)
				{
					state = GS_SELECT_HERO;
					return;
				}
				
				if(!IsEmpty(indicator.position))
					return;
				if(!WithinRange(indicator.position, selectedHero->tileX, selectedHero->moveRange))
					return;
				selectedHero->tileX = indicator.position;
				
				state = GS_SELECT_HERO;
				break;
			case GS_HERO_ATK:
				if(selectedHero == NULL)
				{
					state = GS_SELECT_HERO;
					return;
				}
				
				if(indicator.position == selectedHero->tileX)
					return;
				
				if(!WithinRange(indicator.position, selectedHero->tileX, selectedHero->attackRange))
					return;
				for(int i = 0; i < enemyGroup.size(); i++)
				{
					switch(selectedHero->type)
					{
						case T_FOOTMAN:
						case T_KNIGHT:
						case T_ROGUE:
							if(WithinRange(enemyGroup[i]->tileX, indicator.position, 0))
							{
								selectedHero->Attack(enemyGroup[i]);
							}
							break;
						case T_ARCHER:
							if(indicator.position < selectedHero->tileX)
							{
								if(enemyGroup[i]->tileX >= selectedHero->tileX - selectedHero->attackRange)
									selectedHero->Attack(enemyGroup[i]);
							}
							else if(indicator.position > selectedHero->tileX)
							{
								if(enemyGroup[i]->tileX <= selectedHero->tileX + selectedHero->attackRange)
									selectedHero->Attack(enemyGroup[i]);
							}
							break;
					}
				}
				state = GS_SELECT_HERO;
				break;
			case GS_HERO_SPECIAL:
				if(selectedHero == NULL)
				{
					state = GS_SELECT_HERO;
					return;
				}
				
				if(indicator.position == selectedHero->tileX)
					return;
				
				if(!WithinRange(indicator.position, selectedHero->tileX, selectedHero->specialRange))
					return;
				for(int i = 0; i < enemyGroup.size(); i++)
				{
					switch(selectedHero->type)
					{
						case T_ARCHER:
							if(WithinRange(enemyGroup[i]->tileX, indicator.position, 1))
							{
								selectedHero->SpecialAttack(enemyGroup[i]);
							}
							break;
						case T_FOOTMAN:
							if(indicator.position < selectedHero->tileX)
							{
								if(enemyGroup[i]->tileX >= selectedHero->tileX - selectedHero->specialRange)
									selectedHero->SpecialAttack(enemyGroup[i]);
							}
							else if(indicator.position > selectedHero->tileX)
							{
								if(enemyGroup[i]->tileX <= selectedHero->tileX + selectedHero->specialRange)
									selectedHero->SpecialAttack(enemyGroup[i]);
							}
							break;
						case T_ROGUE:
							if(WithinRange(enemyGroup[i]->tileX, indicator.position, 0))
							{
								selectedHero->SpecialAttack(enemyGroup[i]);
							}
							break;
					}
				}
				state = GS_SELECT_HERO;
				break;
		}
	}
} mySystem;

int main()
{
	SetQuickEditMode(false);
	SetWindowSize(windowWidth, windowHeight);
	
	HUD gameTitle((windowWidth / 2) - (78 / 2), (windowHeight / 2) - (8 / 2), 78, 8);
	gameTitle.ImportSprite(NULL, &Spritesheet::GAMETITLE[0]);
	gameTitle.Draw();
	/*
	ASCIISprite gameTitle(&Spritesheet::GAMETITLE[0], NULL, 78, 8);
	gameTitle.Draw((windowWidth / 2) - (78 / 2), (windowHeight / 2) - (8 / 2));
	*/
	/*
	Sprite footmanSprite(&Spritesheet::FOOTMAN[0][0], 16, 16);
	footmanSprite.Draw(0, (windowHeight / 2) - (16 / 2 / 2));
	
	Sprite archerSprite(&Spritesheet::ARCHER[0][0], 16, 16);
	archerSprite.Draw((windowWidth - 1) - 16, (windowHeight / 2) - (16 / 2 / 2));
	*/
	
	Hero* footman = new Footman(windowWidth / 2 - (16 / 2), (windowHeight / 6 * 5) - (16 / 2 / 2), 16, 16);
	footman->ImportSprite(&Spritesheet::FOOTMAN[0][0], NULL);
	footman->tileX = 10;
	mySystem.playerGroup.push_back(footman);
	
	Hero* e_footman = new Footman(windowWidth / 2 - (16 / 2), (windowHeight / 6 * 5) - (16 / 2 / 2), 16, 16);
	e_footman->ImportSprite(&Spritesheet::FOOTMAN[0][0], NULL);
	e_footman->tileX = mySystem.lineLength - 10;
	mySystem.enemyGroup.push_back(e_footman);
	
	Hero* archer = new Archer(windowWidth / 2 - (16 / 2), (windowHeight / 6 * 5) - (16 / 2 / 2), 16, 16);
	archer->ImportSprite(&Spritesheet::ARCHER[0][0], NULL);
	archer->tileX = 5;
	mySystem.playerGroup.push_back(archer);
	
	Hero* e_archer = new Archer(windowWidth / 2 - (16 / 2), (windowHeight / 6 * 5) - (16 / 2 / 2), 16, 16);
	e_archer->ImportSprite(&Spritesheet::ARCHER[0][0], NULL);
	e_archer->tileX = mySystem.lineLength - 5;
	mySystem.enemyGroup.push_back(e_archer);
	
	Hero* rogue = new Rogue(windowWidth / 2 - (16 / 2), (windowHeight / 6 * 5) - (16 / 2 / 2), 16, 16);
	rogue->ImportSprite(&Spritesheet::ROGUE[0][0], NULL);
	rogue->tileX = 8;
	mySystem.playerGroup.push_back(rogue);
	
	Hero* e_rogue = new Rogue(windowWidth / 2 - (16 / 2), (windowHeight / 6 * 5) - (16 / 2 / 2), 16, 16);
	e_rogue->ImportSprite(&Spritesheet::ROGUE[0][0], NULL);
	e_rogue->tileX = mySystem.lineLength - 8;
	mySystem.enemyGroup.push_back(e_rogue);
	
	Hero* knight = new Knight(windowWidth / 2 - (16 / 2), (windowHeight / 6 * 5) - (16 / 2 / 2), 16, 16);
	knight->ImportSprite(&Spritesheet::KNIGHT[0][0], NULL);
	knight->tileX = 6;
	mySystem.playerGroup.push_back(knight);
	
	Hero* e_knight = new Knight(windowWidth / 2 - (16 / 2), (windowHeight / 6 * 5) - (16 / 2 / 2), 16, 16);
	e_knight->ImportSprite(&Spritesheet::KNIGHT[0][0], NULL);
	e_knight->tileX = mySystem.lineLength - 6;
	mySystem.enemyGroup.push_back(e_knight);
	
	GoToXY((windowWidth / 2) - (32 / 2), (windowHeight / 2) + 6);
	system("PAUSE");
	
	do
	{
		system("CLS");
		mySystem.Update();
		//Draw
		mySystem.DrawAll();
		
		GoToXY((windowWidth / 2) - (32 / 2), (windowHeight / 2) + 3);
		system("PAUSE");
		
		if(GetAsyncKeyState(VK_ESCAPE))
			break;
		else
		{
			if(GetAsyncKeyState(VK_SPACE))
			{
				mySystem.Confirm();
				continue;
			}
			if(GetAsyncKeyState(VK_LEFT))
			{
				if
				(
					mySystem.state == GS_SELECT_HERO ||
					mySystem.state == GS_MOVE_HERO ||
					mySystem.state == GS_HERO_ATK ||
					mySystem.state == GS_HERO_SPECIAL
				)
					mySystem.indicator.Move(D_LEFT);
			}
			if(GetAsyncKeyState(VK_RIGHT))
			{
				if
				(
					mySystem.state == GS_SELECT_HERO ||
					mySystem.state == GS_MOVE_HERO ||
					mySystem.state == GS_HERO_ATK ||
					mySystem.state == GS_HERO_SPECIAL
				)
					mySystem.indicator.Move(D_RIGHT);
			}
			if(GetAsyncKeyState(VK_UP))
			{
				if
				(
					mySystem.state == GS_SELECT_ACTION
				)
					mySystem.selector.Move(D_LEFT);
			}
			if(GetAsyncKeyState(VK_DOWN))
			{
				if
				(
					mySystem.state == GS_SELECT_ACTION
				)
					mySystem.selector.Move(D_RIGHT);
			}
		}
	}
	while(true);
	
	system("CLS");
	GoToXY(0, windowHeight - 1);
	return 0;
}
