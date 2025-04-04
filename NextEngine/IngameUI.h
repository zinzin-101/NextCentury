#pragma once
#include <list>
#include "SimpleObject.h"
#include "PlayerObject.h"

class IngameUI {
private:
    SimpleObject* healthBar = nullptr;
public:
    void initUI(std::list<DrawableObject*>& objectsList);
    void updateUI(PlayerObject& playerObject, glm::vec3 camPos);
};
