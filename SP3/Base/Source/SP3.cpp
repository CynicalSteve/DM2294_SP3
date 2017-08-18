#include "SP3.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SP3::SP3()
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
	if (Application::IsKeyPressed('A'))
	{
		if (!KeyBounce['A'])
			//--playerInfo->pos.x;
			playerInfo->move(2, 11, 11, theMap);
		KeyBounce['A'] = true;
	}
	else KeyBounce['A'] = false;

	if (Application::IsKeyPressed('D'))
	{
		if (!KeyBounce['D'])
			//++playerInfo->pos.x;
			playerInfo->move(0, 11, 11, theMap);
		KeyBounce['D'] = true;
	}
	else KeyBounce['D'] = false;

	if (Application::IsKeyPressed('W'))
	{
		if (!KeyBounce['W'])
			//++playerInfo->pos.y;
			playerInfo->move(1, 11, 11, theMap);
		KeyBounce['W'] = true;
	}
	else KeyBounce['W'] = false;

	if (Application::IsKeyPressed('S'))
	{
		if (!KeyBounce['S'])
			//--playerInfo->pos.y;
			playerInfo->move(3, 11, 11, theMap);
		KeyBounce['S'] = true;
	}
	else KeyBounce['S'] = false;

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
		GameObject *bombGO = FetchGO();
		bombGO->active = true;
		bombGO->type = GameObject::GO_NORMALBOMB;
		bombGO->pos = playerInfo->pos;
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
	case GameObject::GO_NORMALBOMB:

		if (go->bombTimer < 3.f)
		{
			go->bombTimer += doubletime;
		}
		else
		{
			go->bombTimer = 0.f;
			go->active = false;

			GameObject *bombFireGO = FetchGO();
			bombFireGO->active = true;
			bombFireGO->type = GameObject::GO_BOMBFIRE;
			bombFireGO->pos = go->pos;

			break;
		}

		//exercise 4a: render a sphere with radius 1
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		//modelstack.rotate(math::radiantodegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_NORMALBOMB], true);
		modelStack.PopMatrix();

		break;
	case GameObject::GO_BOMBFIRE:
	{
		if (go->fireTimer < 2.f)
		{
			go->fireTimer += doubletime;
		}
		else
		{
			go->fireTimer = 0.f;
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
					if (playerInfo->pos.x == x && playerInfo->pos.y == y)
						RenderMesh(meshList[GEO_PLAYER], false);
				}
			}
		}
		modelStack.PopMatrix();

		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}
	}
	modelStack.PopMatrix();

	std::ostringstream ss;
	//On screen text

	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid", Color(0, 1, 0), 3, 0, 0);
	
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

	for (short x = 0; x < 11; ++x)
		delete theMap[x];
	delete theMap;
}