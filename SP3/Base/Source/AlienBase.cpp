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

float alienBase::getAlienHealth()
{
	return this->alienHealth;
}

float alienBase::getAlienSpeed()
{
	return this->alienSpeed;
}

float alienBase::getAlienDamage()
{
	return this->alienDamage;
}

int alienBase::getAlienCurrencyWorth()
{
	return this->alienCurrencyWorth;
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

void alienBase::setAlienCurrencyWorth(int newAlienCurrencyWorth)
{
	 this->alienCurrencyWorth = newAlienCurrencyWorth;
}
