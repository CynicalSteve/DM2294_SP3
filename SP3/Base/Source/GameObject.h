#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

class GameObject
{
public:
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
		GO_NORMALBOMB,
		GO_BOMBFIRE,
		GO_ALIENGRUB,
		GO_WALL_LVL1,
		GO_WALL_LVL2,
		GO_WALL_LVL3,
		GO_WALL_LVL4,
		GO_WALL_LVL5,
		GO_LOOTCRATE,

		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	struct coord
	{
		short x = 0, y = 0;
		void set(short x, short y)
		{
			this->x = x;
			this->y = y;
		}
	} pos;
	Vector3 animationPos;
	Vector3 scale;
	Vector3 dir; //direction/orientation
	float momentOfInertia;
	float angularVelocity; //in radians
	bool active;
	float mass;

	float fireBurnTime;
	float fireScaler;

	GameObject();
	virtual ~GameObject();
	bool move(char direction, short **map);

	friend bool operator == (coord &lhs, coord &rhs);
};

#endif