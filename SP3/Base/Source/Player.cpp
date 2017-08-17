#include "Player.h"

Player::Player(float playerHealth, float playerSpeed) : bombManager(NULL), currentBomb(0)
{
	this->playerHealth = playerHealth;
	this->playerSpeed = playerSpeed;
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
