#pragma once

#include <list>
#include <vector>
#include <string>
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

    TexturedObject* potionIcon0 = nullptr;
    TexturedObject* potionIcon1 = nullptr;
    TexturedObject* potionIcon2 = nullptr;
    TexturedObject* potionIcon3 = nullptr;

    SimpleObject* deathBlackdrop = nullptr;
    TexturedObject* deathText = nullptr;
    std::vector<Button*> deathmenubuttons;
    std::vector<Button*> pausemenubuttons;
	TexturedObject* pauseText = nullptr;

    TexturedObject* arrow = nullptr;
    int             selectedButtonIndex = 0;

    int             MAX_BULLETS = 4;
    TexturedObject* gunIcons = nullptr;
    TexturedObject* ammoIcon = nullptr;
    std::string     currentAmmoState = "__none__";

    glm::vec3 camPos;
    bool      isPaused = false;
    bool hideGunUI = false;
    bool hidePotionUI = false;
    bool isdeathMenuDeactivate = false;

    glm::vec3 healthBarBasePos = { -5.5f, -4.0f, 0.0f };
    glm::vec3 healthBarBaseScale = { 2.0f,  (2.0f / 14.0f), 0.0f };

    glm::vec3 healthBarFillBasePos = healthBarBasePos;
    glm::vec3 healthBarFillBaseScale = { 2.0f,  (2.0f / 14.0f) * 0.8f, 0.0f };

    glm::vec3 staminaBarBasePos = { -5.75f, -3.85f, 0.0f };
    glm::vec3 staminaBarBaseScale = { 1.0f * 1.5f,  (7.0f / 94.0f) * 1.5f, 0.0f };

    glm::vec3 staminaBarFillBasePos = staminaBarBasePos;
    glm::vec3 staminaBarFillBaseScale = { (1.0f * 1.5f) * 0.8f,  ((7.0f / 94.0f) * 1.5f) * 0.8f, 0.0f };

    glm::vec3 potionIconBasePos = { -7.0f, -3.75f, 0.0f };
    glm::vec3 potionIconBaseScale = { 0.7f,  0.7f, 0.0f };

    glm::vec3 ammoIconBasePos = { -5.5f, -3.525f, 0.0f };
    glm::vec3 ammoIconBaseScale = { 1.0f*0.75f,  (12.0f/29.0f)*0.75f, 0.0f };

    glm::vec3 gunIconsBasePos = { -6.25f, -3.525f, 0.0f }; 
    glm::vec3 gunIconsBaseScale = { 1.0f * 0.4f,  (15.0f/21.0f) * 0.4f, 0.0f };

    glm::vec3 deathBlackdropBasePos = { 0.0f,  0.0f, 0.0f };
    glm::vec3 deathBlackdropBaseScale = { 20.0f,  20.0f, 0.0f };

    glm::vec3 deathTextBasePos = { 0.0f,  0.0f, 0.0f };
    glm::vec3 deathTextBaseScale = { 0.0f,  0.0f, 0.0f };

    glm::vec3 arrowBasePos = { 0.0f,  0.0f, 0.0f };
    glm::vec3 arrowBaseScale = { 0.213f, 0.213f, 0.0f };

	glm::vec3 pauseTextBasePos = { 0.0f,  2.5f, 0.0f };
    glm::vec3 pauseTextBaseScale = { 1.0f * 5.0f,(160.0f/640.0f)*5.0f, 0.0f };

    void updateArrowPosition(PlayerObject* playerObject);
    void hideAllUI();
    void updateHealthStaminaUI(PlayerObject*);
    void updateAmmoUI(PlayerObject*);
    void updatePotionUI(PlayerObject*);
    void showPauseMenu();
    void showDeathMenu(PlayerObject*);

public:
    void initUI(std::list<DrawableObject*>& objectsList);
    void updateUI(PlayerObject* playerObject);
    void handleInput(InputManager& input, PlayerObject* playerObject);
    ~IngameUI();
    void setDeathMenuDeactivate(bool deactivate) {
        isdeathMenuDeactivate = deactivate;
        if (deactivate) {
            deathBlackdrop->getTransform().setScale({ 0,0,0 });
            deathText->getTransform().setScale({ 0,0,0 });
            for (auto* btn : deathmenubuttons) {
                btn->getTransform().setScale({ 0,0,0 });
            }
            arrow->getTransform().setScale({ 0,0,0 });
        }
    }
    void setDeactivateGunUI(bool deactivate) {
        hideGunUI = deactivate;
        if (deactivate) {
            gunIcons->getTransform().setScale({ 0,0,0 });
            ammoIcon->getTransform().setScale({ 0,0,0 });
        }
    }

    void setDeactivatePotionUI(bool deactivate) {
        hidePotionUI = deactivate;
        if (deactivate) {
            potionIcon0->getTransform().setScale({ 0,0,0 });
            potionIcon1->getTransform().setScale({ 0,0,0 });
            potionIcon2->getTransform().setScale({ 0,0,0 });
            potionIcon3->getTransform().setScale({ 0,0,0 });
        }
    }
};
