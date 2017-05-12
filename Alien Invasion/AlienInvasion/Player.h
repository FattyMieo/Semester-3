
#ifndef	_PLAYER_H_
#define	_PLAYER_H_

#include "AllHeaderFiles.h"

using namespace std;

class Player : public GameObject
{
public :
	
	int health;
	int armor;
	int score;
	int shootDelayTimer;
	int shootDelayDuration;
	
	int regenTime;
	int regenDelayTimer;
	int regenDelayDuration;
	
	int penetrateAmmo;
	
	BULLET_TYPE bulletType;
	
	vector<Bullet> bulletList;
	
	Player();
	
	void Update();
	
	void Shoot();
	
	void SwitchWeapon();
};

#endif
