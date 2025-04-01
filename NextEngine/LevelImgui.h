#pragma once
#include "Level.h"
#include "MapLoader.h"
#include "EnemyInfo.h"
#include "PlayerInfo.h"

class LevelImgui : public Level {
private:
    list<DrawableObject*> objectsList;
    PlayerObject* player = nullptr;
    SimpleObject* marker;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};