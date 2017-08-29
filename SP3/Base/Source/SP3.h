#ifndef SP3_H
#define SP3_H

#include "GameObject.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "alienGrub.h"
#include "alienGhoul.h"
#include "alienRaptor.h"
#include "alienGoliath.h"
#include "alienLeviathan.h"
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
	void RenderPauseUI();
	void RenderLoseUI();

	void ReadSettings();
	void renderShopScreen();

	void AlienMovement(double dt);
	void PlayerChecks(double dt);
	void BombFireCreation(double dt);
	void m_goListInteractions(double dt);
	void spawnAliens(double dt);

	GameObject * FetchGO();

	enum PauseSelection
	{
		CONTINUE,
		SETTINGS,
		EXIT_MAINMENU,
		EXIT_GAME,
		TOTAL_NUM
	};

	enum ShopSelection
	{
		NORMALBOMB,
		MINEBOMB,
		NUKEBOMB,
		TOTAL_SHOP
	};

	enum LoseSelection
	{
		RESTART,
		LOSE_EXIT_MAINMENU,
		LOSE_EXIT_GAME,
		TOTAL_LOSE
	};
	enum GameState
	{
		WAVE_STATE,
		WAVE_END_STATE,
		LOSE_STATE,
		TOTAL_STATES
	};

	PauseSelection pauseSelection;
	ShopSelection shopselection;
	LoseSelection loseSelection;
	GameState gameState;
	unsigned int pauseSelectionIterator;
	unsigned int shopSelectionIterator;
	unsigned int loseSelectionIterator;

	float loseBombPosition;

protected:
	std::vector<GameObject *> m_goList;
	Player * playerInfo;
	short **theMap;
	short mapSize;
	std::vector<GameObject::coord> spawnPosition;
	double doubletime;
	std::vector<alienBase *> alienManager;

	bool isPaused;
	unsigned long int dayNumber;

	bool hasSound;
	bool KeyBounce[256];
};

#endif