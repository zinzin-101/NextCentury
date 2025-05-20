#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class LevelAct3 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    InteractableObject* door;
    InteractableObject* board;
    InteractableObject* shelf;
    InteractableObject* desk;
    InteractableObject* chest;
    ProtagThoughts* done;
    ProtagThoughts* repeatNotDone;
    GotItemText* medicineOnTable;
    GotItemText* fakePassport;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};