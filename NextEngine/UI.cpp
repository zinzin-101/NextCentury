#include "UI.h"
#include <iostream>
#include <SDL.h>  // Include SDL for key input

UI::UI() {
    buttons.resize(4); // Set the correct size
    selectedButtonIndex = 0; // Start with the first button selected
}

void UI::initUI(std::list<DrawableObject*>& objectsList) {
    /*healthBar = new SimpleObject();
    healthBar->setColor(1.0f, 0.0f, 0.0f);
    healthBar->getTransform().setScale(glm::vec3(2.0f, 0.2f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    objectsList.push_back(healthBar);*/

    // First button
    buttons[0] = new Button("Start", "Start");
    buttons[0]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[0]->getTransform().setPosition(glm::vec3(-2.0f, 2.0f, 0.0f));
    buttons[0]->setOnClickCallback([]() {
        std::cout << "Start Button clicked!" << std::endl;
        });
    objectsList.push_back(buttons[0]);

    // Second button
    buttons[1] = new Button("Button Two", "Button Two"); 
    buttons[1]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[1]->getTransform().setPosition(glm::vec3(-2.0f, 1.0f, 0.0f));
    buttons[1]->setOnClickCallback([]() {
        std::cout << "Button Two clicked!" << std::endl;
        });
    objectsList.push_back(buttons[1]);

    buttons[2] = new Button("Button Three", "Button Three"); 
    buttons[2]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[2]->getTransform().setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    buttons[2]->setOnClickCallback([]() {
        std::cout << "Button Three clicked!" << std::endl;
        });
    objectsList.push_back(buttons[2]);

    buttons[3] = new Button("Button Four", "Button Four"); 
    buttons[3]->getTransform().setScale(glm::vec3(1.5f, 0.5f, 0.0f));
    buttons[3]->getTransform().setPosition(glm::vec3(-2.0f, -1.0f, 0.0f));
    buttons[3]->setOnClickCallback([]() {
        std::cout << "Button Four clicked!" << std::endl;
        });
    objectsList.push_back(buttons[3]);

    buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 0.0f); // Yellow for selection
}

void UI::updateUI(PlayerObject& playerObject, glm::vec3 camPos) {
    float healthPercentage = static_cast<float>(playerObject.getHealth()) / 100;
    float healthBarWidth = healthPercentage * 2.0f;
    healthBar->getTransform().setScale(glm::vec3(healthBarWidth, 0.2f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(camPos.x - 3.5f, camPos.y - 2.5f, 0.0f));
    
}

void UI::handleInput(SDL_Keycode key) {
    if (buttons.empty()) {
        std::cout << "buttons is empty" << std::endl;
        return;
    }

    std::cout << "Key received in UI::handleInput: " << key << std::endl;

    if (key == SDLK_a) {
        // Navigate left.
        buttons[selectedButtonIndex]->setFocused(false);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 1.0f); // White
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 0.0f); // Yellow
    }
    else if (key == SDLK_d) {
        // Navigate right.
        buttons[selectedButtonIndex]->setFocused(false);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 1.0f); // White
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 0.0f); // Yellow
    }
    else if (key == SDLK_RETURN || key == SDLK_SPACE) {
        if (selectedButtonIndex == 0) {
            GameEngine::getInstance()->getStateController()->gameStateNext = (GameState)((GameEngine::getInstance()->getStateController()->gameStateCurr + 1) % 3);
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
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 1.0f);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 0.0f);
    }
    else if (key == 'd') {
        buttons[selectedButtonIndex]->setFocused(false);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 1.0f);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 0.0f);
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

