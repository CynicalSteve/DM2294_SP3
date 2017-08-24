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

	void setPlayerHealth(float newPlayerHealth), setPlayerSpeed(float newPlayerSpeed), setMaxPlayerHealth(float newMaxPlayerHealth), setPlayerCurrency(int newCurrencyAmount);
	void setSpeedBoostState(bool newSpeedBoostState), setMaxSpeedCooldownTime(float newSpeedBoostTime);
	void addCurrency(int currencyToAdd), subtractCurrency(int currencyToSubtract), addHealth(int healthToAdd), subtractHealth(int healthToSubtract);
	float getPlayerHealth(), getPlayerSpeed(), getMaxSpeedBoostCooldownTime();
	int getEquipmentCurrency(), getMaxPlayerHealth();
	bool getSpeedBoostState();

	vector<BombBase*> bombManager;
	int currentBomb;
	Inventory* playerInventory[5];
	float speedBoostCooldown;
	float normalSpeed;

private:
	float playerHealth, playerSpeed, maxPlayerHealth, maxSpeedBoostCooldownTime;
	int equipmentCurrency;
	bool speedBoosted;
};
#endif

