#ifndef BOMB_BASE_H
#define BOMB_BASE_H

#include <string>
#include "GameObject.h"
using namespace std;

class BombBase : public GameObject
{
public:
	BombBase();
	~BombBase();

	struct coord
	{
		short x = 0, y = 0;
		void set(short x, short y)
		{
			this->x = x;
			this->y = y;
		}
	} pos;

	void setBombName(string bombName), setBombDamage(float bombDamage), setBombTimer(float timeToExplode);
	string getBombName();
	float getBombDamage(), getTimeToExplode(), getFireBurnTime();

	float bombTimer;
protected:
	string bombName;
	float bombDamage, timeToExplode;
};
#endif

