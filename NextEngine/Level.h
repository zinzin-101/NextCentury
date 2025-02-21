#pragma once
#include "GameEngine.h"
#include "InputManager.h"
#include "SquareMeshVbo.h"
#include "SquareBorderMesh.h"
#include "SimpleObject.h"
#include "TexturedObject.h"
#include "Button.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "Audio.h"
#include "ParallaxObject.h"
#include "TextObject.h"

class Level
{
private:
    list<DrawableObject*> objectsList;
    PlayerObject* player;

public:
    SimpleObject* healthBar = nullptr;
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(char key);
    virtual void handleKey(InputManager& input);
    virtual void handleMouse(int type, int x, int y);
    virtual void handleAnalogStick(int type, float amount);

    virtual void startObjects(list<DrawableObject*>& objectsList);
    virtual void updateObjects(list<DrawableObject*>& objectsList);

    virtual void initPlayer(PlayerObject*& player, PlayerInfo playerInfo);
};
