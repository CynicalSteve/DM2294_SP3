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

	theMap = parse(".//map.csv");

	mapSize = 0;
	while (theMap[0][mapSize] > 0 && theMap[0][mapSize] < 10)
		++mapSize;

	playerInfo = new Player(100, 5.f);
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

	GameObject *lootcrateGO2 = FetchGO();
	lootcrateGO2->type = GameObject::GO_LOOTCRATE;
	lootcrateGO2->pos.set(3, 7);

	GameObject *lootcrateGO3 = FetchGO();
	lootcrateGO3->type = GameObject::GO_LOOTCRATE;
	lootcrateGO3->pos.set(7, 3);
}

int SP3::RandomNumberGen(int FirstNumber, int LastNumber)
{
	srand(time(0));  //Random Num seeder

	//Note that FirstNumber & LastNumber are included in the generator
	int RandomNumber = FirstNumber + (rand() % LastNumber);

	cout << RandomNumber << endl;

	return RandomNumber;
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

void SP3::AlienMovement(double dt)
{
	for (std::vector<alienBase *>::iterator it = alienManager.begin(); it != alienManager.end(); ++it)
	{
		alienBase *go = (alienBase *)*it;

		if (go->move(-1, theMap))
		{
			GameObject::coord distance;
			distance.set(go->pos.x - playerInfo->pos.x, go->pos.y - playerInfo->pos.y);
			if (distance.x == 0 && distance.y == 0);
			else if (distance.x >= abs(distance.y))
			{
				if (go->move(2, theMap))
					go->animationPos.z = 2 + dt * go->getAlienSpeed();
				else if (distance.y > 0)
				{
					if (go->move(3, theMap))
						go->animationPos.z = 3 + dt * go->getAlienSpeed();
				}
				else if (go->move(1, theMap))
					go->animationPos.z = 1 + dt * go->getAlienSpeed();
			}
			else if (distance.y >= abs(distance.x))
			{
				if (go->move(3, theMap))
					go->animationPos.z = 3 + dt * go->getAlienSpeed();
				else if (distance.x > 0)
				{
					if (go->move(2, theMap))
						go->animationPos.z = 2 + dt * go->getAlienSpeed();
				}
				else if (go->move(0, theMap))
					go->animationPos.z = dt * go->getAlienSpeed();
			}
			else if (distance.x < -abs(distance.y))
			{
				if (go->move(0, theMap))
					go->animationPos.z = 0 + dt * go->getAlienSpeed();
				else if (distance.y > 0)
				{
					if (go->move(3, theMap))
						go->animationPos.z = 3 + dt * go->getAlienSpeed();
				}
				else if (go->move(1, theMap))
					go->animationPos.z = 1 + dt * go->getAlienSpeed();
			}
			else
			{
				if (go->move(1, theMap))
					go->animationPos.z = 1 + dt * go->getAlienSpeed();
				else if (distance.x > 0)
				{
					if (go->move(2, theMap))
						go->animationPos.z = 2 + dt * go->getAlienSpeed();
				}
				else if (go->move(0, theMap))
					go->animationPos.z = dt * go->getAlienSpeed();
			}
		}
		go->move(4, theMap);
	}
}

void SP3::PlayerChecks(double dt)
{
	if (playerInfo->getPlayerHealth() < 0) //Reset health to 0 if current player health is under 0
	{
		playerInfo->setPlayerHealth(0);
	}

	if (playerInfo->getPlayerHealth() > playerInfo->getMaxPlayerHealth()) //Health cap
	{
		playerInfo->setPlayerHealth(playerInfo->getMaxPlayerHealth());
	}

	if (playerInfo->getSpeedBoostState() == true) //Speed Boost
	{
		if (playerInfo->speedBoostCooldown < playerInfo->getMaxSpeedBoostCooldownTime())
		{
			playerInfo->speedBoostCooldown += dt;
		}
		else
		{
			playerInfo->setSpeedBoostState(false);
			playerInfo->speedBoostCooldown = 0.f;
			playerInfo->setPlayerSpeed(playerInfo->normalSpeed);
		}
	}
}


void SP3::Update(double dt)
{
	SceneBase::Update(dt);
	doubletime = dt;

	if (Application::IsKeyPressed('9'));

	if (Application::IsKeyPressed('0'));

	//Exercise 6: set m_force values based on WASD
	if (Application::IsKeyPressed('D'))
		if (playerInfo->move(-1, theMap))
			//++playerInfo->pos.x;
			if (playerInfo->move(0, theMap))
				playerInfo->animationPos.z = dt * playerInfo->getPlayerSpeed();

	if (Application::IsKeyPressed('W'))
		if (playerInfo->move(-1, theMap))
			//++playerInfo->pos.y;
			if (playerInfo->move(1, theMap))
				playerInfo->animationPos.z = 1 + dt * playerInfo->getPlayerSpeed();

	if (Application::IsKeyPressed('A'))
		if (playerInfo->move(-1, theMap))
			//--playerInfo->pos.x;
			if (playerInfo->move(2, theMap))
				playerInfo->animationPos.z = 2 + dt * playerInfo->getPlayerSpeed();

	if (Application::IsKeyPressed('S'))
		if (playerInfo->move(-1, theMap))
			//--playerInfo->pos.y;
			if (playerInfo->move(3, theMap))
				playerInfo->animationPos.z = 3 + dt * playerInfo->getPlayerSpeed();

	playerInfo->move(4, theMap);

	if (Application::IsKeyPressed('F') && !KeyBounce['F']) //Temporary button for placing mine
		playerInfo->bombManager.push_back(new MineBomb("MineBomb", 100, 2, playerInfo->pos.x, playerInfo->pos.y));

	if (Application::IsKeyPressed('G') && !KeyBounce['G']) //Lower player health
		playerInfo->setPlayerHealth(playerInfo->getPlayerHealth() - 10);

	if (Application::IsKeyPressed('Z') && !KeyBounce['Z'])
		if (currentAlien < 3)
		{
			alienManager.push_back(new alienGrub("Grub", 100, 4.f, 5, 5, 9, 1));
			++currentAlien;
		}

	if (Application::IsKeyPressed('X') && !KeyBounce['X'])
		if (currentAlien < 3)
		{
			alienManager.push_back(new alienGhoul("Ghoul", 100, 4.f, 5, 5, 9, 1));
			++currentAlien;
		}

	if (Application::IsKeyPressed('C') && !KeyBounce['C'])
		if (currentAlien < 3)
		{
			alienManager.push_back(new alienRaptor("Raptor", 100, 4.f, 5, 5, 9, 1));
			++currentAlien;
		}

	if (Application::IsKeyPressed(VK_UP));

	if (Application::IsKeyPressed(VK_DOWN)); //use this for hold controls

	if (Application::IsKeyPressed(VK_SPACE) && !KeyBounce[VK_SPACE]); //use this for toggle controls

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
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Creation of bomb fire after bomb goes off
	for (std::vector<BombBase *>::iterator it = playerInfo->bombManager.begin(); it != playerInfo->bombManager.end(); ++it)
	{
		BombBase *go = (BombBase *)*it;

		switch (go->type)
		{
		case GameObject::GO_MINEBOMB:
		{
			for (unsigned int x = 0; x < alienManager.size(); ++x)
			{
				if (go->pos == alienManager[x]->pos)
				{
					go->setTriggeredState(true);
				}
			}

			if (go->getTriggeredState() == true)
			{
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

				it = playerInfo->bombManager.erase(it);
			}
			break;
		}
		case GameObject::GO_NORMALBOMB:
		{
			if (go->bombTimer < go->getTimeToExplode())
			{
				go->bombTimer += dt;
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

			it = playerInfo->bombManager.erase(it);
			break;
		}
		}

		if (it == playerInfo->bombManager.end())
			break;
	}

	//Alien Movement
	for (std::vector<alienBase *>::iterator it = alienManager.begin(); it != alienManager.end(); ++it)
	{
		alienBase *go = (alienBase *)*it;

		go->move(4, theMap);

		if (!go->move(-1, theMap))
			continue;

		GameObject::coord distance;

		if (go->alienType == alienBase::TYPE1_GRUB) //chases player
		distance.set(go->pos.x - playerInfo->pos.x, go->pos.y - playerInfo->pos.y);
		else if (go->alienType == alienBase::TYPE2_GHOUL) //random movement
			distance.set(go->pos.x - RandomNumberGen(0, 10), go->pos.y - RandomNumberGen(0, 10));
		else if (go->alienType == alienBase::TYPE3_RAPTOR) //goes to objective
			distance.set(go->pos.x - 5, go->pos.y - 5);

		if (distance.x == 0 && distance.y == 0);
		else if (distance.x >= abs(distance.y))
		{
			if (go->move(2, theMap))
				go->animationPos.z = 2 + dt * go->getAlienSpeed();
			else if (distance.y > 0)
			{
				if (go->move(3, theMap))
					go->animationPos.z = 3 + dt * go->getAlienSpeed();
			}
			else if (go->move(1, theMap))
				go->animationPos.z = 1 + dt * go->getAlienSpeed();
		}
		else if (distance.y >= abs(distance.x))
		{
			if (go->move(3, theMap))
				go->animationPos.z = 3 + dt * go->getAlienSpeed();
			else if (distance.x > 0)
			{
				if (go->move(2, theMap))
					go->animationPos.z = 2 + dt * go->getAlienSpeed();
			}
			else if (go->move(0, theMap))
				go->animationPos.z = dt * go->getAlienSpeed();
		}
		else if (distance.x < -abs(distance.y))
		{
			if (go->move(0, theMap))
				go->animationPos.z = 0 + dt * go->getAlienSpeed();
			else if (distance.y > 0)
			{
				if (go->move(3, theMap))
					go->animationPos.z = 3 + dt * go->getAlienSpeed();
			}
			else if (go->move(1, theMap))
				go->animationPos.z = 1 + dt * go->getAlienSpeed();
		}
		else
		{
			if (go->move(1, theMap))
				go->animationPos.z = 1 + dt * go->getAlienSpeed();
			else if (distance.x > 0)
			{
				if (go->move(2, theMap))
					go->animationPos.z = 2 + dt * go->getAlienSpeed();
			}
			else if (go->move(0, theMap))
				go->animationPos.z = dt * go->getAlienSpeed();
		}
	}

	//Check m_golist against other list for interactions
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

		//Checks if any m_golist is in the bombfire
		bool lostHealth = false;

		for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
		{
			GameObject *go2 = (GameObject *)*it2;

			if (!go2->active)
				continue;

			if (go->pos != go2->pos)
				continue;

			if (go->type == GameObject::GO_BOMBFIRE && go2->type == GameObject::GO_LOOTCRATE) //bombfire lootcrate
			{
				go2->active = false;

				int RandomDropper = RandomNumberGen(1, 3);

				GameObject *PowerupGO = FetchGO();
				PowerupGO->pos.set(go->pos.x, go->pos.y);

				if (RandomDropper == 1)
				{
					PowerupGO->type = GameObject::GO_POWERUP_HEALTH;
					PowerupGO->scale.Set(0.1, 0.1, 1);
				}
				else if (RandomDropper == 2)
				{
					PowerupGO->type = GameObject::GO_POWERUP_SPEED;
					PowerupGO->scale.Set(1, 1, 1);
				}
				else if (RandomDropper == 3)
				{
					PowerupGO->type = GameObject::GO_POWERUP_EQUIPMENT;
					PowerupGO->scale.Set(1, 1, 1);
				}
			}

			if (go->type == GameObject::GO_PLAYER)
			{
				if (go2->type == GameObject::GO_BOMBFIRE) //player-bombfire
				{
					if (playerInfo->loseHealthCooldown == 0.f)
					{
						playerInfo->subtractHealth(10);
						playerInfo->loseHealthCooldown += dt;
					}
					else
					{
						playerInfo->loseHealthCooldown += dt;

						if (playerInfo->loseHealthCooldown > 1.f)
							playerInfo->loseHealthCooldown = 0.f;
					}
				}

				else if (go2->type == GameObject::GO_POWERUP_HEALTH) //player-health
				{
					if (playerInfo->getPlayerHealth() != playerInfo->getMaxPlayerHealth())  //Checks if the player already has full health
					{
						playerInfo->addHealth(20);
						go2->active = false;
					}
				}

				else if (go2->type == GameObject::GO_POWERUP_SPEED) //player-speed
				{
					if (playerInfo->getSpeedBoostState() == false)  //Checks if the player already has speed boost
					{
						playerInfo->setSpeedBoostState(true);
						playerInfo->setPlayerSpeed(playerInfo->getPlayerSpeed() * 1.5f); //50% more speed
					}
					else  //If player has speed boost active, reset the cooldown time
					{
						playerInfo->speedBoostCooldown = 0.f;
					}
					go2->active = false;
				}

				else if (go2->type == GameObject::GO_POWERUP_EQUIPMENT) //player-currency
				{
					playerInfo->addCurrency(30);
					go2->active = false;
				}
			}
		}

		if (go->type != GameObject::GO_BOMBFIRE)
			continue;

		for (unsigned int i = 0; i < alienManager.size(); ++i) //bombfire-alien
			if (go->pos == alienManager[i]->pos)
			{
				playerInfo->addCurrency(alienManager[i]->getAlienCurrencyWorth());
				alienManager.erase(alienManager.begin() + i);
				--currentAlien;
			}
	}

	//Player stats checks & adjustments
	PlayerChecks(dt);

	for (short i = 0; i < 256; ++i)
	{
		if (Application::IsKeyPressed(i))
			KeyBounce[i] = true;
		else KeyBounce[i] = false;
	}
}

void SP3::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_PLAYER:
		modelStack.PushMatrix(); //player
		{
			modelStack.Translate(go->animationPos.x, go->animationPos.y, 0);
			RenderMesh(meshList[GEO_PLAYER], false);
		}
		modelStack.PopMatrix(); ///player
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

		break;
	}
	case GameObject::GO_LOOTCRATE:
	{
		modelStack.PushMatrix(); //lootcrate
		{
			modelStack.Translate(go->pos.x, go->pos.y, 0);
			RenderMesh(meshList[GEO_LOOTCRATE], false);
		}
		modelStack.PopMatrix(); ///lootcrate

		break;
	}
	case GameObject::GO_POWERUP_HEALTH:
	{
		modelStack.PushMatrix(); //health powerup
		{
			modelStack.Translate(go->pos.x, go->pos.y, 0);
			RenderMesh(meshList[GEO_HEALTH], false);
		}
		modelStack.PopMatrix(); ///health powerup

		break;
	}

	case GameObject::GO_POWERUP_SPEED:
	{
		modelStack.PushMatrix(); //speed boost powerup
		{
			modelStack.Translate(go->pos.x, go->pos.y, 0);
			RenderMesh(meshList[GEO_POWERUP_SPEED], false);
		}
		modelStack.PopMatrix(); ///speed boost powerup

		break;
	}
	case GameObject::GO_POWERUP_EQUIPMENT:
	{
		modelStack.PushMatrix(); //speed boost powerup
		{
			modelStack.Translate(go->pos.x, go->pos.y, 0);
			RenderMesh(meshList[GEO_EQUIPMENT], false);
		}
		modelStack.PopMatrix(); ///speed boost powerup

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
		modelStack.PushMatrix(); //AlienGrub
		{
			modelStack.Translate(alien->animationPos.x, alien->animationPos.y, 0);
			RenderMesh(meshList[GEO_ALIENGRUB], false);
		}
		modelStack.PopMatrix(); ///AlienGrub
		break;
	case alienBase::TYPE2_GHOUL:
		modelStack.PushMatrix(); //AlienGhoul
		{
			modelStack.Translate(alien->animationPos.x, alien->animationPos.y, 0);
			RenderMesh(meshList[GEO_ALIENGHOUL], false);
		}
		modelStack.PopMatrix(); ///AlienGhoul
		break;
	case alienBase::TYPE3_RAPTOR:
		modelStack.PushMatrix(); //AlienRaptor
		{
			modelStack.Translate(alien->animationPos.x, alien->animationPos.y, 0);
			RenderMesh(meshList[GEO_ALIENRAPTOR], false);
		}
		modelStack.PopMatrix(); ///AlienRaptor
		break;
	default:
		break;
	}
}

void SP3::renderBombs(BombBase *bomb, int currentBombIndex)
{
	switch (bomb->type)
	{
	case GameObject::GO_NORMALBOMB:
	{
		modelStack.PushMatrix(); //normal bomb
	{
		modelStack.Translate(bomb->pos.x, bomb->pos.y, 0);
		RenderMesh(meshList[GEO_NORMALBOMB], false);
	}
	modelStack.PopMatrix(); ///normal bomb
		break;
	}
	case GameObject::GO_MINEBOMB:
	{
		modelStack.PushMatrix(); //mine bomb
		{
			modelStack.Translate(bomb->pos.x, bomb->pos.y, 0);
			RenderMesh(meshList[GEO_MINEBOMB], false);
		}
		modelStack.PopMatrix(); ///mine bomb
		break;
	}
	default:
		break;
	}

}

void SP3::renderUI()
{
	

	modelStack.PushMatrix(); //UI background
	{
		modelStack.Translate(100, 100, 0);
		modelStack.Scale(200, 20, 1);
		RenderMesh(meshList[GEO_QUAD], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Health Bar (Red)
	{
		modelStack.Translate(25, 95, 0);
		modelStack.Scale(100 / 3, 5, 1);
		RenderMesh(meshList[GEO_HEALTH_BAR_RED], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Health Bar (Green)
	{
		modelStack.Translate(25, 95, 0);
		modelStack.Scale(playerInfo->getPlayerHealth() / 3, 5, 1);
		RenderMesh(meshList[GEO_HEALTH_BAR_GREEN], false);
	}
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
	//RenderMesh(meshList[GEO_HOUSE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //grid system
	{
		theMap[11];
		modelStack.Scale(100. / mapSize, 100. / mapSize, 1);
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

	renderUI();  //Main Game UI

	std::ostringstream ss;
	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "EP: " << playerInfo->getEquipmentCurrency();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.str("");
	ss.precision(5);
	ss << playerInfo->getPlayerHealth() << "/" << playerInfo->getMaxPlayerHealth();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 4.5f, 41.6f);

	ss.str("");
	ss.precision(5);
	ss << "Speed: " << playerInfo->speedBoostCooldown << "/" << playerInfo->getMaxSpeedBoostCooldownTime();
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