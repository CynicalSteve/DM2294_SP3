#ifndef BOMB_MINE_H
#define BOMB_MINE_H

#include <string>
#include "BombBase.h"
using namespace std;

class MineBomb : public BombBase
{
public:
	MineBomb(string bombName, float bombDamage, float fireBurnTime, short posX, short posY);
	~MineBomb();
};
#endif

