#include "IngameUI.h"

void IngameUI::initUI(std::list<DrawableObject*>& objectsList) {
    healthBar = new TexturedObject();
    healthBar->setTexture("../Resource/Texture/UI/Ingame/MC_HP_Bar.png");
    objectsList.push_back(healthBar);

    healthBarFill = new SimpleObject();
    healthBarFill->setColor(glm::vec4(0.0353f, 0.7216f, 0.6706f, 1.0f));  
    objectsList.push_back(healthBarFill);

    staminaBar = new TexturedObject();
    staminaBar->setTexture("../Resource/Texture/UI/Ingame/MC_Mana_Bar.png");
    objectsList.push_back(staminaBar);

    staminaBarFill = new SimpleObject();
    staminaBarFill->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));  
    objectsList.push_back(staminaBarFill);
}

void IngameUI::updateUI(PlayerObject& playerObject) {
    camPos = GameEngine::getInstance()->getRenderer()->getCamera()->getPosition();

    float healthPercent = static_cast<float>(playerObject.getHealth()) / 100.0f;
    float staminaPercent = static_cast<float>(playerObject.getStamina()) / 100.0f;

    float fullWidthHP = 4.0f;
    float frameHeightHP = 0.40f;
    float paddingHP = 0.08f;

    float innerWidthHP = fullWidthHP - 2.0f * paddingHP;
    float innerHeightHP = frameHeightHP - 2.0f * paddingHP;

    float fullWidthST = 2.5f;
    float frameHeightST = 0.20f;
    float paddingST = 0.06f;

    float innerWidthST = fullWidthST - 2.0f * paddingST;
    float innerHeightST = frameHeightST - 2.0f * paddingST;

    float horizontalOffsetHP = -5.0f;   
    float horizontalOffsetST = -5.75f;   

    float leftEdgeX_HP = camPos.x - (fullWidthHP * 0.5f);
    float leftEdgeX_ST = camPos.x - (fullWidthST * 0.5f);

    float verticalOffsetHP = 3.80f;  
    float verticalOffsetST = 3.50f;  

    float healthY = camPos.y - verticalOffsetHP;
    float staminaY = camPos.y - verticalOffsetST;

    {
        float centerX_HP = (leftEdgeX_HP + (fullWidthHP * 0.5f)) + horizontalOffsetHP;

        healthBar->getTransform().setScale(glm::vec3(fullWidthHP, frameHeightHP, 0.0f));
        healthBar->getTransform().setPosition(glm::vec3(centerX_HP, healthY, 0.0f));
    }

    {
        float centerX_ST = (leftEdgeX_ST + (fullWidthST * 0.5f)) + horizontalOffsetST;

        staminaBar->getTransform().setScale(glm::vec3(fullWidthST, frameHeightST, 0.0f));
        staminaBar->getTransform().setPosition(glm::vec3(centerX_ST, staminaY, 0.0f));
    }

    {
        float hpFillWidth = healthPercent * innerWidthHP;
        float hpFillLeftX = (leftEdgeX_HP + paddingHP) + horizontalOffsetHP;
        float hpFillCenterX = hpFillLeftX + (hpFillWidth * 0.5f);

        healthBarFill->getTransform().setScale(glm::vec3(hpFillWidth, innerHeightHP, 0.0f));
        healthBarFill->getTransform().setPosition(glm::vec3(hpFillCenterX, healthY, 0.0f));
    }

    {
        float stFillWidth = staminaPercent * innerWidthST;
        float stFillLeftX = (leftEdgeX_ST + paddingST) + horizontalOffsetST;

        float stFillCenterX = stFillLeftX + (stFillWidth * 0.5f);

        staminaBarFill->getTransform().setScale(glm::vec3(stFillWidth, innerHeightST, 0.0f));
        staminaBarFill->getTransform().setPosition(glm::vec3(stFillCenterX, staminaY, 0.0f));
    }
}