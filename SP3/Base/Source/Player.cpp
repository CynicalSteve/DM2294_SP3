#include "Player.h"

Player::Player(float playerHealth, float playerSpeed) : bombManager(NULL), currentBomb(0), playerHealth(playerHealth), playerSpeed(playerSpeed), equipmentCurrency(0)
{
	active = true;

	for (unsigned int i = 0; i < 5; ++i)
	{
		playerInventory[i] = new Inventory();
	}

	playerInventory[0]->inventoryBombType = Inventory::INVENTORY_NORMALBOMB;
	playerInventory[0]->setDiscoveredState(true);

	playerInventory[1]->inventoryBombType = Inventory::INVENTORY_MINEBOMB;
}

Player::~Player()
{
}

void Player::setPlayerSpeed(float newPlayerSpeed)
{
	this->playerSpeed = newPlayerSpeed;
}

void Player::setPlayerHealth(float newPlayerHealth)
{
	this->playerHealth = newPlayerHealth;
}

void Player::setPlayerCurrency(int newCurrencyAmount)
{
	this->equipmentCurrency = newCurrencyAmount;
}

void Player::addCurrency(int currencyToAdd)
{
	this->equipmentCurrency += currencyToAdd;
}

void Player::subtractCurrency(int currencyToSubtract)
{
	this->equipmentCurrency -= currencyToSubtract;
}

float Player::getPlayerHealth()
{
	return this->playerHealth;
}

float Player::getPlayerSpeed()
{
	return this->playerSpeed;
}

int Player::getEquipmentCurrency()
{
	return this->equipmentCurrency;
}
