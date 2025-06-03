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
#include "IngameUI.h"
#include "InteractableObject.h"
#include "Dialogue.h"
#include "GameStateList.h"
#include "ProtagThoughts.h"
#include "GotItemText.h"
#include "ChatBubble.h"
#include "FadeBlack.h"

namespace LevelConstant {
    constexpr float DEFAULT_BUFFER_DURATION = 0.3f;
}

class Level
{
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player;
    IngameUI* UIobject;
	SimpleObject blackLoadingScreen;

protected:
    map<SDL_Keycode, float> keyHeldDuration;
    map<SDL_Keycode, float> keyBuffer;

    map<unsigned int, float> mouseHeldDuration;
    map<unsigned int, float> mouseBuffer;

    static void appendEnemyData(std::ofstream& output, EnemyObject* enemy);
    static void readEnemyData(std::ifstream& file, std::string type, std::list<DrawableObject*>& objectsList, bool setDrawCollider);

public:
    //SimpleObject* healthBar = nullptr;
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();
    void loadNextLevel();

    virtual void signalFromEngine();

    virtual void handleKey(char key);
    virtual void handleKey(InputManager& input);
    void processHeldKey(InputManager& input, SDL_Keycode key);
    void processKeyBuffer(InputManager& input, SDL_Keycode key);
    void processHeldMouse(InputManager& input, unsigned int mouse);
    void processMouseBuffer(InputManager& input, unsigned int mouse);
    bool isKeyInBuffer(SDL_Keycode key) const;
    bool isMouseInBuffer(unsigned int mouse) const;
    void clearKeyBuffer(SDL_Keycode key);
    void clearMouseBuffer(unsigned int mouse);
    virtual void handleMouse(int type, int x, int y);
    virtual void handleAnalogStick(int type, float amount);

    virtual void startObjects(list<DrawableObject*>& objectsList);
    virtual void updateObjects(list<DrawableObject*>& objectsList);

    virtual void initPlayer(PlayerObject*& player, PlayerInfo playerInfo);

    #ifdef DEBUG_MODE_ON
    static void drawImGui(std::list<DrawableObject*>& objectsList);
    #endif
    static void exportTransformData(std::list<DrawableObject*>& objectsList, std::string fileName);
    static void importTransformData(std::list<DrawableObject*>& objectsList, std::string fileName, bool drawOutline);

    static void saveCurrentGameState();
    static GameState getLastGameStateData();
    static void resetGameStateSave();

	virtual void addLoadingScreen(std::list<DrawableObject*>& objectsList);
	virtual void removeLoadingScreen(std::list<DrawableObject*>& objectsList);
	virtual void LoadContent();
};