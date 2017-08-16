#ifndef SP3_H
#define SP3_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "AlienBase.h"
#include "BombBase.h"
#include "Player.h"

class SP3 : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	SP3();
	~SP3();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ship;
	Vector3 m_force;
	Vector3 m_torque;
	int m_objectCount;
	int m_lives;
	int m_score;
	Player* playerinfo;

	bool KeyBounce[256];
};

#endif