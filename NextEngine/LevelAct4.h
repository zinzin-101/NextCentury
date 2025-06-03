#pragma once

#include "Level.h"
#include "ObjectHeader.h"
#include "IngameUI.h"

class LevelAct4 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    ChatBubble* chat1;
    FadeBlack* fb;
    bool end = false;
    float timefade = 1.0f;
    bool isStop = false;
    EnemyObject* enem;
    
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