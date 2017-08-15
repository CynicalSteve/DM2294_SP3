#include "AlienBase.h"

alienBase::alienBase(string alienName, unsigned int alienHealth, unsigned int alienSpeed)
{
	this->alienName = alienName;
	this->alienHealth = alienHealth;
	this->alienSpeed = alienSpeed;
}

alienBase::~alienBase()
{
}