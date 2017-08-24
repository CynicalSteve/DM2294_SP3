#ifndef ALIEN_GHOUL_H
#define ALIEN_GHOUL_H

#include "AlienBase.h"
using namespace std;

class alienGhoul : public alienBase
{
public:
	alienGhoul(string alienName, float alienHealth, float alienSpeed, float alienDamage, int alienCurrencyWorth, short posX, short posY);
	virtual ~alienGhoul();
};
#endif