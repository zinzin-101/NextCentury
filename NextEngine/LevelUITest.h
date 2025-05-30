#pragma once
#include "Level.h"
#include "MapLoader.h"

class LevelUITest : public Level
{
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player;
    EnemyObject* enemy;
    TexturedObject* background;
    float zoomRatio = 0.05f;
    
    SimpleObject* attackHitbox;
    bool isHitboxActive = false;
    float hitboxTimer = 0.0f;
    MapLoader mapLoader;
    UI* UIobject;
    int mouseX, mouseY;
    

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};
