#include "GameObject.h"

GameObject::GameObject()
	: scale(1, 1, 1),
	active(false),
	mass(1.f),
	fireBurnTime(0.f),
	fireScaler(0.f),
	loseHealthCooldown(0.f)
{ 
}

GameObject::~GameObject()
{
}

bool GameObject::move(char direction, short **map)
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
	case 4:
		switch ((short)animationPos.z)
		{
			//take whole number to find speed (up to 1 unit/frame)
		case -1:
			return false;
		case 0:
			if (animationPos.x + animationPos.z > pos.x)
			{
				animationPos.x = pos.x;
				animationPos.z = -1;
			}
			else animationPos.x += animationPos.z;
			return true;
		case 1:
			if (animationPos.y + animationPos.z - 1 > pos.y)
			{
				animationPos.y = pos.y;
				animationPos.z = -1;
			}
			else animationPos.y += animationPos.z - 1;
			return true;
		case 2:
			if (animationPos.x - animationPos.z + 2 < pos.x)
			{
				animationPos.x = pos.x;
				animationPos.z = -1;
			}
			else animationPos.x -= animationPos.z - 2;
			return true;
		case 3:
			if (animationPos.y - animationPos.z + 3 < pos.y)
			{
				animationPos.y = pos.y;
				animationPos.z = -1;
			}
			else animationPos.y -= animationPos.z - 3;
			return true;
		default:
			return false;
		}
	default:
		if (animationPos.z > 0 && animationPos.z < 4)
			return false;
		return true;
	}
}

bool operator == (GameObject::coord &lhs, GameObject::coord &rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator > (GameObject::coord &lhs, GameObject::coord &rhs)
{
	return lhs.x > rhs.x && lhs.y > rhs.y;
}

bool operator < (GameObject::coord &lhs, GameObject::coord &rhs)
{
	return lhs.x < rhs.x && lhs.y < rhs.y;
}