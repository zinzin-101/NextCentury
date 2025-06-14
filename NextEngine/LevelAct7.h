#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class LevelAct7 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    FadeBlack* fb;
    InteractableObject* door;
    InteractableObject* shelf;
    InteractableObject* board;
    InteractableObject* computer;
    GotItemText* map;
    GotItemText* medicine;
    ProtagThoughts* repeat;

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