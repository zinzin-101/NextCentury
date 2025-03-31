#pragma once
#include "Level.h"
#include "MapLoader.h"
#include "EnemyInfo.h"
#include "PlayerInfo.h"

class LevelPrototypeNMMN : public Level {
    private:
        list<DrawableObject*> objectsList;
        PlayerObject* player;
        SimpleObject* marker;
        RayObject* ray;
        MapLoader mapLoader;
        list<DrawableObject*> UIObjects;
        bool viewMarker;


    public:
        virtual void levelLoad();
        virtual void levelInit();
        virtual void levelUpdate();
        virtual void levelDraw();
        virtual void levelFree();
        virtual void levelUnload();

        virtual void handleKey(InputManager& input);
        virtual void handleMouse(int type, int x, int y);
        virtual void handleAnalogStick(int type, float amount);

        void initPlayer(PlayerObject*& player, glm::vec3 position, PlayerInfo playerInfo);
        void initPlayer(PlayerObject*& player, PlayerInfo playerInfo);
        void instantiateEnemy(glm::vec3 position, EnemyInfo enemyInfo, EnemyType type);
};