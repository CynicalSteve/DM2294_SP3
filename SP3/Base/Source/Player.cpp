#include "Player.h"

Player::Player(float playerHealth, float playerSpeed)
{
	this->playerHealth = playerHealth;
	this->playerSpeed = playerSpeed;
}

Player::~Player()
{
}

void Player::setPlayerSpeed(unsigned int newPlayerSpeed)
{
	this->playerSpeed = newPlayerSpeed;
}

void Player::setPlayerHealth(unsigned int newPlayerHealth)
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

void Player::move(char direction, int x, int y, short **map)
{
	switch (direction)
	{
	case 0:
		if (!map[pos.x + 1][pos.y])
			++pos.x;
		break;
	case 1:
		if (!map[pos.x][pos.y + 1])
			++pos.y;
		break;
	case 2:
		if (!map[pos.x - 1][pos.y])
			--pos.x;
		break;
	case 3:
		if (!map[pos.x][pos.y - 1])
			--pos.y;
		break;
	}
}
