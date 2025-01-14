#pragma once

#include "Level.h"
#include "MapLoader.h"

class LevelMapEditorTest : public Level {
private:
    list<DrawableObject*> objectsList;
    PlayerObject* player;
    MapLoader mapLoader;
    TexturedObject* background;

    SimpleObject* attackHitbox;
    bool isHitboxActive = false;
    float hitboxTimer = 0.0f;

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