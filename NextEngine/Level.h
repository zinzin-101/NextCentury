#pragma once
#include "GameEngine.h"
#include "InputManager.h"
#include "SquareMeshVbo.h"
#include "SquareBorderMesh.h"
#include "SimpleObject.h"
#include "TexturedObject.h"
#include "Button.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "Audio.h"
#include "ParallaxObject.h"
#include "UI.h"

class Level
{
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player;
    UI* UIobject;

protected:
    map<SDL_Keycode, float> keyHeldDuration;
    map<SDL_Keycode, int> keyBuffer;

    map<unsigned int, float> mouseHeldDuration;
    map<unsigned int, int> mouseBuffer;

public:
    //SimpleObject* healthBar = nullptr;
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(char key);
    virtual void handleKey(InputManager& input);
    void processHeldKey(InputManager& input, SDL_Keycode key);
    void processKeyBuffer(InputManager& input, SDL_Keycode key);
    void processHeldMouse(InputManager& input, unsigned int mouse);
    void processMouseBuffer(InputManager& input, unsigned int mouse);
    void clearKeyBuffer(SDL_Keycode key);
    virtual void handleMouse(int type, int x, int y);
    virtual void handleAnalogStick(int type, float amount);

    virtual void startObjects(list<DrawableObject*>& objectsList);
    virtual void updateObjects(list<DrawableObject*>& objectsList);

    virtual void initPlayer(PlayerObject*& player, PlayerInfo playerInfo);

    #ifdef DEBUG_MODE_ON
    void drawImGui(std::list<DrawableObject*>& objectsList);
    void exportTransformData(std::list<DrawableObject*>& objectsList, std::string fileName);
    void importTransformData(std::list<DrawableObject*>& objectsList, std::string fileName, bool drawOutline);
    #endif
};