#include "AllHeaderFiles.h"

class Bullet : public GameObject
{
public :
	BULLET_TYPE type;
	
	int damage;
	bool isAlive;
		
	Bullet(BULLET_TYPE type);
	
	void Update();
};

class EnemyBullet : public Bullet
{
public :
	EnemyBullet(BULLET_TYPE type);
	
	void Update();
};
