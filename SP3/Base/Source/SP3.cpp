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

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (size_t i = 0; i < 100; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
		m_goList[i]->scale.Set(2, 2, 2);
	}

	//Exercise 2b: Initialize m_lives and m_score
	m_lives = 3;
	m_score = 0;

	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	m_ship = new GameObject(GameObject::GO_SHIP);
	m_ship->scale.Set(1, 1, 1);
	m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0.f);
	m_ship->active = true;
	m_ship->angularVelocity = 0.f;
	m_ship->momentOfInertia = 1.f;

	for (size_t i = 0; i < 256; ++i)
	{
		KeyBounce[i] = false;
	}

	/*meshList[ship] = MeshBuilder::GenerateQuad("ship", Color(), 5);
	meshList[ship]->textureID = LoadTGA("Image//cheekibreeki.tga");*/
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

	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}
	m_force.SetZero();
	m_torque.SetZero();

	//Exercise 6: set m_force values based on WASD
	if (Application::IsKeyPressed('A'))
	{
		Vector3 r(1, -1, 0), F(0, 5, 0);
		m_force += m_ship->dir * 5.f;
		m_torque += r.Cross(F);
	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 r(-1, -1, 0), F(0, 5, 0);
		m_force += m_ship->dir * 5.f;
		m_torque += r.Cross(F);
	}
	if (Application::IsKeyPressed('W'))
	{
		m_force += m_ship->dir * 100.f;
	}
	if (Application::IsKeyPressed('S'))
	{
		m_force -= m_ship->dir * 100.f;
	}

	//Exercise 8: use 2 keys to increase and decrease mass of ship
	if (Application::IsKeyPressed(VK_UP))
	{
		m_ship->mass += 0.01;
	}
	if (Application::IsKeyPressed(VK_DOWN) && m_ship->mass > 0.02)
	{
		m_ship->mass -= 0.01;
	}

	//Exercise 11: use a key to spawn some asteroids
	if (Application::IsKeyPressed('V'))
	{
		GameObject *go = FetchGO();
		go->type = GameObject::GO_ASTEROID;
		go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 0);
		go->vel.Set(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), 0);
		go->scale.Set(1, 1, 1);
	}

	//Exercise 14: use a key to spawn a bullet
	//Exercise 15: limit the spawn rate of bullets
	if (Application::IsKeyPressed(VK_SPACE))
	{
		if (!KeyBounce[VK_SPACE])
		{
			GameObject* go = FetchGO();
			go->active = true;
			go->type = GameObject::GO_BULLET;
			go->pos = m_ship->pos;
			go->vel = m_ship->dir * BULLET_SPEED;
			go->scale.Set(0.2, 0.2, 0.2);
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

	//Physics Simulation Section

	//Exercise 7: Update ship's velocity based on m_force
	if (m_ship->active)
	{
		m_ship->vel += m_force * (dt / m_ship->mass) * m_speed;
		m_ship->pos += m_ship->vel * (dt / m_ship->mass) * m_speed;
		if (m_ship->vel.IsZero())
			m_ship->dir.Set(1, 0, 0);


		float angularAcceleration = m_torque.z * (1.f / m_ship->momentOfInertia);
		m_ship->angularVelocity += angularAcceleration * dt * m_speed;
		m_ship->dir += Vector3(cos(m_ship->angularVelocity), sin(m_ship->angularVelocity), 0) * dt * m_speed;

		float theta = atan2(m_ship->dir.y, m_ship->dir.x);
		theta += m_ship->angularVelocity * dt * m_speed;
		m_ship->dir.Set(cos(theta), sin(theta), 0);
	}

	//Exercise 10
	if (m_ship->vel.Length() > MAX_SPEED)
	{
		m_ship->vel.Normalize();
		m_ship->vel *= MAX_SPEED;
	}

	//Exercise 9: wrap ship position if it leaves screen
	if (m_ship->pos.x > m_worldWidth)
	{
		m_ship->pos.x -= m_worldWidth;
	}

	else if (m_ship->pos.x < 0)
	{
		m_ship->pos.x += m_worldWidth;
	}

	if (m_ship->pos.y > m_worldHeight)
	{
		m_ship->pos.y -= m_worldHeight;
	}

	else if (m_ship->pos.y < 0)
	{
		m_ship->pos.y += m_worldHeight;
	}

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			go->pos += go->vel * (dt / go->mass);

			//Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check
			if (go->type == GameObject::GO_ASTEROID)
			{
				Vector3 dist = go->pos - m_ship->pos;
				float cScale = go->scale.x + m_ship->scale.x;
				if (dist.LengthSquared() < cScale * cScale)
				{
					go->active = false;
					m_lives -= 1;
					m_ship->pos.Set(m_worldWidth*0.5, m_worldHeight*0.5, 0);
					m_ship->vel.SetZero();
				}
			} //wrap around collision?

			//Exercise 13: asteroids should wrap around the screen like the ship
			if (go->type == GameObject::GO_ASTEROID)
			{
				if (go->pos.x > m_worldWidth)
				{
					go->pos.x -= m_worldWidth;
				}

				else if (go->pos.x < 0)
				{
					go->pos.x += m_worldWidth;
				}

				if (go->pos.y > m_worldHeight)
				{
					go->pos.y -= m_worldHeight;
				}

				else if (go->pos.y < 0)
				{
					go->pos.y += m_worldHeight;
				}
			}

			//Exercise 16: unspawn bullets when they leave screen
			if (go->type == GameObject::GO_BULLET)
			{
				if (go->pos.x - go->scale.x < 0 || go->pos.x + go->scale.x > m_worldWidth || go->pos.y - go->scale.y < 0 || go->pos.y + go->scale.y > m_worldHeight)
				{
					go->active = false;
				}

				//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
				for (auto go2 : m_goList)
				{
					if (go2->type == GameObject::GO_ASTEROID && go2->active)
					{
						if ((go->pos - go2->pos).LengthSquared() < (go->scale.x + go2->scale.x) * (go->scale.x + go2->scale.x))
						{
							go->active = false;
							go2->active = false;
							m_score += 2;
						}
					}
				}
			}
		}
	}
}


void SP3::RenderGO(GameObject *go)
{
	//switch (go->type)
	//{
	//case GameObject::GO_SHIP:
	//	//Exercise 4a: render a sphere with radius 1
	//	modelStack.PushMatrix();
	//	modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
	//	modelStack.Rotate(Math::RadianToDegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
	//	modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//	RenderMesh(meshList[ship], true);
	//	modelStack.PopMatrix();

	//	if (go->pos.x + go->scale.x > m_worldWidth)
	//	{
	//		modelStack.PushMatrix();
	//		modelStack.Translate(go->pos.x - m_worldWidth, go->pos.y, go->pos.z);
	//		modelStack.Rotate(Math::RadianToDegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
	//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//		RenderMesh(meshList[ship], true);
	//		modelStack.PopMatrix();
	//	}
	//	else if (go->pos.x - go->scale.x < 0)
	//	{
	//		modelStack.PushMatrix();
	//		modelStack.Translate(go->pos.x + m_worldWidth, go->pos.y, go->pos.z);
	//		modelStack.Rotate(Math::RadianToDegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
	//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//		RenderMesh(meshList[ship], true);
	//		modelStack.PopMatrix();
	//	}
	//	if (go->pos.y + go->scale.y > m_worldHeight)
	//	{
	//		modelStack.PushMatrix();
	//		modelStack.Translate(go->pos.x, go->pos.y - m_worldHeight, go->pos.z);
	//		modelStack.Rotate(Math::RadianToDegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
	//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//		RenderMesh(meshList[ship], true);
	//		modelStack.PopMatrix();
	//	}
	//	else if (go->pos.y - go->scale.x < 0)
	//	{
	//		modelStack.PushMatrix();
	//		modelStack.Translate(go->pos.x, go->pos.y + m_worldHeight, go->pos.z);
	//		modelStack.Rotate(Math::RadianToDegree(atan2(-go->dir.x, go->dir.y)), 0, 0, 1);
	//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//		RenderMesh(meshList[ship], true);
	//		modelStack.PopMatrix();
	//	}

	//	//Exercise 17a: render a ship texture or 3D ship model
	//	//Exercise 17b:	re-orientate the ship with velocity
	//	break;
	//case GameObject::GO_ASTEROID:
	//	//Exercise 4b: render a cube with length 2
	//	modelStack.PushMatrix();
	//	modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
	//	modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//	RenderMesh(meshList[GEO_CUBE], true);
	//	modelStack.PopMatrix();

	//	if (go->pos.x + go->scale.x > m_worldWidth)
	//	{
	//		modelStack.PushMatrix();
	//		modelStack.Translate(go->pos.x - m_worldWidth, go->pos.y, go->pos.z);
	//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//		RenderMesh(meshList[GEO_CUBE], true);
	//		modelStack.PopMatrix();
	//	}
	//	else if (go->pos.x - go->scale.x < 0)
	//	{
	//		modelStack.PushMatrix();
	//		modelStack.Translate(go->pos.x + m_worldWidth, go->pos.y, go->pos.z);
	//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//		RenderMesh(meshList[GEO_CUBE], true);
	//		modelStack.PopMatrix();
	//	}
	//	if (go->pos.y + go->scale.y > m_worldHeight)
	//	{
	//		modelStack.PushMatrix();
	//		modelStack.Translate(go->pos.x, go->pos.y - m_worldHeight, go->pos.z);
	//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//		RenderMesh(meshList[GEO_CUBE], true);
	//		modelStack.PopMatrix();
	//	}
	//	else if (go->pos.y - go->scale.x < 0)
	//	{
	//		modelStack.PushMatrix();
	//		modelStack.Translate(go->pos.x, go->pos.y + m_worldHeight, go->pos.z);
	//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//		RenderMesh(meshList[GEO_CUBE], true);
	//		modelStack.PopMatrix();
	//	}

	//	break;
	//case GameObject::GO_BULLET:
	//	modelStack.PushMatrix();
	//	modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
	//	modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//	RenderMesh(meshList[GEO_BALL], true);
	//	modelStack.PopMatrix();
	//	break;
	//}
modelStack.PushMatrix();
modelStack.Translate(0, 0, 0);
modelStack.Scale(10, 10, 10);
RenderMesh(meshList[GEO_MAZEWALL], false);
modelStack.PopMatrix();

    modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(270, 200, 1);
	RenderMesh(meshList[GEO_GROUND], false);
	modelStack.PopMatrix();

	
}

void SP3::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	RenderMesh(meshList[GEO_AXES], false);

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	if (m_ship->active)
	{
		RenderGO(m_ship);
	}

	std::ostringstream ss;
	//On screen text

	if (m_ship->active)
	{
		//Exercise 5a: Render m_lives, m_score
		ss << "Lives: " << m_lives << "Score: " << m_score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 18);

		//Exercise 5b: Render position, velocity & mass of ship
		ss.str("");
		ss << "Pos: " << m_ship->pos;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

		ss.str("");
		ss << "Vel: " << m_ship->vel;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);

		ss.str("");
		ss << "Mass: " << m_ship->mass;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);
	}

	ss.str("");
	ss.precision(3);
	ss << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid", Color(0, 1, 0), 3, 0, 0);
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
	if (m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}