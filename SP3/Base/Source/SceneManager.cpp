#include "SceneManager.h"

#include "SP3.h"
#include"StartMenu.h"

SceneManager *SceneManager::s_instance = 0;

SceneManager *SceneManager::instance()
{
	if (!s_instance)
		s_instance = new SceneManager;
	return s_instance;
}

void SceneManager::Init()
{
	scene = new StartMenu();
	nextScene = 0;
	scene->Init();

	state = SCENE_STARTMENU;
	sceneSwitching = false;
}

void SceneManager::Update(double dt)
{
	scene->Update(dt);
	scene->Render();
}

void SceneManager::CleanUp()
{
	scene->Exit();
	delete scene;

	delete s_instance;
}

void SceneManager::Quit(bool in) { quit = in; }
bool SceneManager::Quit() { return quit; }

void SceneManager::SwitchScene(unsigned int sceneID)
{
	if (state == sceneID || sceneSwitching)
		return;

	switch (sceneID)
	{
	case SCENE_STARTMENU: nextScene = new StartMenu(); break;
	case SCENE_MAINGAME: nextScene = new SP3(); break;
	default: return;
	}

	sceneSwitching = true;
}

SceneManager::SCENE_TYPE SceneManager::State() { return state; }
void SceneManager::State(SCENE_TYPE in) { state = in; }