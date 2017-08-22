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
	int getEquipmentCurrency();

	vector<BombBase*> bombManager;
	int currentBomb;

private:
	float playerHealth, playerSpeed;
	int equipmentCurrency;
};

