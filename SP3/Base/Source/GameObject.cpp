
#include "GameObject.h"

GameObject::GameObject() 
	: scale(1, 1, 1),
	active(false),
	mass(1.f),
	fireBurnTime(0.f)
{
}

GameObject::~GameObject()
{
}