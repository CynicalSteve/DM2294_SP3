#ifndef SP3_H
#define SP3_H

#include "GameObject.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "alienGrub.h"
#include "alienGhoul.h"
#include "alienRaptor.h"
#include "BombBase.h"
#include "BombNormal.h"
#include "BombMine.h"
#include "Player.h"
#include "Map.h"

#include <vector>
#include <cstdlib>
#include <ctime>

class SP3 : public SceneBase
{
public:
	SP3();
	~SP3();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	void renderAliens(alienBase *alien);
	void renderBombs(BombBase *bomb, int currentBombIndex);
	void renderUI();
	void AlienMovement(double dt);
	void PlayerChecks(double dt);

	int RandomNumberGen(int FirstNumber = 0, int LastNumber = 0);

	GameObject * FetchGO();

protected:
	std::vector<GameObject *> m_goList;
	Player * playerInfo;
	short **theMap;
	short mapSize;
	//GameObject *myInventory[10];
	double doubletime;
	std::vector<alienBase *> alienManager;

	int currentAlien;

	bool KeyBounce[256];
};

#endif