#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class LevelAct12 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;

    ChatBubble* preFight;
    ChatBubble* postFight;
    InteractableObject* door;
    ElivaBoss* boss;

    bool isStop;
    FadeBlack* fb;
    bool end = false;
    bool once = false;
    float timefade = 1.0f;

    int killCount = 0;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);

    virtual void signalFromEngine();
};