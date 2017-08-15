#include "AlienBase.h"

alienBase::alienBase(string alienName, unsigned int alienHealth, unsigned int alienSpeed, unsigned int alienDamage)
{
	this->alienName = alienName;
	this->alienHealth = alienHealth;
	this->alienSpeed = alienSpeed;
	this->alienDamage = alienDamage;
}

alienBase::~alienBase()
{
}