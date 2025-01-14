#pragma once
#include "Level.h"
#include "MapLoader.h"
#include "EnemyInfo.h"
#include "PlayerInfo.h"

class LevelPrototype : public Level {
    private:
        list<DrawableObject*> objectsList;
        PlayerObject* player;
        SimpleObject* marker;
        MapLoader mapLoader;

        bool viewMarker;


    public:
        virtual void levelLoad();
        virtual void levelInit();
        virtual void levelUpdate();
        virtual void levelDraw();
        virtual void levelFree();
        virtual void levelUnload();

        virtual void handleKey(char key);
        virtual void handleMouse(int type, int x, int y);
        virtual void handleAnalogStick(int type, float amount);

        void initPlayer(PlayerObject*& player, glm::vec3 position, PlayerInfo playerInfo);
        void initPlayer(PlayerObject*& player, PlayerInfo playerInfo);
        void instantiateEnemy(glm::vec3 position, EnemyInfo enemyInfo, EnemyType type);
};