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
	meshList[floor] = MeshBuilder::GenerateQuad("floor", Color(.2, .6, .2), 1);
	meshList[wall] = MeshBuilder::GenerateQuad("wall", Color(.6, .3, .3), 1);

	Math::InitRNG();

	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	for (size_t i = 0; i < 256; ++i)
	{
		KeyBounce[i] = false;
	}

	theMap = new short *[11];
	for (short x = 0; x < 11; ++x)
		theMap[x] = new short[11];

	for (short y = 0; y < 11; ++y)
		for (short x = 0; x < 11; ++x)
			theMap[x][y] = 0;

	for (short y = 0; y < 11; ++y)
	{
		for (short x = 0; x < 11; ++x)
		{
			if (y == 0 || y == 10)
				theMap[x][y] = 1;
			else if (y % 2)
			{
				theMap[0][y] = 1;
				theMap[10][y] = 1;
			}
			else if (!(x % 2))
				theMap[x][y] = 1;
		}
	}

	playerInfo = new Player(100, 0.2f);
	playerInfo->pos.set(1, 1);
	playerInfo->animationPos.Set(playerInfo->pos.x, playerInfo->pos.y, -1);
	playerInfo->type = GameObject::GO_PLAYER;

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	m_goList.push_back(playerInfo);
	for (size_t i = 0; i < 100; ++i)
	{
		m_goList.push_back(new GameObject());
	}

	GameObject *lootcrateGO = FetchGO();
	lootcrateGO->type = GameObject::GO_LOOTCRATE;
	lootcrateGO->pos.set(5, 5);
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
		m_goList.push_back(new GameObject());
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
	if (Application::IsKeyPressed('D'))
	{
		if (playerInfo->move(-1, theMap))
			//++playerInfo->pos.x;
			if (playerInfo->move(0, theMap))
				playerInfo->animationPos.z = dt / playerInfo->getPlayerSpeed();
		KeyBounce['D'] = true;
	}
	else KeyBounce['D'] = false;

	if (Application::IsKeyPressed('W'))
	{
		if (playerInfo->move(-1, theMap))
		{
			//++playerInfo->pos.y;
			if (playerInfo->move(1, theMap))
				playerInfo->animationPos.z = 1 + dt / playerInfo->getPlayerSpeed();
		}
		KeyBounce['W'] = true;
	}
	else KeyBounce['W'] = false;

	if (Application::IsKeyPressed('A'))
	{
		if (playerInfo->move(-1, theMap))
		{
			//--playerInfo->pos.x;
			if (playerInfo->move(2, theMap))
			{
				playerInfo->animationPos.z = 2 + dt / playerInfo->getPlayerSpeed();
			}
		}
	}
	if (!alienManager.empty())
	{
		for (unsigned int i = 0; i < alienManager.size(); ++i)
		{
			if (alienManager[i]->move(-1, theMap))
			{
				if (alienManager[i]->move(2, theMap))
				{
					alienManager[i]->animationPos.z = 2 + dt / alienManager[i]->getAlienSpeed();
				}
			}
			alienManager[i]->move(4, theMap);
		}
	}

	if (Application::IsKeyPressed('S'))
	{
		if (playerInfo->move(-1, theMap))
			//--playerInfo->pos.y;
			if (playerInfo->move(3, theMap))
				playerInfo->animationPos.z = 3 + dt / playerInfo->getPlayerSpeed();
		KeyBounce['S'] = true;
	}
	else KeyBounce['S'] = false;

	playerInfo->move(4, theMap);

	if (Application::IsKeyPressed('C')) //Lower player health
	{
		if (!KeyBounce['C'])
		{
			playerInfo->setPlayerHealth(playerInfo->getPlayerHealth() - 10);
		}

		KeyBounce['C'] = true;
	}
	else KeyBounce['C'] = false;
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
			alienManager.push_back(new alienGrub("Grub", 100, 0.7f, 5, 5, 9, 1));

			++currentAlien;
		}
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	for (short it = 0; it < playerInfo->bombManager.size(); ++it)
	{
		BombBase *go = playerInfo->bombManager[it];

		if (go->bombTimer < go->getTimeToExplode())
		{
			go->bombTimer += doubletime;
			continue;
		}

		GameObject *bombFireGO = FetchGO();
		bombFireGO->type = GameObject::GO_BOMBFIRE;
		bombFireGO->pos.set(go->pos.x, go->pos.y);
		bombFireGO->scale.Set(0.1, 0.1, 1);
		short i = 1;

		for (; i < 3 && !theMap[go->pos.x + i][go->pos.y]; ++i)
		{
			bombFireGO = FetchGO();
			bombFireGO->type = GameObject::GO_BOMBFIRE;
			bombFireGO->pos.set(go->pos.x + i, go->pos.y);
			bombFireGO->scale.Set(0.1, 0.1, 1);
		}

		for (i = 1; i < 3 && !theMap[go->pos.x][go->pos.y + i]; ++i)
		{
			bombFireGO = FetchGO();
			bombFireGO->type = GameObject::GO_BOMBFIRE;
			bombFireGO->pos.set(go->pos.x, go->pos.y + i);
			bombFireGO->scale.Set(0.1, 0.1, 1);
		}

		for (i = 1; i < 3 && !theMap[go->pos.x - i][go->pos.y]; ++i)
		{
			bombFireGO = FetchGO();
			bombFireGO->type = GameObject::GO_BOMBFIRE;
			bombFireGO->pos.set(go->pos.x - i, go->pos.y);
			bombFireGO->scale.Set(0.1, 0.1, 1);
		}

		for (i = 1; i < 3 && !theMap[go->pos.x][go->pos.y - i]; ++i)
		{
			bombFireGO = FetchGO();
			bombFireGO->type = GameObject::GO_BOMBFIRE;
			bombFireGO->pos.set(go->pos.x, go->pos.y - i);
			bombFireGO->scale.Set(0.1, 0.1, 1);
		}

		playerInfo->bombManager.erase(playerInfo->bombManager.begin() + it); //Destroys current bomb object in vector
	}

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
			continue;

		if (go->type == GameObject::GO_BOMBFIRE)
		{
			if (go->fireBurnTime < 2.f)
			{
				go->fireBurnTime += doubletime;
			}
			else
			{
				go->fireBurnTime = 0.f;
				go->active = false;
				continue;
			}

			if (go->scale.x < 1.f && go->scale.y < 1.f)
			{
				go->scale.x += doubletime * 1.1f;
				go->scale.y += doubletime * 1.1f;
			}
		}
	}
}


void SP3::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_PLAYER:
		modelStack.PushMatrix();
		{
			modelStack.Translate(go->animationPos.x, go->animationPos.y, 0);
			RenderMesh(meshList[GEO_PLAYER], false);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOMBFIRE:
	{
		modelStack.PushMatrix(); //bombfire
		{
			modelStack.Translate(go->pos.x, go->pos.y, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
		}
		modelStack.PopMatrix(); ///bombfire

		//Checks if anything is in the bombfire
		for (unsigned int i = 0; i < m_goList.size(); ++i)
		{
			if (m_goList[i]->type != GameObject::GO_PLAYER && m_goList[i]->type != GameObject::GO_BOMBFIRE && m_goList[i]->type != GameObject::GO_NORMALBOMB)
			{
				if (go->pos.x == m_goList[i]->pos.x && go->pos.y == m_goList[i]->pos.y)
				{
					m_goList[i]->active = false;
				}
			}
		}

		break;
	}
	case GameObject::GO_LOOTCRATE:
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(go->pos.x, go->pos.y, 0);
			RenderMesh(meshList[GEO_LOOTCRATE], false);
		}
		modelStack.PopMatrix();

		break;
	}
	default:
		break;
	}
	
	
}

void SP3::renderAliens(alienBase *alien)
{
	switch (alien->alienType)
	{
	case alienBase::TYPE1_GRUB:
	{
		modelStack.PushMatrix(); //AlienGrub
		{
			modelStack.Translate(alien->animationPos.x, alien->animationPos.y, 0);
			RenderMesh(meshList[GEO_ALIENGRUB], true);
		}
		modelStack.PopMatrix(); ///AlienGrub
	}
		break;
	default:
		break;
	}
}

void SP3::renderBombs(BombBase *bomb, int currentBombIndex)
{

	modelStack.PushMatrix(); //norrmal bomb
	{
		modelStack.Translate(bomb->pos.x, bomb->pos.y, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-alien->dir.x, alien->dir.y)), 0, 0, 1);
		RenderMesh(meshList[GEO_NORMALBOMB], true);
	}
	modelStack.PopMatrix(); ///normal bomb
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
	//RenderMesh(meshList[GEO_HOUSE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //grid system
	{
		modelStack.Scale(9, 9, 1);
		modelStack.Translate(.5, .5, 0);

		modelStack.PushMatrix(); //map
		{
			for (short y = 0; y < 11; ++y)
			{
				for (short x = 0; x < 11; ++x)
				{
					if (theMap[x][y])
						RenderMesh(meshList[wall], false);
					else RenderMesh(meshList[floor], false);
					modelStack.Translate(1, 0, 0);
					//if (playerInfo->pos.x == x && playerInfo->pos.y == y)
						//RenderMesh(meshList[GEO_PLAYER], false);
				}
				modelStack.Translate(-11, 1, 0);
			}
		}
		modelStack.PopMatrix(); ///map

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
				RenderGO(go);
		}
	}
	modelStack.PopMatrix(); ///grid system

	std::ostringstream ss;
	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "EP: " << playerInfo->getEquipmentCurrency();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.str("");
	ss.precision(5);
	ss << "Health: " << playerInfo->getPlayerHealth();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 40, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str("");
	ss.precision(5);
	ss << "Player - X: " << playerInfo->pos.x << " Y:" <<playerInfo->pos.y;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);
	
	glEnable(GL_DEPTH_TEST);
}

void SP3::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
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
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	for (short x = 0; x < 11; ++x)
		delete theMap[x];
	delete theMap;
}