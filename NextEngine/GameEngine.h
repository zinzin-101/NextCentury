#pragma once

#include "DebugMode.h"
#include <list>
#include <string>
#include <SDL.h>
#include "DrawableObject.h"
#include "GLRenderer.h"
#include "GameStateController.h"
#include "Time.h"
#include "InputManager.h"
#include "Audio.h"  

using namespace std;

class GameEngine {
    static GameEngine* instance;

    int winWidth;
    int winHeight;

	float engineTimer;
	bool isGamePaused;
	float prevTimeScale;
    GLRenderer* renderer;
    GameStateController* stateController;
    Time* time;
    InputManager* inputHandler;

    SDL_Window* gameWindow;
    SDL_GLContext* glContext;

    float engineTimer;
    bool isGamePaused;

    AudioEngine audioEngine;

    GameEngine();

public:
    static GameEngine* getInstance();

    GLRenderer* getRenderer();
    GameStateController* getStateController();
    Time* getTime();
    InputManager* getInputHandler();

    void init(int width, int height);
    void updateEngineComponent();
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
    void pauseTimeForSeconds(float duration);
    bool getIsGamePaused() const;

    SDL_Window* getSDLWindow() const;
    SDL_GLContext* getGLContextPtr() const;

    void initAudio(const std::string& effectFolder, const std::string& musicFolder);
    void playSoundEffect(const std::string& fileName, int loop = 0);
    void playMusic(const std::string& fileName, int loop = -1);
    void stopMusic();
};
