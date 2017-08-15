#include <string>
#include "GameObject.h"
using namespace std;

class BombBase : GameObject
{
public:
	BombBase(string bombName, unsigned int bombDamage);
	~BombBase();

	string setBombName();
	int setBombDamage();

private:
	string bombName;
	unsigned int bombDamage;
};

