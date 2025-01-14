#pragma once

#include "Level.h"
#include "PlayerObject.h"
#include "EnemyObject.h"

class LevelCollisionTest : public Level {
    private:
        list<DrawableObject*> objectsList;
        PlayerObject* player;
        DrawableObject* ground;
        TexturedObject* background;

    public:
        virtual void levelLoad();
        virtual void levelInit();
        virtual void levelUpdate();
        virtual void levelDraw();
        virtual void levelFree();
        virtual void levelUnload();

        virtual void handleKey(char key);

        void handleMouse(int type, int x, int y) {} // temporary fix to disable mouse
};

void triggerTest(DrawableObject* obj1, DrawableObject* obj2);
void printCollisionState(DrawableObject* obj1, DrawableObject* obj2);