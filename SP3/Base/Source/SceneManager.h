#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"

class SceneManager
{
public:
	static enum SCENE_TYPE
	{
		SCENE_STARTMENU,
		SCENE_MAINGAME
	};

	static SceneManager* instance();

	void Init();
	void Update(double dt);
	void CleanUp();
	void Quit(bool in);
	bool Quit();

	void SwitchScene(unsigned int sceneID = 0);
	SCENE_TYPE State();
	void State(SCENE_TYPE in);

private:
	static SceneManager* s_instance;
	Scene* scene;
	Scene* nextScene;
	bool sceneSwitching;
	SCENE_TYPE state;
	bool quit;

	SceneManager() : quit(false) {}
	SceneManager(SceneManager const&);
	void operator=(SceneManager const&);
};

#endif