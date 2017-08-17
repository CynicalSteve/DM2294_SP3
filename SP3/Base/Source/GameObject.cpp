
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	bombTimer(0.f),
	fireTimer(0.f)
{
}

GameObject::~GameObject()
{
}