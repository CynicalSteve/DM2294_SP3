#ifndef ALIEN_LEVIATHAN_H
#define ALIEN_LEVIATHAN_H

#include "AlienBase.h"
using namespace std;

class alienLeviathan : public alienBase
{
public:
	alienLeviathan(string alienName, float alienHealth, float alienSpeed, float alienDamage, int alienCurrencyWorth, short posX, short posY);
	virtual ~alienLeviathan();
};
#endif