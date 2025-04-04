#include "IngameUI.h"

void IngameUI::initUI(std::list<DrawableObject*>& objectsList) {
    healthBar = new SimpleObject();
    healthBar->setColor(1.0f, 0.0f, 0.0f);
    healthBar->getTransform().setScale(glm::vec3(200.0f, 30.0f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    objectsList.push_back(healthBar);
}

void IngameUI::updateUI(PlayerObject& playerObject, glm::vec3 camPos) {
    float healthPercentage = static_cast<float>(playerObject.getHealth()) / 100;
    float healthBarWidth = healthPercentage * 2.0f;
    healthBar->getTransform().setScale(glm::vec3(healthBarWidth, 0.2f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(playerObject.getTransform().getPosition().x , playerObject.getTransform().getPosition().y +0.3f, 0.0f));

}