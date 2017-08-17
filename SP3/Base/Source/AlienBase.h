#ifndef ALIEN_BASE_H
#define ALIEN_BASE_H

#include <string>
#include "Vector3.h"
#include "GameObject.h"

using namespace std;

class alienBase : public GameObject
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

	
	alienBase(); //string alienName, unsigned int alienHealth, unsigned int alienSpeed, unsigned int alienDamage
	~alienBase();

	string getAlienName();
	int getAlienHealth(), getAlienSpeed(), getAlienDamage();
	void setAlienName(string newAlienName), setAlienHealth(float newAlienHealth), setAlienSpeed(float newAlienSpeed), setAlienDamage(float newAlienDamage);
	
	Vector3 scale;
	alienType alienType;

protected:
	string alienName;
	float alienHealth, alienSpeed, alienDamage;
};
#endif