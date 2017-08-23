#include "BombMine.h"

MineBomb::MineBomb(string bombName, float bombDamage, float fireBurnTime, short posX, short posY)
{
	this->bombName = bombName;
	this->bombDamage = bombDamage;
	this->fireBurnTime = fireBurnTime;
	this->type = GameObject::GO_MINEBOMB;
	this->active = false;
	this->Triggered = false;

	this->pos.set(posX, posY);
}

MineBomb::~MineBomb()
{
}
