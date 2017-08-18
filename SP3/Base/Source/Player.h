#include <string>
#include <vector>
#include "GameObject.h"
#include "BombBase.h"

using namespace std;

class Player : public GameObject
{
public:
	Player (float playerHealth, float playerSpeed);
	~Player();

	void setPlayerHealth(float newPlayerHealth), setPlayerSpeed(float newPlayerSpeed);
	float getPlayerHealth(), getPlayerSpeed();
	void move(char direction, int x, int y, short **map);

	vector<BombBase*> bombManager;
	int currentBomb;

private:
	float playerHealth, playerSpeed;
};

