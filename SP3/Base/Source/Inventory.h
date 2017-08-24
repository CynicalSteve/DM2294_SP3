#ifndef INVENTORY_H
#define INVENTORY_H

#include "GameObject.h"
using namespace std;

class Inventory
{
public:
	Inventory();
	~Inventory();

	enum INVENTORY_BOMB
	{
		INVENTORY_NORMALBOMB,
		INVENTORY_MINEBOMB,
		TOTAL_NUM
	};

	INVENTORY_BOMB inventoryBombType;

	void setBombAmount(unsigned int newBombAmount), setMaxAmountOfBombs(unsigned int newMaxBombsAmount), setDiscoveredState(bool newDiscoveredState);
	unsigned int getBombAmount(), getMaxAmountOfBombs(), getDiscoveredState();
	std::string getCurrentBombName();

	void addBombAmount(int bombNumberToAdd), subtractBombAmount(int bombNumberToSubtract);

private:
	unsigned int bombAmount;
	unsigned int maxAmountOfBombs;

	bool isDiscovered;
};
#endif

