#include "StartMenu.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include <fstream>
#include <iostream>

StartMenu::StartMenu() : currentSelectionState(PLAYGAME),
CurrentSelectionIterator(0), currentSelectionScreen(MAINMENU_SCREEN),
BombPosition(0.f)
{
}

StartMenu::~StartMenu()
{
}

void StartMenu::Init()
{
	SceneBase::Init();

	SceneManager::instance()->State(SceneManager::SCENE_STARTMENU);

	for (size_t i = 0; i < 256; ++i)
	{
		KeyBounce[i] = false;
	}

	hasSound = true;

	ReadSettings();
}

void StartMenu::Update(double dt)
{
	SceneBase::Update(dt);

	if (currentSelectionScreen == MAINMENU_SCREEN)
	{
		if (Application::IsKeyPressed('W'))
		{
			if (!KeyBounce['W'])
			{
				if (CurrentSelectionIterator != 0)
				{
					--CurrentSelectionIterator;
					currentSelectionState = static_cast<CurrentSelection>(CurrentSelectionIterator);
					BombPosition += 22;
				}
			}
			KeyBounce['W'] = true;
		}
		else KeyBounce['W'] = false;

		if (Application::IsKeyPressed('S'))
		{
			if (!KeyBounce['S'])
			{
				if (CurrentSelectionIterator + 1 != TOTAL_NUM)
				{
					++CurrentSelectionIterator;
					currentSelectionState = static_cast<CurrentSelection>(CurrentSelectionIterator);
					BombPosition -= 22;
				}
			}
			KeyBounce['S'] = true;
		}
		else KeyBounce['S'] = false;

		if (Application::IsKeyPressed(VK_RETURN))
		{
			if (!KeyBounce[VK_RETURN])
			{
				switch (currentSelectionState)
				{
				case PLAYGAME:
				{
					SceneManager::instance()->SwitchScene(SceneManager::SCENE_MAINGAME);
					break;
				}
				case SETTINGS:
				{
					currentSelectionScreen = SETTINGS_SCREEN;
					break;
				}
				case EXIT:
				{
					SceneManager::instance()->Quit(true);
					break;
				}
				default:
					break;
				}
			}
			KeyBounce[VK_RETURN] = true;
		}
		else
		{
			KeyBounce[VK_RETURN] = false;
		}
	}
	else if (currentSelectionScreen == SETTINGS_SCREEN)
	{

		if (Application::IsKeyPressed('Q'))
		{
			if (!KeyBounce['Q'])
			{
				std::ifstream settingsFile("Image//settings.txt");

				if (!settingsFile.is_open())
				{
					std::cout << "Missing File.\n";
				}
				else
				{
					settingsFile.close();

					if (hasSound == true)  //Mute sound
					{
						hasSound = false;
						settingsFile.open("Image//settings.txt", std::fstream::out | std::fstream::trunc);
						settingsFile.close();

						std::ofstream settingsFile("Image//settings.txt");
						settingsFile << "Off\n";
					}
					else  //Unmute Sound
					{
						hasSound = true;

						settingsFile.open("Image//settings.txt", std::fstream::out | std::fstream::trunc);
						settingsFile.close();

						std::ofstream settingsFile("Image//settings.txt");
						settingsFile << "On\n";
					}
				}

				settingsFile.close();
			}
			KeyBounce['Q'] = true;
		}
		else
		{
			KeyBounce['Q'] = false;
		}

		if (Application::IsKeyPressed(VK_RETURN))
		{
			if (!KeyBounce[VK_RETURN])
			{
				currentSelectionScreen = MAINMENU_SCREEN;
			}
			KeyBounce[VK_RETURN] = true;
		}
		else
		{
			KeyBounce[VK_RETURN] = false;
		}
	}
}

void StartMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	
	modelStack.PushMatrix();  //Start Menu Background
	{
		modelStack.Translate(100, 50, 0);
		modelStack.Scale(200, 100, 0);
		RenderMesh(meshList[GEO_STARTMENU_BACKGROUND], false);
	}
	modelStack.PopMatrix();

	if (currentSelectionScreen == MAINMENU_SCREEN)
	{
		modelStack.PushMatrix();  //All UI items in start menu
		modelStack.Translate(-5, 0, 0);
		{
			modelStack.PushMatrix();  //Start Menu Title
			{
				modelStack.Translate(95, 78, 0);
				modelStack.Scale(160, 50, 0);
				//modelStack.Rotate(90, 1, 0, 0);
				RenderMesh(meshList[GEO_STARTMENU_TITLE], false);
			}
			modelStack.PopMatrix();

			modelStack.PushMatrix();  //Start Game Button
			{
				modelStack.Translate(95, 66, 1);
				modelStack.Scale(43, 32, 0);
				RenderMesh(meshList[GEO_STARTMENU_STARTGAME], false);
			}
			modelStack.PopMatrix();

			modelStack.PushMatrix();  //Settings Button
			{
				modelStack.Translate(95, 44, 1);
				modelStack.Scale(43, 32, 0);
				RenderMesh(meshList[GEO_STARTMENU_SETTINGS], false);
			}
			modelStack.PopMatrix();

			modelStack.PushMatrix();  //Exit Button
			{
				modelStack.Translate(95, 22, 1);
				modelStack.Scale(43, 32, 0);
				RenderMesh(meshList[GEO_STARTMENU_EXIT], false);
			}
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			{
				modelStack.Translate(70, 59 + BombPosition, 1);
				modelStack.Scale(10, 10, 0);
				RenderMesh(meshList[GEO_NORMALBOMB], false);
			}
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			{
				modelStack.Translate(121, 59 + BombPosition, 1);
				modelStack.Scale(10, 10, 0);
				modelStack.Rotate(180, 0, 1, 0);
				RenderMesh(meshList[GEO_NORMALBOMB], false);
			}
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}
	else if(currentSelectionScreen == SETTINGS_SCREEN)
	{
		modelStack.PushMatrix();  //Sound Symbol
		{
			modelStack.Translate(100, 50, 0);
			modelStack.Scale(50, 50, 1);
			RenderMesh(meshList[GEO_SOUND], false);
		}
		modelStack.PopMatrix();

		if (hasSound == false)
		{
			modelStack.PushMatrix();  //Stop Symbol
			{
				modelStack.Translate(99, 50, 0);
				modelStack.Scale(51, 51, 1);
				RenderMesh(meshList[GEO_STOP], false);
			}
			modelStack.PopMatrix();
		}
	}

	glEnable(GL_DEPTH_TEST);
	SceneManager::instance()->State(SceneManager::SCENE_STARTMENU);
}

void StartMenu::Exit()
{
	SceneBase::Exit();
}

void StartMenu::ReadSettings()
{
	std::ifstream settingsFile("Image//settings.txt");

	if (!settingsFile.is_open())
	{
		std::cout << "Missing File.\n";
	}
	else
	{
		while (!settingsFile.eof())
		{
			std::string soundSetting;

			std::getline(settingsFile, soundSetting, '\n');

			if (soundSetting == "On")
			{
				hasSound = true;
			}
			else if (soundSetting == "Off")
			{
				hasSound = false;
			}
		}
	}

	settingsFile.close();
}
