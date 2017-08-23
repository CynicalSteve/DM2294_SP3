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

	void setBombName(string bombName), setBombDamage(float bombDamage), setBombTimer(float timeToExplode), setTriggeredState(bool newTriggeredState);
	string getBombName();
	float getBombDamage(), getTimeToExplode(), getFireBurnTime();
	bool getTriggeredState();

	float bombTimer;
	
protected:
	string bombName;
	float bombDamage, timeToExplode;
	bool Triggered;
};
#endif

