#pragma once

#include <list>
#include "DrawableObject.h"
#include "PlayerObject.h"
#include "UI.h"



class IngameUI {
private:
    TexturedObject* healthBar = nullptr;
    SimpleObject* healthBarFill = nullptr;
    TexturedObject* staminaBar = nullptr;
    SimpleObject* staminaBarFill = nullptr;
	TexturedObject* gunCooldown = nullptr;
    int MAX_BULLETS = 4;
    std::vector<TexturedObject*> gunIcons;

    
    glm::vec3 baseGunScale;
	glm::vec3 camPos;

public:
    void initUI(std::list<DrawableObject*>& objectsList);
    void updateUI(PlayerObject& playerObject);
};
