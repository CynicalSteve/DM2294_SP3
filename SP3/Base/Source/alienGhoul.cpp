#include "alienGhoul.h"

alienGhoul::alienGhoul(string alienName, float alienHealth, float alienSpeed, float alienDamage, int alienCurrencyWorth, short posX, short posY)
{
	this->alienName = alienName;
	this->alienHealth = alienHealth;
	originalHealth = alienHealth;
	this->alienSpeed = alienSpeed;
	this->alienDamage = alienDamage;
	this->alienCurrencyWorth = alienCurrencyWorth;

	this->alienType = alienBase::TYPE2_GHOUL;

	this->pos.set(posX, posY);
	this->animationPos.Set(posX, posY);
}

alienGhoul::~alienGhoul()
{
}