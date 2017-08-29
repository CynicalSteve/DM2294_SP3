#include "SP3.h"
#include "GL\glew.h"
#include "Application.h"
#include <Windows.h>
#include <sstream>
#include <fstream>


using Math::RandIntMinMax;

std::vector<GameObject::coord> alienBase::spawnPosition;

SP3::SP3() : alienManager(NULL), isPaused(false)
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
	ReadSettings();

	for (size_t i = 0; i < 256; ++i)
	{
		KeyBounce[i] = false;
	}

	theMap = parse(".//map.csv");
	mapSize = 0;
	while (theMap[0][mapSize] > 0 && theMap[0][mapSize] < 10)
		++mapSize;
	alienBase::createSpawnPosition(theMap, mapSize);

	for (short y = mapSize - 1; y > -1; --y)
	{
		for (short x = 0; x < mapSize; ++x)
			std::cout << theMap[x][y] << " ";
		std::cout << "\n";
	}
	for (short i = 0; i < alienBase::spawnPosition.size(); ++i)
	std::cout << alienBase::spawnPosition[i].x << " " << alienBase::spawnPosition[i].y << "\n";

	pauseSelection = CONTINUE;
	pauseSelectionIterator = 0;

	shopselection = NORMALBOMB;
	shopSelectionIterator = 0;

	loseSelection = RESTART;
	loseSelectionIterator = 0;

	loseBombPosition = 0.f;
	dayNumber = 1;

	gameState = WAVE_STATE;

	playerInfo = new Player(100, 5.f);
	playerInfo->pos.set(1, 1);
	playerInfo->animationPos.Set(playerInfo->pos.x, playerInfo->pos.y, -1);
	playerInfo->type = GameObject::GO_PLAYER;

	playerInfo->playerInventory[0]->inventoryBombType = Inventory::INVENTORY_NORMALBOMB;
	playerInfo->playerInventory[0]->setDiscoveredState(true);
	playerInfo->playerInventory[0]->setBombAmount(100);

	playerInfo->playerInventory[1]->inventoryBombType = Inventory::INVENTORY_MINEBOMB;
	playerInfo->playerInventory[1]->setBombAmount(100);

	playerInfo->playerInventory[2]->inventoryBombType = Inventory::INVENTORY_NUKEBOMB;
	playerInfo->playerInventory[2]->setBombAmount(100);

	for (short i = 0; i < 3; ++i)
	{
		alienManager.push_back(new alienGrub("Grub", 10, 1.f, 3, 2, 0, 0));
		alienManager.push_back(new alienGhoul("Ghoul", 40, 2.f, 5, 5, 0, 0));
		alienManager.push_back(new alienRaptor("Raptor", 20, 4.f, 4, 5, 0, 0));
	}

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
	lootcrateGO2->pos.set(2, 7);

	GameObject *lootcrateGO3 = FetchGO();
	lootcrateGO3->type = GameObject::GO_LOOTCRATE;
	lootcrateGO3->pos.set(7, 2);

	houseHealth = FetchGO();
	houseHealth->type = GameObject::GO_HOUSE;
	//houseHealth->pos.set(, 2);
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

void SP3::ReadSettings()
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

void SP3::renderShopScreen()
{
	modelStack.PushMatrix(); //Shop Screen Screen Background
	{
		modelStack.Translate(50, 50, 0);
		modelStack.Scale(70, 80, 1);
		RenderMesh(meshList[GEO_QUAD], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-5, 0, 0);

	modelStack.PushMatrix(); //Normal Bomb
	{
		modelStack.Translate(40, 80, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_NORMALBOMB], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Mine Bomb
	{
		modelStack.Translate(40, 65, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_MINEBOMB], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Nuke Bomb
	{
		modelStack.Translate(40, 50, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_NUKEBOMB], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //House repair
	{
		modelStack.Translate(40, 35, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_REPAIR], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(70, 80, 0);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_EQUIPMENT], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(70, 65, 0);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_EQUIPMENT], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(70, 50, 0);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_EQUIPMENT], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(70, 35, 0);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_EQUIPMENT], false);
	}
	modelStack.PopMatrix();

	modelStack.PopMatrix();
	float normal_G = 1.f, normal_B = 1.f, mine_G = 1.f, mine_B = 1.f, nuke_G = 1.f, nuke_B = 1.f, repair_G = 1.f, repair_B = 1.f;

	if (shopselection == NORMALBOMB)
	{
		normal_G = 0.549f;
		normal_B = 0.f;
	}
	else if (shopselection == MINEBOMB)
	{
		mine_G = 0.549f;
		mine_B = 0.f;
	}
	else if (shopselection == NUKEBOMB)
	{
		nuke_G = 0.549f;
		nuke_B = 0.f;
	}
	else if (shopselection == REPAIR)
	{
		repair_G = 0.549f;
		repair_B = 0.f;
	}

	std::ostringstream ss;

	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "5";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, normal_G, normal_B), 5, 73.f, 77.5f);

	ss.str("");
	ss.precision(5);
	ss << "15";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, mine_G, mine_B), 5, 73.f, 62.5f);

	ss.str("");
	ss.precision(5);
	ss << "50";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, nuke_G, nuke_B), 5, 73.f, 47.5f);

	ss.str("");
	ss.precision(5);
	ss << "30";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, repair_G, repair_B), 5, 73.f, 32.5f);

	ss.str("");
	ss.precision(5);
	ss << "Press 'N' to start";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0.549f, 0), 3.5f, 20.f, 20.5f);
	
	ss.str("");
	ss.precision(5);
	ss << "the next day";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0.549f, 0), 3.5f, 30.f, 17.5f);
	
}

void SP3::AlienMovement(double dt)
{
	int totalHealth = 0;
	for (std::vector<alienBase *>::iterator it = alienManager.begin(); it != alienManager.end(); ++it)
	{
		alienBase *go = (alienBase *)*it;

		totalHealth += go->getAlienHealth();

		if (!go->active)
			continue;

		go->move(4, theMap);

		if (!go->move(-1, theMap))
			continue;

		GameObject::coord distance;

		if (go->alienType == alienBase::TYPE1_GRUB || go->alienType == alienBase::TYPE4_GOLIATH) //chases player
			distance.set(go->pos.x - playerInfo->pos.x, go->pos.y - playerInfo->pos.y);
		else if (go->alienType == alienBase::TYPE2_GHOUL) //random movement
			distance.set(go->pos.x - RandIntMinMax(0, 19), go->pos.y - RandIntMinMax(0, 19));
		else if (go->alienType == alienBase::TYPE3_RAPTOR || go->alienType == alienBase::TYPE5_LEVIATHAN) //goes to objective
			distance.set(go->pos.x - houseHealth->pos.x, go->pos.y - houseHealth->pos.y);

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
	if (!totalHealth)
		gameState = WAVE_END_STATE;
}

void SP3::PlayerChecks(double dt)
{
	//std::cout << playerInfo->normalSpeed << "\n";
	if (playerInfo->getPlayerHealth() <= 0) //Reset health to 0 if current player health is under 0
	{
		playerInfo->setPlayerHealth(0);

		gameState = LOSE_STATE; //Player loses game when health is 0
	}

	if (playerInfo->getEquipmentCurrency() < 0) //Reset currency to 0 if current player currency is under 0
	{
		playerInfo->setPlayerCurrency(0);
	}

	if (playerInfo->getPlayerHealth() > playerInfo->getMaxPlayerHealth()) //Health cap
	{
		playerInfo->setPlayerHealth(playerInfo->getMaxPlayerHealth());
	}

	if (houseHealth->houseHealth > 250) //House Health cap
	{
		houseHealth->houseHealth = 250;
	}
	if (houseHealth->houseHealth <= 0) //Reset health to 0 if house health is under 0
	{
		houseHealth->houseHealth = 0;

		//gameState = LOSE_STATE; //Player loses game when house health is 0
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

	if (playerInfo->getInvulnerabilityState() == true) //Invulnerability
	{
		if (playerInfo->invulnerabilityCooldown < playerInfo->getMaxInvulnerableTime())
		{
			playerInfo->invulnerabilityCooldown += dt;

			if (playerInfo->getPlayerHealth() < playerInfo->invulnerabilityHealth) //Health cap
			{
				playerInfo->setPlayerHealth(playerInfo->invulnerabilityHealth);
			}
		}
		else
		{
			playerInfo->setInvulnerabilityState(false);
			playerInfo->invulnerabilityCooldown = 0.f;
		}
	}
	if (playerInfo->getNukeDeployedState() == true)  //Using nuke
	{
		if (playerInfo->getCurrentTimer() < playerInfo->getMaxBombTimer())
		{
			playerInfo->addToTimer(dt);

			playerInfo->countdown -= dt;
		}
		else
		{
			playerInfo->setCurrentTimer(0.f);
			alienManager.clear();
			playerInfo->setNukeDeployedState(false);

			if (hasSound == true)
			{
				PlaySound(TEXT("Image//nukeBoom.wav"), NULL, SND_ASYNC);
			}
		}
	}
}

void SP3::BombFireCreation(double dt)
{
	for (std::vector<BombBase *>::iterator it = playerInfo->bombManager.begin(); it != playerInfo->bombManager.end(); ++it)
	{
		BombBase *go = (BombBase *)*it;

		switch (go->type)
		{
		case GameObject::GO_MINEBOMB:
		{
			for (unsigned int x = 0; x < alienManager.size(); ++x) //Checks if any alien stepped on the mine
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

				if (hasSound == true)
				{
					PlaySound(TEXT("Image//bombBoom.wav"), NULL, SND_ASYNC);
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
			if (hasSound == true)
			{
				PlaySound(TEXT("Image//bombBoom.wav"), NULL, SND_ASYNC);
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
}

void SP3::m_goListInteractions(double dt)
{
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

				int RandomDropper = RandIntMinMax(1, 4);

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
				else if (RandomDropper == 4)
				{
					PowerupGO->type = GameObject::GO_POWERUP_INVULNERABLE;
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
				else if (go2->type == GameObject::GO_POWERUP_INVULNERABLE) //player-invulnerability
				{
					if (playerInfo->getInvulnerabilityState() == false)  //Checks if the player already has invulnerability
					{
						playerInfo->invulnerabilityHealth = playerInfo->getPlayerHealth();
						playerInfo->setInvulnerabilityState(true);
					}
					else  //If player has invulnerability active, reset the cooldown time
					{
						playerInfo->invulnerabilityCooldown = 0.f;
					}
					go2->active = false;
				}
				else if (go->type == GameObject::GO_PLAYER)
				{
					for (unsigned int i = 0; i < alienManager.size(); ++i) //player-alien
					{
						if (go->pos == alienManager[i]->pos)
						{
							if (playerInfo->loseHealthCooldown == 0.f)
							{
								playerInfo->subtractHealth(alienManager[i]->getAlienDamage());

								playerInfo->loseHealthCooldown += dt;
							}
							else
							{
								playerInfo->loseHealthCooldown += dt;

								if (playerInfo->loseHealthCooldown > 1.f)
								{
									playerInfo->loseHealthCooldown = 0.f;
								}
							}
						}
					}
				}
			}
		}

		if (go->type == GameObject::GO_BOMBFIRE)
		{
			for (unsigned int i = 0; i < alienManager.size(); ++i) //bombfire-alien
			{
				if (!alienManager[i]->active)
					continue;

				if (go->pos == alienManager[i]->pos)
				{

					if (alienManager[i]->loseHealthCooldown == 0.f)
					{
						alienManager[i]->subtractAlienHealth(10);
						alienManager[i]->loseHealthCooldown += dt;
					}
					else
					{
						alienManager[i]->loseHealthCooldown += dt;

						if (alienManager[i]->loseHealthCooldown > 1.f)
							alienManager[i]->loseHealthCooldown = 0.f;
					}
					
					if (alienManager[i]->getAlienHealth() <= 0)
					{
						alienManager[i]->active = false;
						//alienManager.erase(alienManager.begin() + i);
					}
				}
			}

			if (go->type == GameObject::GO_HOUSE)
			{
				for (unsigned int i = 0; i < alienManager.size(); ++i) //house-alien
				{
					if (go->pos == alienManager[i]->pos)
					{
						if (go->loseHealthCooldown == 0.f)
						{

							houseHealth->houseHealth -= alienManager[i]->getAlienDamage();
							houseHealth->loseHealthCooldown+= dt;

}
						else
						{
							houseHealth->loseHealthCooldown += dt;

							if (houseHealth->loseHealthCooldown > 1.f)
							{
								houseHealth->loseHealthCooldown = 0.f;
							}

							if (go->houseHealth <= 0.f)
							{
							gameState = LOSE_STATE;
						}
					}
				}
			}
		}
	}
}
}

void SP3::spawnAliens(double dt)
{
	short whichAlien = RandIntMinMax(0, alienManager.size() * 60);
	if (whichAlien >= alienManager.size())
		return;
	GameObject::coord spawn = alienBase::spawnPosition[RandIntMinMax(0, 2)];

	for (short i = 0; i < alienManager.size(); ++i)
	{
		if (!alienManager[(whichAlien + i) % alienManager.size()]->active && alienManager[(whichAlien + i) % alienManager.size()]->getAlienHealth() > 0)
		{
			alienManager[(whichAlien + i) % alienManager.size()]->active = true;
			alienManager[(whichAlien + i) % alienManager.size()]->pos = spawn;
			alienManager[(whichAlien + i) % alienManager.size()]->animationPos.Set(spawn.x, spawn.y, -1);
			break;
		}
	}

	/*if (alienType >= 0 && alienType <= 34) //Grub - 35% 
	{
		alienManager.push_back(new alienGrub("Grub", 10, 1.f, 3, 2, alienBase::spawnPosition[RandIntMinMax(0, 2)].x, alienBase::spawnPosition[RandIntMinMax(0, 2)].y));
	}
	else if (alienType >= 35 && alienType <= 59) //Ghoul - 25%
	{
		alienManager.push_back(new alienGhoul("Ghoul", 40, 2.f, 5, 5, alienBase::spawnPosition[RandIntMinMax(0, 2)].x, alienBase::spawnPosition[RandIntMinMax(0, 2)].y));
	}
	else if (alienType >= 60 && alienType <= 84) //Raptor - 25%
	{
		alienManager.push_back(new alienRaptor("Raptor", 20, 4.f, 4, 5, alienBase::spawnPosition[RandIntMinMax(0, 2)].x, alienBase::spawnPosition[RandIntMinMax(0, 2)].y));
	}
	else if (alienType >= 85 && alienType <= 94) //Goliath - 10%
	{
		alienManager.push_back(new alienGoliath("Goliath", 50, 1.f, 15, 10, alienBase::spawnPosition[Math::RandIntMinMax(0, 2)].x, alienBase::spawnPosition[Math::RandIntMinMax(0, 2)].y));
	}
	else if (alienType >= 95 && alienType <= 99) //Leviathan - 5%
	{
		alienManager.push_back(new alienLeviathan("Leviathan", 100, .6f, 25, 30, alienBase::spawnPosition[Math::RandIntMinMax(0, 2)].x, alienBase::spawnPosition[Math::RandIntMinMax(0, 2)].y));
	}*/
}

void SP3::Update(double dt)
{
	SceneBase::Update(dt);
	doubletime = dt;

	if (gameState == WAVE_STATE)
	{
		if (isPaused == false)
		{
			//Exercise 6: set m_force values based on WASD
			if (Application::IsKeyPressed('D') && playerInfo->pos.x < mapSize - 1)
				if (playerInfo->move(-1, theMap))
					//++playerInfo->pos.x;
					if (playerInfo->move(0, theMap))
						playerInfo->animationPos.z = dt * playerInfo->getPlayerSpeed();

			if (Application::IsKeyPressed('W') && playerInfo->pos.y < mapSize - 1)
				if (playerInfo->move(-1, theMap))
					//++playerInfo->pos.y;
					if (playerInfo->move(1, theMap))
						playerInfo->animationPos.z = 1 + dt * playerInfo->getPlayerSpeed();

			if (Application::IsKeyPressed('A') && playerInfo->pos.x > 0)
				if (playerInfo->move(-1, theMap))
					//--playerInfo->pos.x;
					if (playerInfo->move(2, theMap))
						playerInfo->animationPos.z = 2 + dt * playerInfo->getPlayerSpeed();

			if (Application::IsKeyPressed('S') && playerInfo->pos.y > 0)
				if (playerInfo->move(-1, theMap))
					//--playerInfo->pos.y;
					if (playerInfo->move(3, theMap))
						playerInfo->animationPos.z = 3 + dt * playerInfo->getPlayerSpeed();

			playerInfo->move(4, theMap);

			if (Application::IsKeyPressed('G') && !KeyBounce['G']) //Lower player health
				playerInfo->subtractHealth(10);

			if (Application::IsKeyPressed('Q') && !KeyBounce['Q']) //Inventory switching
			{
					if (playerInfo->currentBomb != 0)
					{
						playerInfo->currentBomb--;
					}
			}

			if (Application::IsKeyPressed('E') && !KeyBounce['E']) //Inventory switching
			{
					if (playerInfo->currentBomb < sizeof(playerInfo->playerInventory))
					{
						playerInfo->currentBomb++;
					}
			}

			if (Application::IsKeyPressed('Z') && !KeyBounce['Z'])
			{
				alienManager.push_back(new alienGrub("Grub", 10, 1.f, 3, 2, 9, 1));
			}

			if (Application::IsKeyPressed('X') && !KeyBounce['X'])
			{
				alienManager.push_back(new alienGhoul("Ghoul", 40, 2.f, 5, 5, 9, 1));
			}

			if (Application::IsKeyPressed('C') && !KeyBounce['C'])
			{
				alienManager.push_back(new alienRaptor("Raptor", 20, 4.f, 4, 5, 9, 1));
			}

			//Mouse Section
			static bool bLButtonState = false;

			if (!bLButtonState && Application::IsMousePressed(0))
			{
				bLButtonState = true;
				std::cout << "LBUTTON DOWN" << std::endl;

				if (playerInfo->playerInventory[playerInfo->currentBomb]->getBombAmount() > 0)
				{
					switch (playerInfo->playerInventory[playerInfo->currentBomb]->inventoryBombType)
					{
					case Inventory::INVENTORY_NORMALBOMB:
					{

						playerInfo->bombManager.push_back(new NormalBomb("Normal Bomb", 30, 3, 2, playerInfo->pos.x, playerInfo->pos.y));
						playerInfo->playerInventory[playerInfo->currentBomb]->subtractBombAmount(1);

						break;
					}
					case Inventory::INVENTORY_MINEBOMB:
					{
						playerInfo->bombManager.push_back(new MineBomb("MineBomb", 100, 2, playerInfo->pos.x, playerInfo->pos.y));
						playerInfo->playerInventory[playerInfo->currentBomb]->subtractBombAmount(1);
						break;
					}
					case Inventory::INVENTORY_NUKEBOMB:
					{
						if (playerInfo->getNukeDeployedState() == false)
						{
							playerInfo->setNukeDeployedState(true);
							playerInfo->setMaxBombTImer(10.f);
							playerInfo->countdown = 10.f;
							playerInfo->playerInventory[playerInfo->currentBomb]->subtractBombAmount(1);
						}
						break;
					}
					default:
						break;
					}
				}
			}
			else if (bLButtonState && !Application::IsMousePressed(0))
			{
				bLButtonState = false;
				std::cout << "LBUTTON UP" << std::endl;
			}

			//Creation of bomb fire after bomb goes off
			BombFireCreation(dt);

			//Spawn ALiens
			spawnAliens(dt);

			//Alien Movement
			AlienMovement(dt);

			//Check m_golist against other list for interactions
			m_goListInteractions(dt);

			//Player stats checks & adjustments
			PlayerChecks(dt);


			if (Application::IsKeyPressed('K') && !KeyBounce['K']) //Wave end
					gameState = WAVE_END_STATE;  

			if (Application::IsKeyPressed('L') && !KeyBounce['L']) //Lose
					gameState = LOSE_STATE;
			}
		else
		{
			if (Application::IsKeyPressed('W') && !KeyBounce['W'])
			{
				if (pauseSelectionIterator != 0)
				{
					--pauseSelectionIterator;
					pauseSelection = static_cast<PauseSelection>(pauseSelectionIterator);
				}
			}

			if (Application::IsKeyPressed('S') && !KeyBounce['S'])
			{
				if (pauseSelectionIterator + 1 != TOTAL_NUM)
				{
					++pauseSelectionIterator;
					pauseSelection = static_cast<PauseSelection>(pauseSelectionIterator);
				}
			}

			if (Application::IsKeyPressed(VK_RETURN) && !KeyBounce[VK_RETURN])
				{
					switch (pauseSelection)
					{
					case CONTINUE:
					{
						isPaused = false;
						break;
					}
					case SETTINGS:
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
						break;
					}
					case EXIT_MAINMENU:
					{
						SceneManager::instance()->SwitchScene(SceneManager::SCENE_STARTMENU);
						break;
					}
					case EXIT_GAME:
					{
						SceneManager::instance()->Quit(true);
						break;
					}

					default:
						break;
					}
			}
		}
	}
	//End of wave (shop screen)
	else if (gameState == WAVE_END_STATE)
	{
		if (isPaused == false)
		{
			if (Application::IsKeyPressed('W') && !KeyBounce['W'])
			{
				if (shopSelectionIterator != 0)
				{
					--shopSelectionIterator;
					shopselection = static_cast<ShopSelection>(shopSelectionIterator);
				}
			}

			if (Application::IsKeyPressed('S') && !KeyBounce['S'])
			{
				if (shopSelectionIterator + 1 != TOTAL_SHOP)
				{
					++shopSelectionIterator;
					shopselection = static_cast<ShopSelection>(shopSelectionIterator);
				}
			}

			if (Application::IsKeyPressed('N') && !KeyBounce['N'])
			{
				++dayNumber;

				playerInfo->setPlayerHealth(playerInfo->getMaxPlayerHealth());

				gameState = WAVE_STATE;
			}

			if (Application::IsKeyPressed(VK_RETURN) && !KeyBounce[VK_RETURN])
				{
					switch (shopselection)
					{
					case NORMALBOMB:
					{
						if (playerInfo->getEquipmentCurrency() >= 5)
						{
							playerInfo->subtractCurrency(5);
							playerInfo->playerInventory[0]->addBombAmount(1);
						}
						break;
					}
					case MINEBOMB:
					{
						if (playerInfo->getEquipmentCurrency() >= 15)
						{
							playerInfo->subtractCurrency(15);
							playerInfo->playerInventory[1]->addBombAmount(1);

							if (playerInfo->playerInventory[1]->getDiscoveredState() == false)
							{
								playerInfo->playerInventory[1]->setDiscoveredState(true);
							}
						}
						break;
					}
					case NUKEBOMB:
					{
						if (playerInfo->getEquipmentCurrency() >= 50)
						{
							playerInfo->subtractCurrency(50);
							playerInfo->playerInventory[2]->addBombAmount(1);

							if (playerInfo->playerInventory[2]->getDiscoveredState() == false)
							{
								playerInfo->playerInventory[2]->setDiscoveredState(true);
							}
						}
						break;
					}
					case REPAIR:
					{
						if (playerInfo->getEquipmentCurrency() >= 30)
						{
							playerInfo->subtractCurrency(30);

							houseHealth->houseHealth += 30;
						}
						break;
					}

					default:
						break;
					}
			}
		}

		else
		{
			if (Application::IsKeyPressed('W') && !KeyBounce['W'])
			{
				if (pauseSelectionIterator != 0)
				{
					--pauseSelectionIterator;
					pauseSelection = static_cast<PauseSelection>(pauseSelectionIterator);
				}
			}

			if (Application::IsKeyPressed('S') && !KeyBounce['S'])
			{
				if (pauseSelectionIterator + 1 != TOTAL_NUM)
				{
					++pauseSelectionIterator;
					pauseSelection = static_cast<PauseSelection>(pauseSelectionIterator);
				}
			}

			if (Application::IsKeyPressed(VK_RETURN) && !KeyBounce[VK_RETURN])
				{
					switch (pauseSelection)
					{
					case CONTINUE:
					{
						isPaused = false;
						break;
					}
					case SETTINGS:
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
						break;
					}
					case EXIT_MAINMENU:
					{
						SceneManager::instance()->SwitchScene(SceneManager::SCENE_STARTMENU);
						break;
					}
					case EXIT_GAME:
					{
						SceneManager::instance()->Quit(true);
						break;
					}

					default:
						break;
					}
			}
		}
	} 
	else if (gameState == LOSE_STATE)
		{
		if (Application::IsKeyPressed('W') && !KeyBounce['W'])
			{
				if (loseSelectionIterator != 0)
				{
					--loseSelectionIterator;
					loseSelection = static_cast<LoseSelection>(loseSelectionIterator);
					loseBombPosition += 25;
				}
		}

		if (Application::IsKeyPressed('S') && !KeyBounce['S'])
			{
				if (loseSelectionIterator + 1 != TOTAL_LOSE)
				{
					++loseSelectionIterator;
					loseSelection = static_cast<LoseSelection>(loseSelectionIterator);
					loseBombPosition -= 25;
				}
		}

		if (Application::IsKeyPressed(VK_RETURN) && !KeyBounce[VK_RETURN])
			{
				switch (loseSelection)
				{
				case RESTART:
				{
					SceneManager::instance()->SwitchScene(SceneManager::SCENE_MAINGAME);
					break;
				}
				case LOSE_EXIT_MAINMENU:
				{
					SceneManager::instance()->SwitchScene(SceneManager::SCENE_STARTMENU);
					break;
				}
				case LOSE_EXIT_GAME:
				{
					SceneManager::instance()->Quit(true);
					break;
				}

				default:
					break;
				}
		}
	}

	if (Application::IsKeyPressed('P') && !KeyBounce['P'])  //Pause
	{
		if (isPaused == false)
		{
			isPaused = true;
		}
		else
		{
			isPaused = false;
		}
	}

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
	case GameObject::GO_POWERUP_INVULNERABLE:
	{
		modelStack.PushMatrix(); //invulnerability powerup
		{
			modelStack.Translate(go->pos.x, go->pos.y, 0);
			RenderMesh(meshList[GEO_POWERUP_INVULNERABLE], false);
		}
		modelStack.PopMatrix(); ///invulnerability powerup

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

	case alienBase::TYPE4_GOLIATH:
		modelStack.PushMatrix(); //AlienGoliath
		{
			modelStack.Translate(alien->animationPos.x, alien->animationPos.y, 0);
			RenderMesh(meshList[GEO_ALIENGOLIATH], false);
		}
		modelStack.PopMatrix(); ///AlienGoliath
		break;

	case alienBase::TYPE5_LEVIATHAN:
		modelStack.PushMatrix(); //AlienLeviathan
		{
			modelStack.Translate(alien->animationPos.x, alien->animationPos.y, 0);
			RenderMesh(meshList[GEO_ALIENLEVIATHAN], false);
		}
		modelStack.PopMatrix(); ///AlienLeviathan
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
		modelStack.Translate(145, 50, 0);
		modelStack.Scale(90, 100 , 1);
		RenderMesh(meshList[GEO_QUAD], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Player icon for health
	{
		modelStack.Translate(120, 75, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_PLAYER], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //House Health Icon
	{
		modelStack.Translate(110, 90, 0);
		modelStack.Scale(15, 15, 1);
		RenderMesh(meshList[GEO_HOUSE], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //House Health Bar (Red)
	{
		modelStack.Translate(145, 90, 0);
		modelStack.Scale(250 / 5, 5, 1);
		RenderMesh(meshList[GEO_HEALTH_BAR_RED], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //House Health Bar (Green)
	{
		modelStack.Translate(145, 90, 0);
		modelStack.Scale(houseHealth->houseHealth / 5, 5, 1);
		RenderMesh(meshList[GEO_HEALTH_BAR_GREEN], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Health Bar (Red)
	{
		modelStack.Translate(145, 75, 0);
		modelStack.Scale(100 / 3, 5, 1);
		RenderMesh(meshList[GEO_HEALTH_BAR_RED], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Health Bar (Green)
	{
		modelStack.Translate(145, 75, 0);
		modelStack.Scale(playerInfo->getPlayerHealth() / 3, 5, 1);
		RenderMesh(meshList[GEO_HEALTH_BAR_GREEN], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Bomb Selector
	{
		if (playerInfo->currentBomb == 0)
		{
			modelStack.Translate(145, 50, 0);
		}
		else if (playerInfo->currentBomb == 1)
		{
			modelStack.Translate(145, 35, 0);
		}
		else
		{
			modelStack.Translate(145, 20, 0);
		}
		modelStack.Scale(29, 10, 1);
		RenderMesh(meshList[GEO_BOMBSELECTOR], true);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Equipment
	{
		modelStack.Translate(135, 65, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_EQUIPMENT], true);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Normal Bomb
	{
		modelStack.Translate(135, 50, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_NORMALBOMB], true);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Mine Bomb
	{
		modelStack.Translate(135, 35, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_MINEBOMB], true);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //Nuke Bomb
	{
		modelStack.Translate(135, 20, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_NUKEBOMB], true);
	}
	modelStack.PopMatrix();

	if (playerInfo->getNukeDeployedState() == true)
	{
		modelStack.PushMatrix(); //Nuke Bomb Timer Symbol
		{
			modelStack.Translate(140, 5, 0);
			modelStack.Scale(7, 7, 1);
			RenderMesh(meshList[GEO_NUKEBOMB], true);
		}
		modelStack.PopMatrix();

		int countdown = playerInfo->countdown;

		std::ostringstream ss;
		ss.str("");
		ss.precision(2);
		ss << countdown;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 5, 145.f, 2.5f);

		ss.str("");
		ss.precision(2);
		ss << "Day:" << dayNumber;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0.549f, 0), 5, 0.f, 2.5f);
	}
}

void SP3::RenderPauseUI()
{
	modelStack.PushMatrix(); //Pause Screen Background
	{
		modelStack.Translate(50, 50, 0);
		modelStack.Scale(70, 80, 1);
		RenderMesh(meshList[GEO_QUAD], false);
	}
	modelStack.PopMatrix();

	std::ostringstream ss;
	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "Paused";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 8, 27.f, 80.f);
	
	float continue_G = 1.f, continueB = 1.f, settings_G = 1.f, settings_B = 1.f, mainMenu_G = 1.f, mainMenu_B = 1.f, exit_G = 1.f, exit_B = 1.f;

	if (pauseSelection == CONTINUE)
	{
		continue_G = 0.549f; 
		continueB = 0.f;
	}
	else if (pauseSelection == SETTINGS)
	{
		settings_G = 0.549f;
		settings_B = 0.f;
	}
	else if (pauseSelection == EXIT_MAINMENU)
	{
		mainMenu_G = 0.549f;
		mainMenu_B = 0.f;
	}
	else if (pauseSelection == EXIT_GAME)
	{
		exit_G = 0.549f;
		exit_B = 0.f;
	}

	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "Continue";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, continue_G, continueB), 4.f, 35.f, 60.6f);

	ss.str("");
	ss.precision(5);
	if (hasSound == true)
	{
		ss << "Sound: " << "On";
	}
	else
	{
		ss << "Sound: " << "Off";
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, settings_G, settings_B), 4.f, 32.f, 45.6f);

	ss.str("");
	ss.precision(5);
	ss << "Exit to Main Menu";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, mainMenu_G, mainMenu_B), 4.f, 17.f, 30.6f);

	ss.str("");
	ss.precision(5);
	ss << "Exit Game";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, exit_G, exit_B), 4.f, 32.f, 15.6f);
}

void SP3::RenderLoseUI()
{
	modelStack.PushMatrix();  //Start Menu Background
	{
		modelStack.Translate(50, 50, 0);
		modelStack.Scale(100, 110, 0);
		RenderMesh(meshList[GEO_STARTMENU_BACKGROUND], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();  //Restart Button
	{
		modelStack.Translate(50, 75, 1);
		modelStack.Scale(43, 32, 0);
		RenderMesh(meshList[GEO_RESTART], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();  //Exit to Main Menu Button
	{
		modelStack.Translate(50, 50, 1);
		modelStack.Scale(43, 32, 0);
		RenderMesh(meshList[GEO_EXIT_MAINMENU], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();  //Exit Game Button
	{
		modelStack.Translate(50, 25, 1);
		modelStack.Scale(43, 32, 0);
		RenderMesh(meshList[GEO_STARTMENU_EXIT], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(76, 68 + loseBombPosition, 1);
		modelStack.Scale(10, 10, 0);
		RenderMesh(meshList[GEO_NORMALBOMB], false);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(25, 68 + loseBombPosition, 1);
		modelStack.Scale(10, 10, 0);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_NORMALBOMB], false);
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


	modelStack.PushMatrix(); //grid system
	{
		modelStack.Scale(100. / mapSize, 100. / mapSize, 1);
		modelStack.Translate(.5, .5, 0);

		modelStack.PushMatrix(); //map
		{
			for (short y = 0; y < mapSize; ++y)
			{
				for (short x = 0; x < mapSize; ++x)
				{
					if (theMap[x][y])
						RenderMesh(meshList[GEO_MAZEWALL], false);
					//else RenderMesh(meshList[floor], false);
					modelStack.Translate(1, 0, 0);
					//if (playerInfo->pos.x == x && playerInfo->pos.y == y)
						//RenderMesh(meshList[GEO_PLAYER], false);
				}
				modelStack.Translate(-mapSize, 1, 0);
			}
		}
		modelStack.PopMatrix(); ///map

		//Render Aliens
		if (!alienManager.empty())  //Checks if alien manager vector is empty
		{
			for (int currentAlien = 0; currentAlien < alienManager.size(); ++currentAlien)
			{
				if (alienManager[currentAlien]->active)
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

	modelStack.PushMatrix();
	modelStack.Translate(38, 75, 0);
	modelStack.Scale(25, 25, 0);
	RenderMesh(meshList[GEO_HOUSE], false);
	modelStack.PopMatrix();

	renderUI();  //Main Game UI
	if (gameState == WAVE_END_STATE)
	{
		renderShopScreen();
	}
	else if (gameState == LOSE_STATE)
	{
		RenderLoseUI();
	}

	if (isPaused == true)  //Pause Menu
	{
		RenderPauseUI();
	}

	std::ostringstream ss;
	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "x" << playerInfo->playerInventory[0]->getBombAmount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 5, 142.f, 47.5f);

	ss.str("");
	ss.precision(5);
	ss << "x" << playerInfo->playerInventory[1]->getBombAmount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 5, 142.f, 32.5f);

	ss.str("");
	ss.precision(5);
	ss << "x" << playerInfo->playerInventory[2]->getBombAmount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 5, 142.f, 17.5f);

	ss.str("");
	ss.precision(5);
	ss << playerInfo->getPlayerHealth() << "/" << playerInfo->getMaxPlayerHealth();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 5, 129.f, 72.5f);
	
	ss.str("");
	ss.precision(5);
	ss << houseHealth->houseHealth << "/" << 250;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 5, 129.f, 87.5f);
	
	ss.str("");
	ss.precision(5);
	ss << playerInfo->getEquipmentCurrency();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 5, 145.f, 63.f);

	ss.str("");
	ss.precision(5);
	ss << "Player - X: " << playerInfo->pos.x << " Y:" <<playerInfo->pos.y;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 5, 0, 0);
	
	glEnable(GL_DEPTH_TEST);
	SceneManager::instance()->State(SceneManager::SCENE_MAINGAME);
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

	alienBase::spawnPosition.clear();
}