#pragma once
#include <vector>
#include <list>
#include "Button.h"
#include "GameEngine.h"
#include "InputManager.h"
#include "SimpleObject.h"
#include "PlayerObject.h"
#include "IngameUI.h" 

class UI {
public:
    UI();
    void initUI(std::list<DrawableObject*>& objectsList);
    void updateUI(PlayerObject& playerObject, glm::vec3 camPos);
    void handleInput(char key);
    void handleInput(SDL_Keycode key);
    void updateArrowPosition();
    ~UI();

private:
    std::vector<Button*> buttons;
    SimpleObject* healthBar;
    int selectedButtonIndex;
    IngameUI ingameUI; 
    TexturedObject* arrow;
};
