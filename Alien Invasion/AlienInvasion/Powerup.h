
#ifndef	_POWERUP_H_
#define	_POWERUP_H_

#include "AllHeaderFiles.h"

using namespace std;

class Powerup : public GameObject
{
public :
	POWERUP_TYPE type;
	bool isAlive;
	
	int updateDelayTimer;
	int updateDelayDuration;
	
	Powerup(POWERUP_TYPE enemyType);
	
	void Update();
};

#endif
