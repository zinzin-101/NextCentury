#include "UI.h"
#include <iostream>
#include <SDL.h>
#include "Level.h"

UI::UI() {
    buttons.resize(5);
    selectedButtonIndex = 0;
}

UI::~UI() {
    if (settingsUI) {
        delete settingsUI;
        settingsUI = nullptr;
    }
}

void UI::initUI(std::list<DrawableObject*>& objectsList) {
    sharedObjectsList = &objectsList;

    TexturedObject* background = new TexturedObject();
    background->setTexture("../Resource/Texture/UI/MainMenu/MainmenuArt.png");
    background->getTransform().setScale(glm::vec3(17.77777777777f, 10.0f, 0.0f));
    objectsList.push_back(background);
    background->setRenderOrder(20);

    TexturedObject* title = new TexturedObject("title");
    title->setTexture("../Resource/Texture/UI/MainMenu/NextCenturyTitle.png");
    title->getTransform().setScale(glm::vec3(14.22222222222222f, 8.0f, 0.0f));
    title->getTransform().setPosition(glm::vec3(-1.0f, 0.5f, 0.0f));
    objectsList.push_back(title);
    title->setRenderOrder(21);

    arrow = new TexturedObject("arrow");
    arrow->setTexture("../Resource/Texture/UI/MainMenu/Arrow.png");
    arrow->getTransform().setScale(glm::vec3(0.213f, 0.213f, 0.0f));
    arrow->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    objectsList.push_back(arrow);
    arrow->setRenderOrder(22);

    buttons[0] = new Button("ContinueButton", "");
    buttons[0]->setTexture("../Resource/Texture/UI/MainMenu/ContinueButton.png");
    buttons[0]->getTransform().setScale(glm::vec3(1.5f, 0.27f, 0.0f));
    buttons[0]->getTransform().setPosition(glm::vec3(-5.0f, 1.5f, 0.0f));
    buttons[0]->setOnClickCallback([]() {
        GameState last = Level::getLastGameStateData();
        if (last != GameState::GS_NONE) {
            GameEngine::getInstance()->getStateController()->gameStateNext = last;
        }
        else {
            std::cout << "No saved game found; cannot continue.\n";
        }
        });
    buttons[0]->setRenderOrder(27);
    objectsList.push_back(buttons[0]);

    buttons[1] = new Button("PlayGameButton", "");
    buttons[1]->setTexture("../Resource/Texture/UI/MainMenu/PlayGame.png");
    buttons[1]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[1]->getTransform().setPosition(glm::vec3(-5.0f, 0.5f, 0.0f));
    buttons[1]->setOnClickCallback([]() {
		GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_ACT1;
        });
    buttons[1]->setRenderOrder(24);
    objectsList.push_back(buttons[1]);
    

    buttons[2] = new Button("SettingButton", ""); 
    buttons[2]->setTexture("../Resource/Texture/UI/MainMenu/Setting.png");
    buttons[2]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[2]->getTransform().setPosition(glm::vec3(-5.0f, -0.5f, 0.0f));
    buttons[2]->setOnClickCallback([this]() {
        this->inSettings = true;
        std::cout << "Switched to Settings Screen\n";
        });
	buttons[2]->setRenderOrder(24);
    objectsList.push_back(buttons[2]);

    buttons[3] = new Button("CreditsButton", "");
    buttons[3]->setTexture("../Resource/Texture/UI/MainMenu/Credits.png");
    buttons[3]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[3]->getTransform().setPosition(glm::vec3(-5.0f, -1.5f, 0.0f));
    buttons[3]->setOnClickCallback([]() {
        GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_CREDIT;
        });
    buttons[3]->setRenderOrder(25);
    objectsList.push_back(buttons[3]);

    buttons[4] = new Button("QuitGameButton", "");
    buttons[4]->setTexture("../Resource/Texture/UI/MainMenu/QuitGame.png");
    buttons[4]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[4]->getTransform().setPosition(glm::vec3(-5.0f, -2.5f, 0.0f));
    buttons[4]->setOnClickCallback([]() {
		GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_QUIT;
        });
    buttons[4]->setRenderOrder(26);
    objectsList.push_back(buttons[4]);
    
    
    

    settingsUI = new UISetting();
    settingsUI->initUI(objectsList);
    settingsUI->hideAllSettings();

    buttons[0]->setFocused(true);
    updateArrowPosition();


}

void UI::updateArrowPosition() {
    Transform& bt = buttons[selectedButtonIndex]->getTransform();
    glm::vec3 bpos = bt.getPosition();
    glm::vec3 bscale = bt.getScale();

    glm::vec3 ascale = arrow->getTransform().getScale();
    const float margin = 0.1f;

    float arrowX =
        bpos.x
        - (bscale.x * 0.5f)
        - (ascale.x * 0.5f)
        - margin;

    arrow->getTransform().setPosition({ arrowX, bpos.y, bpos.z });
}

void UI::updateUI() {
    if (inSettings && settingsUI) {
        settingsUI->updateUI();
        return;
    }

    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i]->setFocused(int(i) == selectedButtonIndex);
    }

    updateArrowPosition();
}

void UI::handleInput(SDL_Keycode key) {
    if (inSettings && settingsUI) {
        settingsUI->handleInput(key);
        if (key == SDLK_ESCAPE) {
            settingsUI->hideAllSettings();
            inSettings = false;
        }
        return;
    }

    if (buttons.empty()) return;

    switch (key) {
    case SDLK_w:
        GameEngine::getInstance()->getAudioEngine().playSoundEffectByName("hover.wav");
        buttons[selectedButtonIndex]->setState(Button::ButtonState::NORMAL);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setState(Button::ButtonState::HOVERED);
        updateArrowPosition();
        break;

    case SDLK_s:
        GameEngine::getInstance()->getAudioEngine().playSoundEffectByName("hover.wav");
        buttons[selectedButtonIndex]->setState(Button::ButtonState::NORMAL);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setState(Button::ButtonState::HOVERED);
        updateArrowPosition();
        break;

    case SDLK_RETURN:
    case SDLK_SPACE:
        buttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, true);
        buttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, false);
        break;

    default:
        break;
    }
}

void UI::handleInput(char key) {
    if (inSettings && settingsUI) {
        SDL_Keycode kc = 0;
        switch (key) {
        case 'w': kc = SDLK_w; break;
        case 's': kc = SDLK_s; break;
        case 'a': kc = SDLK_a; break;
        case 'd': kc = SDLK_d; break;
        case '\t': kc = SDLK_TAB; break;
        case '\r':
        case ' ': kc = SDLK_RETURN; break;
        case 27:  kc = SDLK_ESCAPE; break;
        default:  kc = 0; break;
        }
        settingsUI->handleInput(key);
        if (key == SDLK_ESCAPE) {
            settingsUI->hideAllSettings();
            inSettings = false;
        }

        return;
    }

    if (buttons.empty()) {
        std::cout << "buttons is empty" << std::endl;
        return;
    }

    if (key == 'a') {
		GameEngine::getInstance()->getAudioEngine().playSoundEffectByName("hover.wav");
        buttons[selectedButtonIndex]->setFocused(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
        updateArrowPosition();
    }
    else if (key == 'd') {
        GameEngine::getInstance()->getAudioEngine().playSoundEffectByName("hover.wav");
        buttons[selectedButtonIndex]->setFocused(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
        updateArrowPosition();
    }
    else if (key == ' ' || key == '\r') {
        buttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, true);
        buttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, false);
    }
}


