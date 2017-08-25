#include "alienGrub.h"

alienGrub::alienGrub(string alienName, float alienHealth, float alienSpeed, float alienDamage, int alienCurrencyWorth, short posX, short posY)
{
	this->alienName = alienName;
	this->alienHealth = alienHealth;
	this->alienSpeed = alienSpeed;
	this->originalSpeed = alienSpeed;
	this->alienDamage = alienDamage;
	this->alienCurrencyWorth = alienCurrencyWorth;

	this->alienType = alienBase::TYPE1_GRUB;

	this->pos.set(posX, posY);
	this->animationPos.Set(posX, posY);
}

alienGrub::~alienGrub()
{
}