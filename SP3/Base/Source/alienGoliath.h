#ifndef ALIEN_GOLIATH_H
#define ALIEN_GOLIATH_H

#include "AlienBase.h"
using namespace std;

class alienGoliath : public alienBase
{
public:
	alienGoliath(string alienName, float alienHealth, float alienSpeed, float alienDamage, int alienCurrencyWorth, short posX, short posY);
	virtual ~alienGoliath();
};
#endif