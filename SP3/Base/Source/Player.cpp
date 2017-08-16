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
