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

	playerInfo = new Player(100, 35);
	playerInfo->pos.set(1, 1);
	playerInfo->animationPos.SetZero();
	playerInfo->type = GameObject::GO_PLAYER;

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	m_goList.push_back(playerInfo);
	for (size_t i = 0; i < 100; ++i)
	{
		m_goList.push_back(new GameObject());
}
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
		if (!KeyBounce['D'])
			//++playerInfo->pos.x;
			if (playerInfo->move(0, theMap))
			{
			}
		KeyBounce['D'] = true;
	}
	else KeyBounce['D'] = false;

	if (Application::IsKeyPressed('W'))
	{
		if (!KeyBounce['W'])
			//++playerInfo->pos.y;
			if (playerInfo->move(1, theMap))
			{
			}
		KeyBounce['W'] = true;
	}
	else KeyBounce['W'] = false;

	if (Application::IsKeyPressed('A'))
	{
		if (!KeyBounce['A'])
			//--playerInfo->pos.x;
			if (playerInfo->move(2, theMap))
			{
			}
		KeyBounce['A'] = true;
	}
	else KeyBounce['A'] = false;

	if (Application::IsKeyPressed('S'))
	{
		if (!KeyBounce['S'])
			//--playerInfo->pos.y;
			if (playerInfo->move(3, theMap))
			{
			}
		KeyBounce['S'] = true;
	}
	else KeyBounce['S'] = false;

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

	GameObject *lootcrateGO = FetchGO();
	lootcrateGO->active = true;
	lootcrateGO->type = GameObject::GO_LOOTCRATE;
	lootcrateGO->pos.set(5, 5);
}


void SP3::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_PLAYER:
		modelStack.PushMatrix();
		{
			modelStack.Translate(go->pos.x, go->pos.y, 0);
			RenderMesh(meshList[GEO_PLAYER], false);
		}
		modelStack.PopMatrix();
		break;
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
		if (go->scale.x < 1.f && go->scale.y < 1.f)
		{
			go->scale.x += doubletime * 1.1f;
			go->scale.y += doubletime * 1.1f;
		}

		modelStack.PushMatrix();
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x + 1, go->pos.y, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y + 1, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x - 1, go->pos.y, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y - 1, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x + 2, go->pos.y, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y + 2, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x - 2, go->pos.y, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y - 2, 0);
			modelStack.Scale(go->scale.x, go->scale.y, 1);
			RenderMesh(meshList[GEO_BOMBFIRE], false);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();

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
	short DirectionX = playerInfo->pos.x - alien->pos.x, directionY = playerInfo->pos.y - alien->pos.y;

	switch (alien->alienType)
	{
	case alienBase::TYPE1_GRUB:
	{
		modelStack.PushMatrix();
		modelStack.Translate(alien->pos.x += (DirectionX * .05f), alien->pos.y += (directionY * .05f), 0);
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
		modelStack.PushMatrix(); //norrmal bomb
		{
			modelStack.Translate(bomb->pos.x, bomb->pos.y, 0);
			//modelstack.rotate(math::radiantodegree(atan2(-alien->dir.x, alien->dir.y)), 0, 0, 1);
			RenderMesh(meshList[GEO_NORMALBOMB], true);
	}
		modelStack.PopMatrix(); ///normal bomb
	}
	else
	{
		GameObject *bombFireGO = FetchGO();
		bombFireGO->active = true;
		bombFireGO->type = GameObject::GO_BOMBFIRE;
		bombFireGO->pos.set(bomb->pos.x, bomb->pos.y);
		bombFireGO->scale.Set(0.1f, 0.1f, 1);
	
		playerInfo->bombManager.erase(playerInfo->bombManager.begin() + currentBombIndex); //Destroys current bomb object in vector
	}
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

	//map
	modelStack.PushMatrix();
	{
		modelStack.Scale(9, 9, 1);
		modelStack.Translate(.5, .5, 0);

		modelStack.PushMatrix();
		{
			modelStack.Translate(10, -1, 0);
			for (short y = 0; y < 11; ++y)
			{
				modelStack.Translate(-11, 1, 0);
				for (short x = 0; x < 11; ++x)
				{
					modelStack.Translate(1, 0, 0);
					if (theMap[x][y])
						RenderMesh(meshList[wall], false);
					else RenderMesh(meshList[floor], false);
					//if (playerInfo->pos.x == x && playerInfo->pos.y == y)
						//RenderMesh(meshList[GEO_PLAYER], false);
				}
			}
		}
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
			RenderGO(go);
		}
	}
	modelStack.PopMatrix();

	std::ostringstream ss;
	//On screen text

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