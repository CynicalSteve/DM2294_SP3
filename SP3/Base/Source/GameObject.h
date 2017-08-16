#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_ASTEROID,
		GO_BALL,
		GO_BULLET,
		GO_CUBE,
		GO_SHIP,
		GO_ENEMY, //enemy ship
		GO_ENEMY_BULLET, //enemy bullet
		GO_MISSILE, //player missile
		GO_PILLAR,
		GO_POWERUP, //powerup item
		GO_WALL,

		//SP3
		GO_PLAYER,
		GO_BOMB,

		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 dir; //direction/orientation
	float momentOfInertia;
	float angularVelocity; //in radians
	bool active;
	float mass;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif