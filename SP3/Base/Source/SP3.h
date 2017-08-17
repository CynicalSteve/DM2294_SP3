#ifndef SP3_H
#define SP3_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "AlienBase.h"
#include "alienGrub.h"
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
	void renderAliens(alienBase *alien);

	GameObject* FetchGO();
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_worldWidth;
	float m_worldHeight;
	Player* playerInfo;
	int theMap[11][11];
	GameObject *myInventory[10];
	double doubletime;
	alienBase** alienManager;
	int currentAlien;

	bool KeyBounce[256];
};

#endif