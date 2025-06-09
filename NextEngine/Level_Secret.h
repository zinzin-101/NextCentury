#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class Level_Secret : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;

    ChatBubble* chat;

    TexturedObject* Ono;

    bool isStop;
    FadeBlack* fb;
    FadeBlack* fbO;
    bool end = false;
    bool once = false;
    bool once2 = false;
    float timefade = 0.0f;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};