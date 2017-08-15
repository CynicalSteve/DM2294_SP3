#include "BombBase.h"

BombBase::BombBase(string bombName, unsigned int bombDamage)
{
	this->bombName = bombName;
	this->bombDamage = bombDamage;
}

BombBase::~BombBase()
{
}