#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class LevelAct11 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    
    InteractableObject* ishelf;
    InteractableObject* medicine;
    ProtagThoughts* p1;
    

    bool isStop;
    FadeBlack* fb;
    bool end = false;
    bool once = false;
    float timefade = 10.0f;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};