#include "StartMenu.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

StartMenu::StartMenu()
{
}

StartMenu::~StartMenu()
{
}

void StartMenu::Init()
{
	SceneBase::Init();

	for (size_t i = 0; i < 256; ++i)
	{
		KeyBounce[i] = false;
	}
}

void StartMenu::Update(double dt)
{
	SceneBase::Update(dt);

	//Exercise 6: set m_force values based on WASD
	if (Application::IsKeyPressed('A'))
	{
	}
	if (Application::IsKeyPressed('D'))
	{
	}
	if (Application::IsKeyPressed('W'))
	{
	}
	if (Application::IsKeyPressed('S'))
	{
	}

	//Exercise 8: use 2 keys to increase and decrease mass of ship
	if (Application::IsKeyPressed(VK_UP))
	{
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
	}

	if (Application::IsKeyPressed(VK_SPACE))
	{
		if (!KeyBounce[VK_SPACE])
		{
		}
		KeyBounce[VK_SPACE] = true;
	}
	else
	{
		KeyBounce[VK_SPACE] = false;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
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
	
	modelStack.PushMatrix();  //All UI items in start menu
	{
		modelStack.PushMatrix();  //Start Menu Title
		{
			modelStack.Translate(100, 50, 0);
			modelStack.Scale(50, 50, 0);
			RenderMesh(meshList[GEO_STARTMENU_TITLE], false);
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();  //Start Menu Background
		{
			modelStack.Translate(100, 50, 0);
			modelStack.Scale(200, 100, 0);
			RenderMesh(meshList[GEO_STARTMENU_BACKGROUND], false);
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();  //Start Menu Title
		{
			modelStack.Translate(87, 78, 0);
			modelStack.Scale(160, 50, 0);
			//modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_STARTMENU_TITLE], false);
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
	
	std::ostringstream ss;
	std::ostringstream ss1;
	//On screen text

	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	/*ss1.str("");
	ss1.precision(5);
	ss1 << "Player - X: " << playerInfo->pos.x << " Y:" <<playerInfo->pos.y;
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 0, 0);*/
	
	glEnable(GL_DEPTH_TEST);
}

void StartMenu::Exit()
{
	SceneBase::Exit();
}