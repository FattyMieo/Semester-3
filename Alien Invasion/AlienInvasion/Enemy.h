
#ifndef	_ENEMY_H_
#define	_ENEMY_H_

#include "AllHeaderFiles.h"

using namespace std;

class Enemy : public GameObject
{
public:
	ENEMY_TYPE enemyType;
	int health;
	bool isAlive;
	
	BULLET_TYPE bulletType;
	
	int updateDelayTimer;
	int updateDelayDuration;
	int shootDelayTimer;
	int shootDelayDuration;
	
	Player* player;
	vector<EnemyBullet>* bulletList;
	
	Enemy(ENEMY_TYPE enemyType);
	
	void Update();
	
	void Shoot();
};

enum BossAction
{
	BAM = 0,
	RAM,
	HAM,
	TOTAL_BOSS_ACTIONs
};

class Boss : public Enemy
{
public:
	bool panningLeft;
	bool vulnerable;
	bool ramming;
	BossAction currentAction;
	int actionTimer;
	int actionDuration;
	
	Boss();
	
	void Update();
	
	void ChooseRandomAction();
	void Ram();
	void Bam();
};

#endif
