#pragma once
#include "Level.h"
#include "GameEngine.h"
#include "SquareMeshVbo.h"
#include "SquareBorderMesh.h"
#include "SimpleObject.h"
#include "TexturedObject.h"
#include "ParallaxObject.h"

class LevelParallax : public Level
{
private:
    list<DrawableObject*> objectsList;
    SimpleObject* player;
    ParallaxObject* spaceShip;
    ParallaxObject* midGround;
    float zoomRatio = 0.05f;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(char key);
    virtual void handleMouse(int type, int x, int y);
    virtual void handleAnalogStick(int type, float amount);
};

//#pragma once
//#include "Level.h"
//#include "Audio.h"
//
//class LevelAnimTest : public Level
//{
//private:
//	vector<DrawableObject*> objectsList;
//	TexturedObject* player;
//	AudioEngine* audio;
//	int d = 0;
//	int row = 0;
//	int col = 0;
//public:
//	virtual void levelLoad();
//	virtual void levelInit();
//	virtual void levelUpdate();
//	virtual void levelDraw();
//	virtual void levelFree();
//	virtual void levelUnload();
//
//	virtual void handleKey(char key);
//	virtual void handleMouse(int type, int x, int y);
//	virtual void handleAnalogStick(int type, float amount);
//};
