#ifndef ALIEN_GRUB_H
#define ALIEN_GRUB_H

#include "AlienBase.h"
using namespace std;

class alienRaptor : public alienBase
{
public:
	alienRaptor(string alienName, float alienHealth, float alienSpeed, float alienDamage, int alienCurrencyWorth, short posX, short posY);
	virtual ~alienRaptor();
};
#endif