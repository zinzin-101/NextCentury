#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class LevelAct6 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    FadeBlack* fb;
    ChatBubble* chat1;
    ChatBubble* chat2;
    ProtagThoughts* thought1;
    SimpleObject* camTarget;
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