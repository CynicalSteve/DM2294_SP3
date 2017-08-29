#include "AlienBase.h"

alienBase::alienBase() : alienName(""), alienHealth(0), alienDamage(0), alienSpeed(0)
{
	active = false;
}

alienBase::~alienBase()
{
}

string alienBase::getAlienName()
{
	return this->alienName;
}

float alienBase::getAlienHealth()
{
	return this->alienHealth;
}

float alienBase::getAlienSpeed()
{
	return this->alienSpeed;
}

float alienBase::getAlienDamage()
{
	return this->alienDamage;
}

int alienBase::getAlienCurrencyWorth()
{
	return this->alienCurrencyWorth;
}

void alienBase::setAlienName(string newAlienName)
{
	this->alienName = newAlienName;
}

void alienBase::setAlienHealth(float newAlienHealth)
{
	this->alienHealth = newAlienHealth;
}

void alienBase::setAlienSpeed(float newAlienSpeed)
{
	this->alienSpeed = newAlienSpeed;
}

void alienBase::setAlienDamage(float newAlienDamage)
{
	this->alienDamage = newAlienDamage;
}

void alienBase::setAlienCurrencyWorth(int newAlienCurrencyWorth)
{
	 this->alienCurrencyWorth = newAlienCurrencyWorth;
}

void alienBase::addAlienHealth(int alienHealthToAdd)
{
	this->alienHealth += alienHealthToAdd;
}

void alienBase::subtractAlienHealth(int alienHealthToSubtract)
{
	this->alienHealth -= alienHealthToSubtract;
}

void alienBase::createSpawnPosition(short **map, short size)
{
	std::vector<coord> perimeter;
	coord target;
	for (; target.x < size - 1; ++target.x)
	{
		perimeter.push_back(target);
	}
	for (; target.y < size - 1; ++target.y)
	{
		perimeter.push_back(target);
	}
	for (; target.x > 0; --target.x)
	{
		perimeter.push_back(target);
	}
	for (; target.y > 0; --target.y)
	{
		perimeter.push_back(target);
	}
	for (; target.x < perimeter.size(); ++target.x)
	{
		if (!map[perimeter[target.x].x][perimeter[target.x].y])
			spawnPosition.push_back(perimeter[target.x]);
	}
	while (spawnPosition.size() < 6)
	{
<<<<<<< HEAD
		target.x = Math::RandIntMinMax(0, perimeter.size());
 		if (map[perimeter[target.x].x][perimeter[target.x].y])
=======
		target.x = Math::RandIntMinMax(0, perimeter.size() - 1);
		if (map[perimeter[target.x].x][perimeter[target.x].y])
>>>>>>> 1106b14b08521a525a197afe45a0344b25c9b255
		{
			map[perimeter[target.x].x][perimeter[target.x].y] = 0;
			spawnPosition.push_back(perimeter[target.x]);
		}
	}
}