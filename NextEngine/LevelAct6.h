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
    InteractableObject* door;
    ParallaxObject* doorKeepTrack;
    bool isStop = false;
    bool set1FightDone = false;
    bool set2FightDone = false;
    float turnTime = 0.8f;
    ColliderObject* set1Block;
    list<EnemyObject*> enemSet1;
    list<EnemyObject*> enemSet2;

    bool end;
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