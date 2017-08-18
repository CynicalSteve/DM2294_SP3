#include "BombBase.h"

BombBase::BombBase() : bombTimer(0.f)
{
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

float BombBase::getTimeToExplode()
{
	return this->timeToExplode;
}

float BombBase::getFireBurnTime()
{
	return this->fireBurnTime;
}
