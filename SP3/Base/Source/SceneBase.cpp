#include "SceneBase.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glDisable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//comg.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("Quad", Color(0.f, 0.f, 0.f), 1.f);
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 1, 1), 10, 10, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 2.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	//SP3
	meshList[GEO_HEALTH_BAR_GREEN] = MeshBuilder::GenerateQuad("Quad", Color(0.f, 1.f, 0.f), 1.f);
	meshList[GEO_HEALTH_BAR_RED] = MeshBuilder::GenerateQuad("Quad", Color(1.f, 0.f, 0.f), 1.f);

	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("Ground", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//ground.tga");

	meshList[GEO_MAZEWALL] = MeshBuilder::GenerateQuad("MazeWall", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_MAZEWALL]->textureID = LoadTGA("Image//mazeWall.tga");

	meshList[GEO_PLAYER] = MeshBuilder::GenerateQuad("Player", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_PLAYER]->textureID = LoadTGA("Image//player.tga");

	meshList[GEO_NORMALBOMB] = MeshBuilder::GenerateQuad("NormalBomb", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_NORMALBOMB]->textureID = LoadTGA("Image//normalBomb.tga");

	meshList[GEO_MINEBOMB] = MeshBuilder::GenerateQuad("MineBomb", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_MINEBOMB]->textureID = LoadTGA("Image//mineBomb.tga");

	meshList[GEO_NUKEBOMB] = MeshBuilder::GenerateQuad("NukeBomb", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_NUKEBOMB]->textureID = LoadTGA("Image//nukeBomb.tga");

	meshList[GEO_BOOM] = MeshBuilder::GenerateQuad("Boom", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_BOOM]->textureID = LoadTGA("Image//boom.tga");

	meshList[GEO_HOUSE] = MeshBuilder::GenerateQuad("House", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_HOUSE]->textureID = LoadTGA("Image//house.tga");

	meshList[GEO_BOMBFIRE] = MeshBuilder::GenerateQuad("BombFire", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_BOMBFIRE]->textureID = LoadTGA("Image//bombFire.tga");

	meshList[GEO_ALIENGRUB] = MeshBuilder::GenerateQuad("AlienGrub", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_ALIENGRUB]->textureID = LoadTGA("Image//grub.tga");

	meshList[GEO_ALIENGHOUL] = MeshBuilder::GenerateQuad("AlienGhoul", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_ALIENGHOUL]->textureID = LoadTGA("Image//ghoul.tga");

	meshList[GEO_ALIENRAPTOR] = MeshBuilder::GenerateQuad("Raptor", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_ALIENRAPTOR]->textureID = LoadTGA("Image//raptor.tga");

	meshList[GEO_ALIENGOLIATH] = MeshBuilder::GenerateQuad("AlienGoliath", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_ALIENGOLIATH]->textureID = LoadTGA("Image//goliath.tga");

	meshList[GEO_ALIENLEVIATHAN] = MeshBuilder::GenerateQuad("AlienLeviathan", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_ALIENLEVIATHAN]->textureID = LoadTGA("Image//leviathan.tga");

	meshList[GEO_WALL_LVL1] = MeshBuilder::GenerateQuad("Wall_LVL1", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_WALL_LVL1]->textureID = LoadTGA("Image//wall_LVL1.tga");

	meshList[GEO_WALL_LVL2] = MeshBuilder::GenerateQuad("Wall_LVL2", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_WALL_LVL2]->textureID = LoadTGA("Image//wall_LVL2.tga");

	meshList[GEO_WALL_LVL3] = MeshBuilder::GenerateQuad("Wall_LVL3", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_WALL_LVL3]->textureID = LoadTGA("Image//wall_LVL3.tga");

	meshList[GEO_WALL_LVL4] = MeshBuilder::GenerateQuad("Wall_LVL4", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_WALL_LVL4]->textureID = LoadTGA("Image//wall_LVL4.tga");

	meshList[GEO_WALL_LVL5] = MeshBuilder::GenerateQuad("Wall_LVL5", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_WALL_LVL5]->textureID = LoadTGA("Image//wall_LVL5.tga");

	meshList[GEO_STARTMENU_BACKGROUND] = MeshBuilder::GenerateQuad("StartMenu_Background", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_STARTMENU_BACKGROUND]->textureID = LoadTGA("Image//StartMenu_Background.tga");

	meshList[GEO_STARTMENU_TITLE] = MeshBuilder::GenerateQuad("StartMenu_Title", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_STARTMENU_TITLE]->textureID = LoadTGA("Image//StartMenu_Title.tga");

	meshList[GEO_STARTMENU_STARTGAME] = MeshBuilder::GenerateQuad("StartMenu_StartGame", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_STARTMENU_STARTGAME]->textureID = LoadTGA("Image//StartMenu_StartGame.tga");

	meshList[GEO_STARTMENU_SETTINGS] = MeshBuilder::GenerateQuad("StartMenu_Settings", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_STARTMENU_SETTINGS]->textureID = LoadTGA("Image//StartMenu_Settings.tga");

	meshList[GEO_STARTMENU_EXIT] = MeshBuilder::GenerateQuad("StartMenu_Exit", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_STARTMENU_EXIT]->textureID = LoadTGA("Image//StartMenu_Exit.tga");

	meshList[GEO_STARTMENU_INSTRUCTIONS] = MeshBuilder::GenerateQuad("StartMenu_Instructions", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_STARTMENU_INSTRUCTIONS]->textureID = LoadTGA("Image//instructions.tga");

	meshList[GEO_LEFTARROW] = MeshBuilder::GenerateQuad("StartMenu_LeftArrow", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_LEFTARROW]->textureID = LoadTGA("Image//StartMenu_LeftArrow.tga");

	meshList[GEO_RIGHTARROW] = MeshBuilder::GenerateQuad("StartMenu_RightArrow", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_RIGHTARROW]->textureID = LoadTGA("Image//StartMenu_RightArrow.tga");

	meshList[GEO_LOOTCRATE] = MeshBuilder::GenerateQuad("LootCrate", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_LOOTCRATE]->textureID = LoadTGA("Image//LootCrate.tga");

	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("Health", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_HEALTH]->textureID = LoadTGA("Image//Health.tga");

	meshList[GEO_POWERUP_SPEED] = MeshBuilder::GenerateQuad("Speed Powerup", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_POWERUP_SPEED]->textureID = LoadTGA("Image//speedBoost.tga");

	meshList[GEO_EQUIPMENT] = MeshBuilder::GenerateQuad("Equipment Currency", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_EQUIPMENT]->textureID = LoadTGA("Image//equipmentCurrency.tga");

	meshList[GEO_POWERUP_INVULNERABLE] = MeshBuilder::GenerateQuad("Invulnerable powerup", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_POWERUP_INVULNERABLE]->textureID = LoadTGA("Image//invulnerable.tga");

	meshList[GEO_BOMBSELECTOR] = MeshBuilder::GenerateQuad("Bomb Selector", Color(1.f, 0.549f, 0.f), 1.f);

	meshList[GEO_SOUND] = MeshBuilder::GenerateQuad("Sound", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_SOUND]->textureID = LoadTGA("Image//sound.tga");

	meshList[GEO_STOP] = MeshBuilder::GenerateQuad("Stop", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_STOP]->textureID = LoadTGA("Image//stop.tga");

	meshList[GEO_RESTART] = MeshBuilder::GenerateQuad("Restart", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_RESTART]->textureID = LoadTGA("Image//restart.tga");

	meshList[GEO_EXIT_MAINMENU] = MeshBuilder::GenerateQuad("ExitMainMenu", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_EXIT_MAINMENU]->textureID = LoadTGA("Image//MainMenuButton.tga");

	meshList[GEO_REPAIR] = MeshBuilder::GenerateQuad("Repair", Color(1.f, 0.f, 0.f), 1.f);
	meshList[GEO_REPAIR]->textureID = LoadTGA("Image//repair.tga");

	bLightEnabled = false;
}

void SceneBase::Update(double dt)
{
	//Keyboard Section
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	fps = (float)(1.f / dt);
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
