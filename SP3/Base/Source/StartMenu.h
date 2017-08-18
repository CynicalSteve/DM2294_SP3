#ifndef StartMenu_H
#define StartMenu_H

#include <vector>
#include "SceneBase.h"
#include "SceneManager.h"


class StartMenu : public SceneBase
{

public:
	StartMenu();
	~StartMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	float m_worldWidth;
	float m_worldHeight;
	

	bool KeyBounce[256];
};

#endif