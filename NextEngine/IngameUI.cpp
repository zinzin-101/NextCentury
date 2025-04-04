#include "IngameUI.h"

void IngameUI::initUI(std::list<DrawableObject*>& objectsList) {
    healthBar = new SimpleObject();
    healthBar->setColor(1.0f, 0.0f, 0.0f);
    healthBar->getTransform().setScale(glm::vec3(200.0f, 30.0f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    objectsList.push_back(healthBar);

    staminaBar = new SimpleObject();
    staminaBar->setColor(0.0f, 0.0f, 1.0f);
    staminaBar->getTransform().setScale(glm::vec3(200.0f, 30.0f, 0.0f));
    staminaBar->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    objectsList.push_back(staminaBar);
}

void IngameUI::updateUI(PlayerObject& playerObject, glm::vec3 camPos) {
    float healthPercentage = static_cast<float>(playerObject.getHealth()) / 100.0f;
    float fullBarWidth = 2.0f; 
    float healthBarWidth = healthPercentage * fullBarWidth;

    glm::vec3 playerPos = playerObject.getTransform().getPosition();
    float leftAlignedXH = playerPos.x - (fullBarWidth / 2.0f) + (healthBarWidth / 2.0f);

    healthBar->getTransform().setScale(glm::vec3(healthBarWidth, 0.2f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(leftAlignedXH, playerPos.y + 0.3f, 0.0f));

    float staminaPercentage = static_cast<float>(playerObject.getStamina()) / 100.0f;
    float staminaBarWidth = staminaPercentage * fullBarWidth;
    float leftAlignedXS = playerPos.x - (fullBarWidth / 2.0f) + (healthBarWidth / 2.0f);
    staminaBar->getTransform().setScale(glm::vec3(staminaBarWidth, 0.2f, 0.0f));
    staminaBar->getTransform().setPosition(glm::vec3(leftAlignedXS, playerPos.y + 0.1f, 0.0f));
}