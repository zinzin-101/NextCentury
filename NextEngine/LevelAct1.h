#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class LevelAct1 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    queue<Dialogue*> dialogueList;
    list<InteractableObject*> interactableList;
    InteractableObject* it = nullptr;
    ProtagThoughts* p1 = nullptr;
    ProtagThoughts* p2 = nullptr;
    int interactCount = 0;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};