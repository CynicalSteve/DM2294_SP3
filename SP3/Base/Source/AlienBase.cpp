#include "AlienBase.h"

alienBase::alienBase() : alienName(""), alienHealth(0), alienDamage(0), alienSpeed(0)
{
}

alienBase::~alienBase()
{
}

string alienBase::getAlienName()
{
	return this->alienName;
}

int alienBase::getAlienHealth()
{
	return this->alienHealth;
}

int alienBase::getAlienSpeed()
{
	return this->alienSpeed;
}

int alienBase::getAlienDamage()
{
	return this->alienDamage;
}

void alienBase::setAlienName(string newAlienName)
{
	this->alienName = newAlienName;
}

void alienBase::setAlienHealth(float newAlienHealth)
{
	this->alienHealth = newAlienHealth;
}

void alienBase::setAlienSpeed(float newAlienSpeed)
{
	this->alienSpeed = newAlienSpeed;
}

void alienBase::setAlienDamage(float newAlienDamage)
{
	this->alienDamage = newAlienDamage;
}
