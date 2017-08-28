#include "alienGoliath.h"

alienGoliath::alienGoliath(string alienName, float alienHealth, float alienSpeed, float alienDamage, int alienCurrencyWorth, short posX, short posY)
{
	this->alienName = alienName;
	this->alienHealth = alienHealth;
	this->alienSpeed = alienSpeed;
	this->alienDamage = alienDamage;
	this->alienCurrencyWorth = alienCurrencyWorth;

	this->alienType = alienBase::TYPE4_GOLIATH;

	this->pos.set(posX, posY);
	this->animationPos.Set(posX, posY);
}

alienGoliath::~alienGoliath()
{
}