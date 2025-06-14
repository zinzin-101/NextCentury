#pragma once

#include "Level.h"
#include "ObjectHeader.h"

class LevelAct2 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    ParallaxObject* doorKeepTrack;
    ProtagThoughts* start;
    InteractableObject* door;
    FadeBlack* fb;
    float timefade = 1.0f;
    bool end = false;

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};