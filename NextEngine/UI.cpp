#include "UI.h"
#include <iostream>
#include <SDL.h>  
UI::UI() {
    buttons.resize(4); 
    selectedButtonIndex = 0; 
}

void UI::initUI(std::list<DrawableObject*>& objectsList) {

    TexturedObject* background = new TexturedObject();
    background->setTexture("../Resource/Texture/UI/MainMenu/MainmenuArt.png");
    background->getTransform().setScale(glm::vec3(17.77777777777f, 10.0f, 0.0f));
    objectsList.push_back(background);

    TexturedObject* title = new TexturedObject("title");
    title->setTexture("../Resource/Texture/UI/MainMenu/NextCenturyTitle.png");
    title->getTransform().setScale(glm::vec3(14.22222222222222f, 8.0f, 0.0f));
    title->getTransform().setPosition(glm::vec3(-1.0f, 0.5f, 0.0f));
    objectsList.push_back(title);

    arrow = new TexturedObject("arrow");
    arrow->setTexture("../Resource/Texture/UI/MainMenu/Arrow.png");
    arrow->getTransform().setScale(glm::vec3(0.213f, 0.213f, 0.0f));
    arrow->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    objectsList.push_back(arrow);

    TexturedObject* confirm = new TexturedObject("confirm");
    confirm->setTexture("../Resource/Texture/UI/MainMenu/AConfirm_BBack.png");
    confirm->getTransform().setScale(glm::vec3(2.295652173913043, 0.3f, 0.0f));
    confirm->getTransform().setPosition(glm::vec3(0.0f, -4.0f, 0.0f));
    objectsList.push_back(confirm);

    buttons[0] = new Button("PlayGameButton", "");           
    buttons[0]->setTexture("../Resource/Texture/UI/MainMenu/PlayGame.png");
    buttons[0]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[0]->getTransform().setPosition(glm::vec3(-5.0f, 1.0f, 0.0f));
    buttons[0]->setOnClickCallback([]() {
        std::cout << "Start Button clicked!" << std::endl;
        });
    objectsList.push_back(buttons[0]);

    buttons[1] = new Button("SettingButton", "");
    buttons[1]->setTexture("../Resource/Texture/UI/MainMenu/Setting.png");
    buttons[1]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[1]->getTransform().setPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
    buttons[1]->setOnClickCallback([]() {
        std::cout << "Button Two clicked!" << std::endl;
        });
    objectsList.push_back(buttons[1]);

    buttons[2] = new Button("CreditsButton", "");
    buttons[2]->setTexture("../Resource/Texture/UI/MainMenu/Credits.png");
    buttons[2]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[2]->getTransform().setPosition(glm::vec3(-5.0f, -1.0f, 0.0f));
    buttons[2]->setOnClickCallback([]() {
        std::cout << "Button Three clicked!" << std::endl;
        });
    objectsList.push_back(buttons[2]);

    buttons[3] = new Button("QuitGameButton", "");
    buttons[3]->setTexture("../Resource/Texture/UI/MainMenu/QuitGame.png");
    buttons[3]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[3]->getTransform().setPosition(glm::vec3(-5.0f, -2.0f, 0.0f));
    buttons[3]->setOnClickCallback([]() {
        std::cout << "Button Four clicked!" << std::endl;
        });
    objectsList.push_back(buttons[3]);

    buttons[0]->setFocused(true);
    updateArrowPosition();
}

void UI::updateArrowPosition() {
    Transform& bt = buttons[selectedButtonIndex]->getTransform();
    glm::vec3 bpos = bt.getPosition();
    glm::vec3 bscale = bt.getScale();

    glm::vec3 ascale = arrow->getTransform().getScale();
    const float margin = 0.1f;

    float arrowX = bpos.x
        - (bscale.x * 0.5f)    
        - (ascale.x * 0.5f)     
        - margin;

    arrow->getTransform().setPosition({ arrowX, bpos.y, bpos.z });
}
void UI::updateUI(PlayerObject& playerObject, glm::vec3 camPos) {
    
}

void UI::handleInput(SDL_Keycode key) {
    if (buttons.empty()) return;

    switch (key) {
    case SDLK_w:
        buttons[selectedButtonIndex]->setState(Button::ButtonState::NORMAL);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setState(Button::ButtonState::HOVERED);
        updateArrowPosition();
        break;

    case SDLK_s:  
        buttons[selectedButtonIndex]->setState(Button::ButtonState::NORMAL);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setState(Button::ButtonState::HOVERED);
        updateArrowPosition();
        break;

    case SDLK_RETURN:
    case SDLK_SPACE:
    {
        if (selectedButtonIndex == 0) {
            GameEngine::getInstance()->getStateController()->gameStateNext = (GameState)((GameEngine::getInstance()->getStateController()->gameStateCurr + 1) % 3);
        }
    }
    }
}

void UI::handleInput(char key) {
    if (buttons.empty()) {
        std::cout << "buttons is empty" << std::endl;
        return;
    }

    std::cout << "Key received in UI::handleInput: " << key << std::endl;

    if (key == 'a') {
        buttons[selectedButtonIndex]->setFocused(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
    }
    else if (key == 'd') {
        buttons[selectedButtonIndex]->setFocused(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
    }
    else if (key == ' ' || key == '\r') {
        if (selectedButtonIndex == 0) {
            GameEngine::getInstance()->getStateController()->gameStateNext = (GameState)((GameEngine::getInstance()->getStateController()->gameStateCurr + 1) % 3);
        }
    }
}
UI::~UI() {
    /*for (auto* obj : buttons) {
        if (obj != nullptr) {
            delete obj;
        }
    }*/ // No need to delete buttons because you deleted them in the level cpp
}

