#ifndef StartMenu_H
#define StartMenu_H

#include <vector>
#include "SceneBase.h"
#include "SceneManager.h"


class StartMenu : public SceneBase
{
	enum CurrentSelection
	{
		PLAYGAME,
		SETTINGS,
		EXIT,
		TOTAL_NUM
	};

	enum selectionScreen
	{
		MAINMENU_SCREEN,
		SETTINGS_SCREEN,
		TOTALNUM
	};

public:
	StartMenu();
	~StartMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	void ReadSettings();

	CurrentSelection currentSelectionState;
	selectionScreen currentSelectionScreen;
	unsigned int CurrentSelectionIterator;

private:
	float BombPosition;

	float m_worldHeight;
	float m_worldWidth;

	bool hasSound;
	bool KeyBounce[256];
};

#endif