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
	glm::vec3 camPos;

public:
    void initUI(std::list<DrawableObject*>& objectsList);
    void updateUI(PlayerObject& playerObject);
};
