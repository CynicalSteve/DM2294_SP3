#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include <vector>
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

class SceneBase : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_TEXT,
		GEO_BALL,
		GEO_CUBE,
		GEO_HEALTH_GREEN,
		GEO_HEALTH_RED,
		GEO_GROUND,
		GEO_MAZEWALL,
		GEO_PLAYER,
		GEO_NORMALBOMB,
		GEO_MINEBOMB,
		GEO_BOOM,
		GEO_HOUSE,
		GEO_BOMBFIRE,
		GEO_ALIENGRUB,
		GEO_ALIENGHOUL,
		GEO_ALIENRAPTOR,
		GEO_ALIENGOLIATH,
		GEO_ALIENLEVIATHAN,
		GEO_WALL_LVL1,
		GEO_WALL_LVL2,
		GEO_WALL_LVL3,
		GEO_WALL_LVL4,
		GEO_WALL_LVL5,
		GEO_STARTMENU_BACKGROUND,
		GEO_STARTMENU_TITLE,
		GEO_STARTMENU_STARTGAME,
		GEO_STARTMENU_SETTINGS,
		GEO_STARTMENU_EXIT,
		GEO_LOOTCRATE,
		GEO_HEALTH,

		floor,
		wall,
		ship,
		NUM_GEOMETRY,
	};
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderGO(GameObject *go);

	GameObject* FetchGO();
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;

	float fps;
};

#endif