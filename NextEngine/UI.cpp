#include "UI.h"
#include <iostream>
#include <SDL.h>  // Include SDL for key input

UI::UI() {
    buttons.resize(4); // Set the correct size
    selectedButtonIndex = 0; // Start with the first button selected
}

void UI::initUI(std::list<DrawableObject*>& objectsList) {
    healthBar = new SimpleObject();
    healthBar->setColor(1.0f, 0.0f, 0.0f);
    healthBar->getTransform().setScale(glm::vec3(2.0f, 0.2f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    objectsList.push_back(healthBar);

    // First button
    buttons[0] = new Button("Button One", "Button One");
    buttons[0]->getTransform().setScale(glm::vec3(1.5f, 1.5f, 0.0f));
    buttons[0]->getTransform().setPosition(glm::vec3(-2.0f, 2.0f, 0.0f));
    buttons[0]->setOnClickCallback([]() {
        std::cout << "Button One clicked!" << std::endl;
        });
    objectsList.push_back(buttons[0]);

    // Second button
    buttons[1] = new Button("Button Two", "Button Two"); 
    buttons[1]->getTransform().setScale(glm::vec3(1.5f, 1.5f, 0.0f));
    buttons[1]->getTransform().setPosition(glm::vec3(2.0f, 2.0f, 0.0f));
    buttons[1]->setOnClickCallback([]() {
        std::cout << "Button Two clicked!" << std::endl;
        });
    objectsList.push_back(buttons[1]);

    buttons[2] = new Button("Button Three", "Button Three"); 
    buttons[2]->getTransform().setScale(glm::vec3(1.5f, 1.5f, 0.0f));
    buttons[2]->getTransform().setPosition(glm::vec3(-2.0f, -2.0f, 0.0f));
    buttons[2]->setOnClickCallback([]() {
        std::cout << "Button Three clicked!" << std::endl;
        });
    objectsList.push_back(buttons[2]);

    buttons[3] = new Button("Button Four", "Button Four"); 
    buttons[3]->getTransform().setScale(glm::vec3(1.5f, 1.5f, 0.0f));
    buttons[3]->getTransform().setPosition(glm::vec3(2.0f, -2.0f, 0.0f));
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

    // Debugging key press
    std::cout << "Key received in UI::handleInput: " << key << std::endl;

    if (key == SDLK_a) {
        std::cout << "Pressed A. Current Index: " << selectedButtonIndex << std::endl;

        // Reset previous button color to white before changing selection
        buttons[selectedButtonIndex]->setFocused(false);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 1.0f); // White

        // Move selection left
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();

        // Highlight new selected button
        buttons[selectedButtonIndex]->setFocused(true);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 0.0f); // Yellow

        std::cout << "New Index after A: " << selectedButtonIndex << std::endl;
    }

    if (key == SDLK_d) {
        std::cout << "Pressed D. Current Index: " << selectedButtonIndex << std::endl;

        // Reset previous button color to white before changing selection
        buttons[selectedButtonIndex]->setFocused(false);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 1.0f); // White

        // Move selection right
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();

        // Highlight new selected button
        buttons[selectedButtonIndex]->setFocused(true);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 0.0f); // Yellow

        std::cout << "New Index after D: " << selectedButtonIndex << std::endl;
    }

    if (key == SDLK_RETURN || key == SDLK_SPACE) { // Enter or Space
        buttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, true);
    }

    if (key == SDLK_RETURN || key == SDLK_SPACE) {
        buttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, false);
    }
}

void UI::handleInput(char key) {
    if (buttons.empty()) {
        std::cout << "buttons is empty" << std::endl;
        return;
    }

    // Debugging key press
    std::cout << "Key received in UI::handleInput: " << key << std::endl;

    if (key == 'a') {
        std::cout << "Pressed A. Current Index: " << selectedButtonIndex << std::endl;

        // Reset previous button color to white before changing selection
        buttons[selectedButtonIndex]->setFocused(false);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 1.0f); // White

        // Move selection left
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();

        // Highlight new selected button
        buttons[selectedButtonIndex]->setFocused(true);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 0.0f); // Yellow

        std::cout << "New Index after A: " << selectedButtonIndex << std::endl;
    }

    if (key == 'd') {
        std::cout << "Pressed D. Current Index: " << selectedButtonIndex << std::endl;

        // Reset previous button color to white before changing selection
        buttons[selectedButtonIndex]->setFocused(false);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 1.0f); // White

        // Move selection right
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();

        // Highlight new selected button
        buttons[selectedButtonIndex]->setFocused(true);
        buttons[selectedButtonIndex]->setColor(1.0f, 1.0f, 0.0f); // Yellow

        std::cout << "New Index after D: " << selectedButtonIndex << std::endl;
    }

    if (key == '\r' || key == ' ') { // Enter or Space
        buttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, true);
    }

    if (key == '\r' || key == ' ') {
        buttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, false);
    }
}
UI::~UI() {
    /*for (auto* obj : buttons) {
        if (obj != nullptr) {
            delete obj;
        }
    }*/ // No need to delete buttons because you deleted them in the level cpp
}

