#pragma once
#include "Level.h"

class LevelAlphaTest : public Level {
private:
    list<DrawableObject*> objectsList;
    PlayerObject* player = nullptr;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};