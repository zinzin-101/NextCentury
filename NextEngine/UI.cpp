#include "UI.h"

UI::UI() {

}

void UI::initUI(list<DrawableObject*> &objectsList) {
    healthBar = new SimpleObject();
    healthBar->setColor(1.0f, 0.0f, 0.0f);
    healthBar->getTransform().setScale(glm::vec3(2.0f, 0.2f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(0.0f,0.0f,0.0f));
	objectsList.push_back(healthBar);
}

void UI::updateUI(PlayerObject& playerObject){
    glm::vec3 playerPosition = playerObject.getTransform().getPosition();
    float healthPercentage = static_cast<float>(playerObject.getHealth()) / 100;
    float healthBarWidth = healthPercentage * 2.0f;
    healthBar->getTransform().setScale(glm::vec3(healthBarWidth, 0.2f, 0.0f));
    float offsetX = (2.0f - healthBarWidth) / 2.0f;
    healthBar->getTransform().setPosition(glm::vec3(playerPosition.x - offsetX, playerPosition.y + 1.5f, 0.0f));
}