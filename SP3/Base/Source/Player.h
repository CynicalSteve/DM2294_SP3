#include <string>
#include "GameObject.h"
using namespace std;

class Player : public GameObject
{
public:
	Player (float playerHealth, float playerSpeed);
	~Player();

	void setPlayerHealth(unsigned int newPlayerHealth), setPlayerSpeed(unsigned int newPlayerSpeed);
	float getPlayerHealth(), getPlayerSpeed();
	void move(char direction, int x, int y, short **map);

private:
	float playerHealth, playerSpeed;
};

