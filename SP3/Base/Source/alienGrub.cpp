#include "alienGrub.h"

alienGrub::alienGrub(string alienName, float alienHealth, float alienSpeed, float alienDamage, short posX, short posY)
{
	this->alienName = alienName;
	this->alienHealth = alienHealth;
	this->alienSpeed = alienSpeed;
	this->alienDamage = alienDamage;

	this->alienType = alienBase::TYPE1_GRUB;

	this->pos.set(posX, posY);
	this->animationPos.Set(posX, posY);
}

alienGrub::~alienGrub()
{
}