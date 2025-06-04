#pragma once
#include "Level.h"
#include "ObjectHeader.h"

class LevelAlphaTest : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);

    int counter;

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