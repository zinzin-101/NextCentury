#pragma once

#include <list>
#include "DrawableObject.h"
#include "GLRenderer.h"
#include "GameStateController.h"
#include "Time.h"
#include "InputManager.h" 

using namespace std;

class GameEngine {
	static GameEngine* instance;
	int winWidth, winHeight;
	
	GLRenderer *renderer;
	GameStateController* stateController;
	Time* time;
	InputManager* inputHandler;

	GameEngine();
public:
	static GameEngine* getInstance();
	GLRenderer * getRenderer();
	GameStateController* getStateController();
	Time* getTime();
	InputManager* getInputHandler();// Getter for input handler
	void init(int width, int height);
	void setDrawArea(float left, float right, float bottom, float top);
	void setBackgroundColor(float r, float g, float b);
	void addMesh(string name, MeshVbo* mesh);
	void clearMesh();
	void render(list<DrawableObject*> renderObjects, bool clear = true);
	int getWindowWidth();
	int getWindowHeight();
	void setWindowWidth(int w);
	void setWindowHeight(int h);
};