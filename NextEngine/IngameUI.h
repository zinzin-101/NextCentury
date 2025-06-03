// IngameUI.h
#pragma once

#include <list>
#include <vector>
#include "DrawableObject.h"
#include "PlayerObject.h"
#include "Button.h"
#include "SimpleObject.h"
#include "InputManager.h"

class IngameUI {
private:
    TexturedObject* healthBar = nullptr;
    SimpleObject* healthBarFill = nullptr;
    TexturedObject* staminaBar = nullptr;
    SimpleObject* staminaBarFill = nullptr;

    PlayerObject* playerObject;

    SimpleObject* deathBlackdrop = nullptr;
    TexturedObject* deathText = nullptr;
    std::vector<Button*> deathmenubuttons;

    std::vector<Button*> pausemenubuttons;

    TexturedObject* arrow = nullptr;
    int             selectedButtonIndex = 0;

    int                       MAX_BULLETS = 4;
    std::vector<TexturedObject*> gunIcons;
    glm::vec3                 baseGunScale;

    glm::vec3 camPos;

    bool isPaused = false;
    void updateArrowPosition();

public:
    void initUI(std::list<DrawableObject*>& objectsList, PlayerObject* playerObject);
    void updateUI();

    void handleInput(InputManager& input);

    ~IngameUI();
};
