#include "alienLeviathan.h"

alienLeviathan::alienLeviathan(string alienName, float alienHealth, float alienSpeed, float alienDamage, int alienCurrencyWorth, short posX, short posY)
{
	this->alienName = alienName;
	this->alienHealth = alienHealth;
	this->alienSpeed = alienSpeed;
	this->alienDamage = alienDamage;
	this->alienCurrencyWorth = alienCurrencyWorth;

	this->alienType = alienBase::TYPE5_LEVIATHAN;

	this->pos.set(posX, posY);
	this->animationPos.Set(posX, posY);
}

alienLeviathan::~alienLeviathan()
{
}