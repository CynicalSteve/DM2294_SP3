#include "SP3.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SP3::SP3() : alienManager(NULL),
currentAlien(0)
{
}

SP3::~SP3()
{
}

void SP3::Init()
{
	SceneBase::Init();

	Math::InitRNG();

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (size_t i = 0; i < 100; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
		m_goList[i]->scale.Set(2, 2, 2);
	}

	//Exercise 2c: Construct m_ship, set active, type, scale and pos

	for (size_t i = 0; i < 256; ++i)
	{
		KeyBounce[i] = false;
	}

	playerInfo = new Player(100, 35);
	playerInfo->pos.set(50, 50);
	playerInfo->type = GameObject::GO_PLAYER;

	currentAlien = 0;
}

GameObject* SP3::FetchGO()
{
	//Exercise 3a: Fetch a game object from m_goList and return it
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (size_t i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}
	return m_goList[m_goList.size() - 1];
}

void SP3::Update(double dt)
{
	SceneBase::Update(dt);
	doubletime = dt;

	if (Application::IsKeyPressed('9'))
	{
	}
	if (Application::IsKeyPressed('0'))
	{
	}

	//Exercise 6: set m_force values based on WASD
	if (Application::IsKeyPressed('A'))
	{
		/*Vector3 r(1, -1, 0), F(0, 5, 0);
		m_force += m_ship->dir * 5.f;
		m_torque += r.Cross(F);*/

		playerInfo->pos.x -= playerInfo->getPlayerSpeed() * dt;
	}
	if (Application::IsKeyPressed('D'))
	{
		/*Vector3 r(-1, -1, 0), F(0, 5, 0);
		m_force += m_ship->dir * 5.f;
		m_torque += r.Cross(F);*/
		//playerInfo->pos.x += playerInfo->getPlayerSpeed() * dt;
		playerInfo->pos.x += 1;
	}
	if (Application::IsKeyPressed('W'))
	{
		/*m_force += m_ship->dir * 100.f;*/
		//playerInfo->pos.y += playerInfo->getPlayerSpeed() * dt;
		playerInfo->pos.y += 1;
	}
	if (Application::IsKeyPressed('S'))
	{
		/*m_force -= m_ship->dir * 100.f;*/

		playerInfo->pos.y -= playerInfo->getPlayerSpeed() * dt;
	}

	if (Application::IsKeyPressed('P')) //Pause
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
		playerInfo->bombManager.push_back(new NormalBomb("Normal Bomb", 30, 3, 2, playerInfo->pos.x, playerInfo->pos.y));
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
		
		if (currentAlien < 3)
		{
			alienManager.push_back(new alienGrub("Grub", 100, 100, 100, 50, 50));

			++currentAlien;
		}
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
}


void SP3::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_BOMBFIRE:
	{
		if (go->fireBurnTime < 2.f)
		{
			go->fireBurnTime += doubletime;
		}
		else
		{
			go->fireBurnTime = 0.f;
			go->active = false;

			break;
		}

		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOMBFIRE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x - 5, go->pos.y, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOMBFIRE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x - 10, go->pos.y, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOMBFIRE], true);
		modelStack.PopMatrix();
		
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x + 5, go->pos.y, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOMBFIRE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x + 10, go->pos.y, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOMBFIRE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y - 5, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOMBFIRE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y - 10, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOMBFIRE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y + 5, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOMBFIRE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y + 10, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOMBFIRE], true);
		modelStack.PopMatrix();

		break;
	}
	//case GameObject::GO_ALIENGRUB:
	//{
	//	short DirectionX = playerInfo->pos.x - go->pos.x, directionY = playerInfo->pos.y - go->pos.y;

	//	modelStack.PushMatrix();
	//	modelStack.Translate(go->pos.x += (DirectionX * .05f), go->pos.y += (directionY * .05f), 0);
	//	//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
	//	modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//	RenderMesh(meshList[GEO_ALIENGRUB], true);
	//	modelStack.PopMatrix();
	//}
	}
	
}

void SP3::renderAliens(alienBase *alien)
{
	short DirectionX = playerInfo->pos.x - alien->pos.x, directionY = playerInfo->pos.y - alien->pos.y;

	switch (alien->alienType)
	{
	case alienBase::TYPE1_GRUB:
	{
		modelStack.PushMatrix();
		modelStack.Translate(alien->pos.x += (DirectionX * .05f), alien->pos.y += (directionY * .05f), 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_ALIENGRUB], true);
		modelStack.PopMatrix();
	}
	default:
		break;
	}
}

void SP3::renderBombs(BombBase *bomb, int currentBombIndex)
{
	if (bomb->bombTimer < bomb->getTimeToExplode())
	{
		bomb->bombTimer += doubletime;
	}
	else
	{
		GameObject *bombFireGO = FetchGO();
		bombFireGO->active = true;
		bombFireGO->type = GameObject::GO_BOMBFIRE;
		bombFireGO->pos.set(bomb->pos.x, bomb->pos.y);
		bombFireGO->scale.Set(10, 10, 10);

		playerInfo->bombManager.erase(playerInfo->bombManager.begin() + currentBombIndex); //Destroys current bomb object in vector
		return;
	}
	modelStack.PushMatrix();
	modelStack.Translate(bomb->pos.x, bomb->pos.y, 0);
	//modelstack.rotate(math::radiantodegree(atan2(-alien->dir.x, alien->dir.y)), 0, 0, 1);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_NORMALBOMB], true);
	modelStack.PopMatrix();
}

void SP3::Render()
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

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(385, 200, 1);
	RenderMesh(meshList[GEO_GROUND], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_MAZEWALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(15, 40, 0);
	modelStack.Scale(50, 50, 0);
	RenderMesh(meshList[GEO_HOUSE], false);
	modelStack.PopMatrix();

	//Render Aliens
	if (!alienManager.empty())  //Checks if alien manager vector is empty
	{
		for (int currentAlien = 0; currentAlien < alienManager.size(); ++currentAlien)
		{
			renderAliens(alienManager[currentAlien]);
		}
	}

	//Render Bombs
	if (!playerInfo->bombManager.empty())  //Checks if bomb manager vector is empty
	{
		for (int currentBomb = 0; currentBomb < playerInfo->bombManager.size(); ++currentBomb)
		{
			renderBombs(playerInfo->bombManager[currentBomb], currentBomb);
		}
	}
	
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(playerInfo->pos.x, playerInfo->pos.y, 0);
	modelStack.Scale(10, 10, 1);
	RenderMesh(meshList[GEO_PLAYER], false);
	modelStack.PopMatrix();

	std::ostringstream ss;
	std::ostringstream ss1;
	//On screen text

	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss1.str("");
	ss1.precision(5);
	ss1 << "Player - X: " << playerInfo->pos.x << " Y:" <<playerInfo->pos.y;
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 0, 0);
	
	glEnable(GL_DEPTH_TEST);
}

void SP3::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects

	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}

	if (!alienManager.empty())
	{
		for (int currentAlien = 0; currentAlien < alienManager.size(); ++currentAlien)
		{
			delete alienManager[currentAlien];
		}
	}

	if (!playerInfo->bombManager.empty())
	{
		for (int currentBomb = 0; currentBomb < playerInfo->bombManager.size(); ++currentBomb)
		{
			delete playerInfo->bombManager[currentBomb];
		}
	}

	delete playerInfo;
}