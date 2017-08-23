#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "GameObject.h"
#include "BombBase.h"
#include "Inventory.h"

using namespace std;

class Player : public GameObject
{
public:
	Player (float playerHealth, float playerSpeed);
	~Player();

	void setPlayerHealth(float newPlayerHealth), setPlayerSpeed(float newPlayerSpeed), setPlayerCurrency(int newCurrencyAmount), addCurrency(int currencyToAdd), subtractCurrency(int currencyToSubtract);
		
	float getPlayerHealth(), getPlayerSpeed();
	int getEquipmentCurrency();

	vector<BombBase*> bombManager;
	int currentBomb;
	Inventory* playerInventory[5];

private:
	float playerHealth, playerSpeed;
	int equipmentCurrency;
};
#endif

