#include "UI.h"
#include <iostream>
#include <SDL.h>  // Include SDL for key input
UI::UI() {
    buttons.resize(4); // Set the correct size
    selectedButtonIndex = 0; // Start with the first button selected
}

void UI::initUI(std::list<DrawableObject*>& objectsList) {

    SimpleObject* background = new SimpleObject();
    background->getTransform().setScale(glm::vec3(800.0f, 800.0f, 0.0f));
    background->setColor(glm::vec4(0.f, 0.f, 0.f, 1.0f));
    objectsList.push_back(background);

    TexturedObject* title = new TexturedObject("title");
    title->setTexture("../Resource/Texture/UI/MainMenu/NextCenturyTitle.png");
    title->getTransform().setScale(glm::vec3(14.22222222222222f, 8.0f, 0.0f));
    title->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
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

    // First button
    buttons[0] = new Button("PlayGameButton", "");           
    buttons[0]->setTexture("../Resource/Texture/UI/MainMenu/PlayGame.png");
    buttons[0]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[0]->getTransform().setPosition(glm::vec3(-3.0f, 1.0f, 0.0f));
    buttons[0]->setOnClickCallback([]() {
        std::cout << "Start Button clicked!" << std::endl;
        });
    objectsList.push_back(buttons[0]);

    // Second button
    buttons[1] = new Button("SettingButton", "");
    buttons[1]->setTexture("../Resource/Texture/UI/MainMenu/Setting.png");
    buttons[1]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[1]->getTransform().setPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
    buttons[1]->setOnClickCallback([]() {
        std::cout << "Button Two clicked!" << std::endl;
        });
    objectsList.push_back(buttons[1]);

    buttons[2] = new Button("CreditsButton", "");
    buttons[2]->setTexture("../Resource/Texture/UI/MainMenu/Credits.png");
    buttons[2]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[2]->getTransform().setPosition(glm::vec3(-3.0f, -1.0f, 0.0f));
    buttons[2]->setOnClickCallback([]() {
        std::cout << "Button Three clicked!" << std::endl;
        });
    objectsList.push_back(buttons[2]);

    buttons[3] = new Button("QuitGameButton", "");
    buttons[3]->setTexture("../Resource/Texture/UI/MainMenu/QuitGame.png");
    buttons[3]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[3]->getTransform().setPosition(glm::vec3(-3.0f, -2.0f, 0.0f));
    buttons[3]->setOnClickCallback([]() {
        std::cout << "Button Four clicked!" << std::endl;
        });
    objectsList.push_back(buttons[3]);

    buttons[0]->setFocused(true);
    updateArrowPosition();
    //ingameUI.initUI(objectsList);
}

void UI::updateArrowPosition() {
    // get the focused button's transform
    Transform& bt = buttons[selectedButtonIndex]->getTransform();
    glm::vec3 bpos = bt.getPosition();
    glm::vec3 bscale = bt.getScale();

    // arrow scale and margin
    glm::vec3 ascale = arrow->getTransform().getScale();
    const float margin = 0.1f;

    // compute arrow X so it's just to the left of button
    float arrowX = bpos.x
        - (bscale.x * 0.5f)     // half button width
        - (ascale.x * 0.5f)     // half arrow width
        - margin;

    // match arrow Y and Z to button
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

    case SDLK_s:  // move selection right
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

