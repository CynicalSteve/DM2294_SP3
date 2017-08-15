#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;
	m_ghost = new GameObject(GameObject::GO_BALL);
	 
	m_timerStarted = false;

	 
	Vector3 centre = (50, 50, 0);
	for (int i = 0; i < 8; ++i)
	{
		GameObject *go = FetchGO();
		go->type = GameObject::GO_WALL;
		go->dir.Set(cos(Math::DegreeToRadian(i *45)),sin(Math::DegreeToRadian(i*45)), 0);
		go->pos = centre + go->dir * 40;
		go->scale.Set(2, 20, 1);
	}
 
}

GameObject* SceneCollision::FetchGO()
{
	//Exercise 2a: implement FetchGO()
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject *)* it;

		if (!go->active)
		{
			go->active = true;
			++m_objectCount;
			return go;
		}
		break;
	}

	for (unsigned i = 0; i < 10; i++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}
	++m_objectCount;
	GameObject *go =  m_goList.back();
	go->active = true;
	return go;
	//Exercise 2b: increase object count every time an object is set to active
		 
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);
	static bool SpaceState = false;
	if (!SpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		SpaceState = true;
		GameObject* go = FetchGO();
		go->active = true;
		go->type = GameObject::GO_BALL;
		go->pos = (m_worldWidth * 0.9f, 10, 0);
		go->vel = (0, 10, 0);
		//float scale = dist / 10;
		go->scale.Set(5, 5, 1);
		go->mass = 2.5f * 2.5f * 2.5f;
	}
	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		if (!m_ghost->active)
		{
			m_ghost->active = true;
			m_ghost->pos.Set(x / w * m_worldWidth, (h - y) / h * m_worldHeight, 0);
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		GameObject* go = FetchGO();

		go->type = GameObject::GO_BALL;
		go->pos = m_ghost->pos;
		go->vel = m_ghost->pos - Vector3(x / w * m_worldWidth, m_worldHeight);
		//float scale = dist / 10;
		go->scale.Set(2, 2, 2);
		go->mass = 1;
		m_ghost->active = false;

		m_timeTaken1 = 0;
		m_timerStarted = true;
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

		//Exercise 10: spawn large GO_BALL
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		if (!m_ghost->active)
		{
			m_ghost->active = true;
			m_ghost->pos.Set(x / w * m_worldWidth, (h - y) / h * m_worldHeight, 0);
		}
		GameObject* go = FetchGO();

		go->type = GameObject::GO_BALL;
		go->pos = m_ghost->pos;
		go->vel = m_ghost->pos - Vector3(x / w * m_worldWidth, m_worldHeight);
		go->scale.Set(3, 3, 3);
		go->mass = 1.5f * 1.5f * 1.5f;
		m_ghost->active = false;
	}
	/*GameObject* go = FetchGO();
	go->type = GameObject::GO_PILLAR;
	go->pos =*/
	//Physics Simulation Section
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->type == GameObject::GO_BALL)
				go->pos += go->vel * dt * m_speed;
			//Exercise 7: handle out of bound game objects
			if (go->pos.x < 0 + go->scale.x && go->vel.x < 0)
			{
				go->vel.x = -go->vel.x;
			}
			else if (go->pos.x > m_worldWidth - go->scale.x && go->vel.x > 0)
			{
				go->vel.x = -go->vel.x;
			}
			if (go->pos.y < 0 + go->scale.y && go->vel.y < 0)
			{
				go->vel.y = -go->vel.y;
			}
			else if (go->pos.y > m_worldHeight - go->scale.y && go->vel.y > 0)
			{
				go->vel.y = -go->vel.y;
			}
			if (go->pos.x < 0 || go->pos.x > m_worldWidth || go->pos.y < -10 || go->pos.y > m_worldHeight)
				go->active = false;

			//Exercise 8a: handle collision between GO_BALL and GO_BALL using velocity swap

			for (std::vector<GameObject*>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject *go2 = (GameObject *)*it2;

				if (!go2->active)
					continue;
				if (go2->type != GameObject::GO_BALL)
					continue;
				if (go->type == GameObject::GO_WALL && go2->type == GameObject::GO_WALL)
					continue;
				GameObject *goA, *goB;
				if (go->type == GameObject::GO_BALL)
				{
					goA = go;
					goB = go2;
				}
				else
				{
					goA = go2;
					goB = go;
				}
				//if(CheckCollision)
				float distsq = (go2->pos - go->pos).LengthSquared();
				float combinedRadiusSq = (go->scale.x + go2->scale.x) * (go->scale.x + go2->scale.x);
				if (distsq < combinedRadiusSq)
				{
					m1 = go->mass;
					m2 = go2->mass;
					u1 = go->vel;
					u2 = go2->vel;


					/*Vector3 temp = go->vel;
					go->vel = go2->vel;
					go2->vel = temp;*/

					//Vector3 momentum1 = go->mass * go->vel;
					//Vector3 momentum2 = go2->mass * go2->vel;

					//go->vel = momentum2 * (1.f / go->mass);
					//go2->vel = momentum1 * (1.f / go2->mass);
					Vector3 N = (go2->pos - go->pos).Normalize();
					Vector3 u1N = u1.Dot(N) * N;
					Vector3 u2N = u2.Dot(N) * N;
					go->vel = u1 + 2 * m2 / (m1 + m2) * (u2N - u1N);
					go2->vel = u2 + 2 * m1 / (m1 + m2) * (u1N - u2N);
					//go->vel = ()
					//go2->vel =

					v1 = go->vel;
					v2 = go2->vel;

					break;
				}


				CheckCollision(go, go2, dt);
			}
		}
	}
}


void SceneCollision::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_BALL:
		//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
		//Exercise 11: think of a way to give balls different colors
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)) -90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	}
	
}

void SceneCollision::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	


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

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}
	if (m_ghost->active)
	{
		RenderGO(m_ghost);
	}
	
	//On screen text

	//Exercise 5: Render m_objectCount

	//Exercise 8c: Render initial and final momentum
	std::ostringstream ss;
	ss << "m_objectCount: " << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	ss.str("");
	ss << "P1: " << m1 * u1 + m2 * u2;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

	ss.str("");
	ss << "P2: " << m1 * v1 + m2 * v2;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);

	float initialKE = 0.5f * m1 * u1.Dot(u1) + 0.5f * m2 * u2.LengthSquared();
	float finalKE = 0.5f * m1 * u1.Dot(v1) + 0.5f * m2 * v2.LengthSquared();

	ss.str("");
	ss << "E1: " << initialKE;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 21);

	ss.str("");
	ss << "E2: " << finalKE;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 18);

	std::ostringstream ss2;
	ss2.precision(3);
	ss2 << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 0, 6);
	
	std::ostringstream ss3;
	ss3.precision(5);
	ss3 << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 0, 3);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
}
bool SceneCollision::CheckCollision(GameObject *go1,GameObject *go2,double dt)
{
	switch (go2->type)
	{
		{
	case GameObject::GO_BALL:
		float distSq = (go2->pos - go1->pos).LengthSquared();
		float combinedRadiusSq = (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
		return distSq < combinedRadiusSq;
		break;
		}
	case GameObject::GO_WALL:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->dir;
		Vector3 NP = N.Cross(Vector3(0, 0, 1));
		float l = go2->scale.y;
		float r = go1->scale.x;
		float h = go2->scale.x;


		return abs((w0 - b1).Dot(N)) < r + h / 2 && (abs((w0 - b1).Dot(NP)) < r + l * 0.5f);
		break;

	}
	case GameObject::GO_PILLAR:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->dir;
		Vector3 NP = N.Cross(Vector3(0, 0, 1));
		float l = go2->scale.y;
		float r = go1->scale.x;
		float h = go2->scale.x;


		return abs((w0 - b1).Dot(N)) < r + h / 2 && (abs((w0 - b1).Dot(NP)) < r + l * 0.5f);
		break;
	}
	}
	 
			//Exercise 8b: store values in auditing variables

			//Exercise 10: handle collision using momentum swap instead



			//Exercise 12: improve inner loop to prevent double collision

			//Exercise 13: improve collision detection algorithm [solution to be given later] 
		
	
}

float SceneCollision::CheckCollision2(GameObject * go1, GameObject * go2)
{
	Vector3 rel = go1->vel - go2->vel;
	Vector3 dir = go1->pos - go2->pos;
	float r = go1->scale.x + go2->scale.x;
	if (rel.Dot(dir) > 0)
		return -1.f;
	float a = rel.Dot(rel);
	float b = 2 * rel.Dot(dir);
	float c = dir.Dot(dir) - r * r;
	float d = b * b - 4 * a * c; // discriminant??
	if (d < 0)
		return -1;
	float t = (-b - sqrt(d)) / (2 * a);
	return t;
}

void SceneCollision::CollisionResponse(GameObject * go1, GameObject * go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 N = (go2->pos - go1->pos).Normalize();
		Vector3 u1N = u1.Dot(N) * N;
		Vector3 u2N = u2.Dot(N) * N;
		go1->vel = u1 + 2 * m2 / (m1 + m2) * (u2N - u1N);
		go2->vel = u2 + 2 * m1 / (m1 + m2) * (u1N - u2N);
		break;
	}
	case GameObject::GO_WALL:
	{
		Vector3 u = go1->vel;
		Vector3 N = go2->dir;
		go1->vel = u - (2 * u.Dot(N)) * N;
		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 u = go1->vel;
		Vector3 N = go2->dir;
		go1->vel = u - (2 * u.Dot(N)) * N;
		break;
	}

	}
}

void SceneCollision::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
