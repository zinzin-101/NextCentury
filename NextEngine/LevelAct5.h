#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class LevelAct5 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    TexturedObject* spS;
    ChatBubble* immigrats1;
    ProtagThoughts* eToPermit;
    ChatBubble* immigrats2;
    bool isStop;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};