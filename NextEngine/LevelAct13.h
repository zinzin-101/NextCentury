#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class LevelAct13 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;

    InteractableObject* door;
    InteractableObject* com;
    ProtagThoughts* repeatNotDone;
    bool isStop;
    FadeBlack* fb;
    bool end = false;
    float timefade = 1.0f;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};