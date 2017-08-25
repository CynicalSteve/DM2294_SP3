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
		TYPE1_GRUB,
		TYPE2_GHOUL,
		TYPE3_RAPTOR,
		TYPE4_GOLIATH,
		TYPE5_LEVIATHAN
	};

	alienBase(); //string alienName, unsigned int alienHealth, unsigned int alienSpeed, unsigned int alienDamage
	~alienBase();

	string getAlienName();
	float getAlienHealth(), getAlienSpeed(), getAlienDamage();
	int getAlienCurrencyWorth();
	void setAlienName(string newAlienName), setAlienHealth(float newAlienHealth), setAlienSpeed(float newAlienSpeed), setAlienDamage(float newAlienDamage), setAlienCurrencyWorth(int newAlienCurrencyWorth);
	void addAlienHealth(int alienHealthToAdd), subtractAlienHealth(int alienHealthToSubtract);

	Vector3 scale;
	alienType alienType;

	float originalSpeed;

protected:
	string alienName;
	float alienHealth, alienSpeed, alienDamage;
	int alienCurrencyWorth;
};
#endif