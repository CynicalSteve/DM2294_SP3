#include <string>
#include "GameObject.h"
using namespace std;

class alienBase : GameObject
{
public:
	alienBase(string alienName, unsigned int alienHealth, unsigned int alienSpeed, unsigned int alienDamage);
	~alienBase();

	string setAlienName();
	int setAlienHealth(), setAlienSpeed(), setAlienDamage();

private:
	string alienName;
	unsigned int alienHealth, alienSpeed, alienDamage;
};