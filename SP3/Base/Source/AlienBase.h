#ifndef ALIEN_BASE_H
#define ALIEN_BASE_H

#include <string>
#include "Vector3.h"
using namespace std;

class alienBase
{
public:
	enum alienType
	{
		TYPE_GRUB
	};

	struct coord
	{
		short x = 0, y = 0;
		void set(short x, short y)
		{
			this->x = x;
			this->y = y;
		}
	} pos;

	Vector3 scale;
	alienBase(); //string alienName, unsigned int alienHealth, unsigned int alienSpeed, unsigned int alienDamage
	virtual ~alienBase();

	virtual string setAlienName();
	virtual int setAlienHealth(), setAlienSpeed(), setAlienDamage();
	alienType alienType;

protected:
	string alienName;
	unsigned int alienHealth, alienSpeed, alienDamage;
};
#endif