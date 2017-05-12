//Monolithic library/header
// # = C++ pre-processors/macros
// ! Include Guard (make sure it is only included/defined once)
#ifndef	_ALL_HEADER_FILES_H_ //if not defined
#define	_ALL_HEADER_FILES_H_ //define it or else go to endif(bottom)

// ! Normal library/header
#include <iostream>
#include <cmath>
#include <fstream>
#include <windows.h>
#include <vector>

#include <time.h>

using namespace std;

#define BOUNDARY_LEFT 5
#define BOUNDARY_RIGHT 55
#define BOUNDARY_TOP 2
#define BOUNDARY_BOTTOM 30

#define ENEMY_SPAWN_DURATION 50
#define MAX_BULLET 30
#define MAX_ENEMY 5
#define BOSS_SPAWN_SCORE 5000

typedef enum{
        CRASHER,
        SHOOTER,
        BOSS,
        TOTAL_ENEMY_TYPE
} ENEMY_TYPE;

typedef enum{
        STRAIGHT,
        DUAL,
        TOTAL_BULLET_TYPE
} BULLET_TYPE;

typedef enum{
        REGEN,
        ARMOR,
        PENETRATE,
        TOTAL_POWERUP_TYPE
} POWERUP_TYPE;

//External declaration
// ! Externally declaring the class Enemy before the actual declaration
extern "C" class Enemy;

//! custom classes
#include "conio_yp.h"
#include "GameObject.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "Powerup.h"

#endif //end if defined
