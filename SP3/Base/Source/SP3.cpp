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

	playerInfo = new Player(100, 1);
	playerInfo->pos.set(5, 5);
	playerInfo->type = GameObject::GO_PLAYER;
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



	if (Application::IsKeyPressed('A'))
	{
		if (!KeyBounce['A'])
		{
			--playerInfo->pos.x;
		}
		KeyBounce['A'] = true;
	}
	else
	{
		KeyBounce['A'] = false;
	}

	if (Application::IsKeyPressed('D'))
	{
		if (!KeyBounce['D'])
		{
			++playerInfo->pos.x;
		}
		KeyBounce['D'] = true;
	}
	else
	{
		KeyBounce['D'] = false;
	}

	if (Application::IsKeyPressed('W'))
	{
		if (!KeyBounce['W'])
		{
			++playerInfo->pos.y;
		}
		KeyBounce['W'] = true;
	}
	else
	{
		KeyBounce['W'] = false;
	}

	if (Application::IsKeyPressed('S'))
	{
		if (!KeyBounce['S'])
		{
			--playerInfo->pos.y;
		}
		KeyBounce['S'] = true;
	}
	else
	{
		KeyBounce['S'] = false;
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
		GameObject *bombGO = FetchGO();
		bombGO->active = true;
		bombGO->type = GameObject::GO_NORMALBOMB;
		bombGO->pos = playerInfo->pos;
		bombGO->scale.Set(10, 10, 10);
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
	case GameObject::GO_NORMALBOMB:
		
		if (go->bombTimer < 3.f)
		{
			go->bombTimer += doubletime;
		}
		else
		{
			go->bombTimer = 0.f;
			go->active = false;
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
	modelStack.Scale(270, 200, 1);
	RenderMesh(meshList[GEO_GROUND], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_MAZEWALL], false);
	modelStack.PopMatrix();

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

	delete playerInfo;
}