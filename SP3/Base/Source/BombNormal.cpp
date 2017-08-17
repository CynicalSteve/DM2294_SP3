#include "BombNormal.h"

NormalBomb::NormalBomb(string bombName, float bombDamage, float timeToExplode, float fireBurnTime, short posX, short posY)
{
	this->bombName = bombName;
	this->bombDamage = bombDamage;
	this->timeToExplode = timeToExplode;
	this->fireBurnTime = fireBurnTime;

	this->pos.set(posX, posY);
}

NormalBomb::~NormalBomb()
{
}
