#pragma once
#include "Level.h"
#include "ObjectHeader.h"

class LevelAct8 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    FadeBlack* fb;

    bool end = false;
    float timefade = 1.0f;
    bool isStop = false;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};