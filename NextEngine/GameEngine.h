#pragma once
#include "DebugMode.h"

#include <list>
#include "DrawableObject.h"
#include "GLRenderer.h"
#include "GameStateController.h"
#include "Time.h"
#include "InputManager.h" 
#include <SDL.h>

using namespace std;

class GameEngine {
	static GameEngine* instance;
	int winWidth, winHeight;
	
	GLRenderer *renderer;
	GameStateController* stateController;
	Time* time;
	InputManager* inputHandler;

	SDL_Window* gameWindow;
	SDL_GLContext* glContext;

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
	void setSDLWindow(SDL_Window* window);
	void setGlContext(SDL_GLContext* glContext);
	void freezeGameForSeconds(float duration);

	SDL_Window* getSDLWindow() const;
	SDL_GLContext* getGLContextPtr() const;
};