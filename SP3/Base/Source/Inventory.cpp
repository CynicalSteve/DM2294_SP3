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