#ifndef BOMB_NORMAL_H
#define BOMB_NORMAL_H

#include <string>
#include "BombBase.h"
using namespace std;

class NormalBomb : public BombBase
{
public:
	NormalBomb(string bombName, float bombDamage, float timeToExplode, float fireBurnTime, short posX, short posY);
	~NormalBomb();
};
#endif

