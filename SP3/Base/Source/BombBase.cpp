#include "BombBase.h"

BombBase::BombBase(string bombName, float bombDamage)
{
	this->bombName = bombName;
	this->bombDamage = bombDamage;
}

BombBase::~BombBase()
{
}

void BombBase::setBombName(string bombName)
{
	this->bombName = bombName;
}

void BombBase::setBombDamage(float bombDamage)
{
	this->bombDamage = bombDamage;
}

void BombBase::setBombTimer(float timeToExplode)
{
	this->timeToExplode = timeToExplode;
}

string BombBase::getBombName()
{
	return this->bombName;
}

float BombBase::getBombDamage()
{
	return this->bombDamage;
}

float BombBase::getBombTimer()
{
	return this->timeToExplode;
}
