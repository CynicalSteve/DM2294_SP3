#include "Player.h"

Player::Player(float playerHealth, float playerSpeed) : bombManager(NULL), currentBomb(0), playerHealth(playerHealth), playerSpeed(playerSpeed)
{
	active = true;
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

float Player::getPlayerHealth()
{
	return this->playerHealth;
}

float Player::getPlayerSpeed()
{
	return this->playerSpeed;
}

bool Player::move(char direction, short **map)
{
	switch (direction)
	{
	case 0:
		if (!map[pos.x + 1][pos.y])
		{
			++pos.x;
			return true;
		}
		return false;
	case 1:
		if (!map[pos.x][pos.y + 1])
		{
			++pos.y;
			return true;
		}
		return false;
	case 2:
		if (!map[pos.x - 1][pos.y])
		{
			--pos.x;
			return true;
		}
		return false;
	case 3:
		if (!map[pos.x][pos.y - 1])
		{
			--pos.y;
			return true;
		}
		return false;
	}
}
