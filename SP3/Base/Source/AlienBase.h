#include <string>

using namespace std;

class alienBase
{
public:
	alienBase(string alienName, unsigned int alienHealth, unsigned int alienSpeed);
	~alienBase();

	string setAlienName();
	int setAlienHealth(), setAlienSpeed();

private:
	string alienName;
	unsigned int alienHealth, alienSpeed;
};

