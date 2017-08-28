#include "Inventory.h"

Inventory::Inventory() : bombAmount(0), maxAmountOfBombs(99), isDiscovered(false)
{
}

Inventory::~Inventory()
{
}

void Inventory::setBombAmount(unsigned int newBombAmount)
{
	this->bombAmount = newBombAmount;
}

void Inventory::setMaxAmountOfBombs(unsigned int newMaxBombsAmount)
{
	this->maxAmountOfBombs = newMaxBombsAmount;
}

void Inventory::setDiscoveredState(bool newDiscoveredState)
{
	this->isDiscovered = newDiscoveredState;
}

unsigned int Inventory::getBombAmount()
{
	return this->bombAmount;
}

unsigned int Inventory::getMaxAmountOfBombs()
{
	return this->bombAmount;
}

unsigned int Inventory::getDiscoveredState()
{
	return this->isDiscovered;
}

std::string Inventory::getCurrentBombName()
{
	if (inventoryBombType == INVENTORY_NORMALBOMB)
	{
		return "Normal Bomb";
	}
	else if (inventoryBombType == INVENTORY_MINEBOMB)
	{
		return "Mine Bomb";
	}
	else if (inventoryBombType == INVENTORY_NUKEBOMB)
	{
		return "Nuke Bomb";
	}
}

void Inventory::addBombAmount(int bombNumberToAdd)
{
	this->bombAmount += bombNumberToAdd;
}

void Inventory::subtractBombAmount(int bombNumberToSubtract)
{
	this->bombAmount -= bombNumberToSubtract;
}
