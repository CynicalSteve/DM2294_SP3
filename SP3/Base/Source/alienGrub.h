#ifndef ALIEN_GRUB_H
#define ALIEN_GRUB_H

#include "AlienBase.h"
using namespace std;

class alienGrub : public alienBase
{
public:
	alienGrub(string alienName, float alienHealth, float alienSpeed, float alienDamage, int alienCurrencyWorth, short posX, short posY);
	virtual ~alienGrub();
};
#endif