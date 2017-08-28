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
	void setSpeedBoostState(bool newSpeedBoostState), setMaxSpeedCooldownTime(float newSpeedBoostTime), setMaxInvulnerableTime(float newMaxInvulnerableTime);
	void addCurrency(int currencyToAdd), subtractCurrency(int currencyToSubtract), addHealth(int healthToAdd), subtractHealth(int healthToSubtract);
	void setInvulnerabilityState(bool newInvulnerabilityState);
	float getPlayerHealth(), getPlayerSpeed(), getMaxSpeedBoostCooldownTime(), getMaxInvulnerableTime();
	int getEquipmentCurrency(), getMaxPlayerHealth();
	bool getSpeedBoostState(), getInvulnerabilityState();

	vector<BombBase*> bombManager;
	int currentBomb;
	Inventory* playerInventory[5];
	float speedBoostCooldown;
	float normalSpeed;
	float invulnerabilityCooldown;
	float invulnerabilityHealth;

	//NUKE
	void addToTimer(double dt), setMaxBombTImer(float newMaxBombTimer), setNukeDeployedState(bool state), setCurrentTimer(float newCurrentTimer);
	float getCurrentTimer(), getMaxBombTimer();
	bool getNukeDeployedState();
	float countdown;

private:
	float playerHealth, playerSpeed, maxPlayerHealth, maxSpeedBoostCooldownTime, maxInvulnerableTime;
	int equipmentCurrency;
	bool speedBoosted;
	bool Invulnerability;

	//NUKE
	float currentBombTimer, maxBombTimer;
	bool nukeDeployed;
};
#endif

