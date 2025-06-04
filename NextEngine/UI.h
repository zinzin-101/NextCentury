#pragma once

#include <vector>
#include "Button.h"
#include "GameEngine.h"
#include "InputManager.h"
#include "SimpleObject.h"
#include "PlayerObject.h"
#include "UISetting.h"       

class UI {
public:
    UI();
    void initUI(std::list<DrawableObject*>& objectsList);
    void updateUI();
    void handleInput(char key);
    void handleInput(SDL_Keycode key);
    void updateArrowPosition();
    
    ~UI();

private:
    std::vector<Button*> buttons;
    SimpleObject* healthBar;
    int            selectedButtonIndex;
    TexturedObject* arrow;

    bool inSettings = false;           
    UISetting* settingsUI = nullptr;    

    std::list<DrawableObject*>* sharedObjectsList = nullptr;

};
