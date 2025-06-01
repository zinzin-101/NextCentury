#include "IngameUI.h"

void IngameUI::initUI(std::list<DrawableObject*>& objectsList) {
    // --- Health Bar ---
    healthBar = new TexturedObject();
    healthBar->setTexture("../Resource/Texture/UI/Ingame/MC_HP_Bar.png");
    objectsList.push_back(healthBar);

    healthBarFill = new SimpleObject();
    healthBarFill->setColor(glm::vec4(0.0353f, 0.7216f, 0.6706f, 1.0f));
    objectsList.push_back(healthBarFill);

    // --- Stamina Bar ---
    staminaBar = new TexturedObject();
    staminaBar->setTexture("../Resource/Texture/UI/Ingame/MC_Mana_Bar.png");
    objectsList.push_back(staminaBar);

    staminaBarFill = new SimpleObject();
    staminaBarFill->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    objectsList.push_back(staminaBarFill);

    // --- Gun Icons (one per bullet) ---
    // We’ll create MAX_BULLETS separate TexturedObjects and push them both
    // into objectsList and into our gunIcons vector.
    baseGunScale = glm::vec3(0.6f, 0.6f, 0.0f);

    for (int i = 0; i < MAX_BULLETS; ++i) {
        TexturedObject* icon = new TexturedObject();
        icon->setTexture("../Resource/Texture/UI/Ingame/Gun.png");

        // Initially hide all icons by scaling to zero
        icon->getTransform().setScale(glm::vec3(0.0f, 0.0f, 0.0f));

        objectsList.push_back(icon);
        gunIcons.push_back(icon);
    }
}

void IngameUI::updateUI(PlayerObject& playerObject) {
    // 1) Camera position
    camPos = GameEngine::getInstance()->getRenderer()->getCamera()->getPosition();

    // 2) Health and Stamina percentages
    float healthPercent = static_cast<float>(playerObject.getHealth()) / 100.0f;
    float staminaPercent = static_cast<float>(playerObject.getStamina()) / 100.0f;

    // --- Health‐bar metrics ---
    float fullWidthHP = 4.0f;
    float frameHeightHP = 0.40f;
    float paddingHP = 0.08f;
    float innerWidthHP = fullWidthHP - 2.0f * paddingHP;
    float innerHeightHP = frameHeightHP - 2.0f * paddingHP;
    float horizontalOffsetHP = -5.0f;
    float verticalOffsetHP = 3.80f;

    // --- Stamina‐bar metrics ---
    float fullWidthST = 2.5f;
    float frameHeightST = 0.20f;
    float paddingST = 0.06f;
    float innerWidthST = fullWidthST - 2.0f * paddingST;
    float innerHeightST = frameHeightST - 2.0f * paddingST;
    float horizontalOffsetST = -5.75f;
    float verticalOffsetST = 3.50f;

    // 3) Compute left edges in world space
    float leftEdgeX_HP = camPos.x - (fullWidthHP * 0.5f);
    float leftEdgeX_ST = camPos.x - (fullWidthST * 0.5f);

    // 4) Compute Y positions for health & stamina bars
    float healthY = camPos.y - verticalOffsetHP;
    float staminaY = camPos.y - verticalOffsetST;

    // ——————————————————————————————————————————
    // 5) Draw the Health‐bar frame
    {
        float centerX_HP = (leftEdgeX_HP + (fullWidthHP * 0.5f)) + horizontalOffsetHP;
        healthBar->getTransform().setScale(glm::vec3(fullWidthHP, frameHeightHP, 0.0f));
        healthBar->getTransform().setPosition(glm::vec3(centerX_HP, healthY, 0.0f));
    }

    // 6) Draw the Stamina‐bar frame
    float centerX_ST;
    {
        centerX_ST = (leftEdgeX_ST + (fullWidthST * 0.5f)) + horizontalOffsetST;
        staminaBar->getTransform().setScale(glm::vec3(fullWidthST, frameHeightST, 0.0f));
        staminaBar->getTransform().setPosition(glm::vec3(centerX_ST, staminaY, 0.0f));
    }

    // ——————————————————————————————————————————
    // 7) Draw the Health‐bar fill (unchanged)
    {
        float hpFillWidth = healthPercent * innerWidthHP;
        float hpFillLeftX = (leftEdgeX_HP + paddingHP) + horizontalOffsetHP;
        float hpFillCenterX = hpFillLeftX + (hpFillWidth * 0.5f);

        healthBarFill->getTransform().setScale(glm::vec3(hpFillWidth, innerHeightHP, 0.0f));
        healthBarFill->getTransform().setPosition(glm::vec3(hpFillCenterX, healthY, 0.0f));
    }

    // ——————————————————————————————————————————
    // 8) Draw the Stamina‐bar fill (colored quad)
    {
        // Anchor the fill on the right, so it shrinks right→left
        float stFillWidth = staminaPercent * innerWidthST;

        // Compute the world‐space X of the *right* inner edge:
        float rightEdgeX_ST = (leftEdgeX_ST + fullWidthST) + horizontalOffsetST;
        float stInnerRightX = rightEdgeX_ST - paddingST;

        // Center the fill halfway to the left from that right edge:
        float stFillCenterX = stInnerRightX - (stFillWidth * 0.5f);

        staminaBarFill->getTransform().setScale(glm::vec3(stFillWidth, innerHeightST, 0.0f));
        staminaBarFill->getTransform().setPosition(glm::vec3(stFillCenterX, staminaY, 0.0f));
    }

    // ——————————————————————————————————————————
    // 9) Draw discrete gun icons based on current bullet count
    {
        int currentBullets = playerObject.getCurrentNumOfBullet();
        if (currentBullets < 0) currentBullets = 0;
        if (currentBullets > MAX_BULLETS) currentBullets = MAX_BULLETS;

        // We will line them up horizontally, each offset by +0.5f in X from the previous.
        // Let’s choose the first icon’s X so that the group is roughly centered over centerX_ST.
        // For example, if MAX_BULLETS=4 and offset=0.5, the group’s total width = 3 * 0.5 = 1.5 units
        // (distance from icon[0] to icon[3]). We can shift the first icon left by half that:
        float totalGroupWidth = (MAX_BULLETS - 1) * 0.5f;
        float startX = centerX_ST - (totalGroupWidth * 0.5f);

        for (int i = 0; i < MAX_BULLETS; ++i) {
            TexturedObject* icon = gunIcons[i];
            if (i < currentBullets) {
                // Show this icon:
                icon->getTransform().setScale(baseGunScale);

                // Position it at startX + (0.5f * i)
                float iconX = startX + (0.8f * i);
                float iconY = staminaY + 0.5f;  // same Y offset you used before
                icon->getTransform().setPosition(glm::vec3(iconX, iconY, 0.0f));
            }
            else {
                // Hide this icon by scaling to zero
                icon->getTransform().setScale(glm::vec3(0.0f, 0.0f, 0.0f));
            }
        }
    }
}