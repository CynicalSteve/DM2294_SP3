#include <string>
#include "GameObject.h"
using namespace std;

class BombBase : GameObject
{
public:
	BombBase(string bombName, float bombDamage);
	~BombBase();
	
	void setBombName(string bombName), setBombDamage(float bombDamage), setBombTimer(float timeToExplode);
	string getBombName();
	float getBombDamage(), getBombTimer();

private:
	string bombName;
	float bombDamage, timeToExplode;
};

