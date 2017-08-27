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
	void RenderPauseUI();

	void ReadSettings();
	void renderShopScreen();

	void AlienMovement(double dt);
	void PlayerChecks(double dt);
	void BombFireCreation(double dt);
	void m_goListInteractions(double dt);

	int RandomNumberGen(int FirstNumber = 0, int LastNumber = 0);

	GameObject * FetchGO();

	enum PauseSelection
	{
		CONTINUE,
		SETTINGS,
		EXIT_MAINMENU,
		EXIT_GAME,
		TOTAL_NUM
	};

	enum GameState
	{
		WAVE_STATE,
		WAVE_END_STATE,
		TOTAL_STATES
	};

	PauseSelection pauseSelection;
	GameState gameState;
	unsigned int pauseSelectionIterator;

protected:
	std::vector<GameObject *> m_goList;
	float m_worldWidth;
	float m_worldHeight;
	Player * playerInfo;
	short **theMap;
	//GameObject *myInventory[10];
	double doubletime;
	std::vector<alienBase *> alienManager;

	bool isPaused;
	int currentAlien;

	bool hasSound;
	bool KeyBounce[256];
};

#endif