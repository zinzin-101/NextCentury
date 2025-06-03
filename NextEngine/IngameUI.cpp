#include "IngameUI.h"
#include "GameEngine.h"
#include <SDL.h>
#include <iostream>

void IngameUI::updateArrowPosition() {
    if (!arrow) return;

    std::vector<Button*>* activeList = nullptr;
    if (isPaused) {
        activeList = &pausemenubuttons;
    }
    else {
        if (playerObject.getHealth() <= 0) {
            activeList = &deathmenubuttons;
        }
        else {
            arrow->getTransform().setScale({ 0, 0, 0 });
            return;
        }
    }

    if (activeList->empty()) return;

    Transform& bt = (*activeList)[selectedButtonIndex]->getTransform();
    glm::vec3   bpos = bt.getPosition();
    glm::vec3   bscale = bt.getScale();

    glm::vec3 ascale = arrow->getTransform().getScale();
    const float margin = 0.1f;

    float arrowX = bpos.x
        - (bscale.x * 0.5f)
        - (ascale.x * 0.5f)
        - margin;

    arrow->getTransform().setPosition({ arrowX, bpos.y, bpos.z });
}

void IngameUI::initUI(std::list<DrawableObject*>& objectsList) {
    deathmenubuttons.resize(2);
    pausemenubuttons.resize(3);

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
    staminaBarFill->setColor(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
    objectsList.push_back(staminaBarFill);

    deathBlackdrop = new SimpleObject();
    deathBlackdrop->setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	deathBlackdrop->setRenderOpacity(0.8f);
    deathBlackdrop->getTransform().setScale({ 0, 0, 0 });
    objectsList.push_back(deathBlackdrop);

    deathText = new TexturedObject();
    deathText->setTexture("../Resource/Texture/UI/DeathScreen/DeathText.png");
    deathText->getTransform().setScale({ 0, 0, 0 });
    objectsList.push_back(deathText);

    deathmenubuttons[0] = new Button("RestartButton", "../Resource/Texture/UI/MainMenu/Retry.png");
    deathmenubuttons[0]->getTransform().setScale({ 0, 0, 0 });
    deathmenubuttons[0]->getTransform().setPosition({ 0, 0, 0 });
    deathmenubuttons[0]->setOnClickCallback([]() {
        std::cout << "Restart Button clicked!" << std::endl;
        GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART;
        });
    objectsList.push_back(deathmenubuttons[0]);

    deathmenubuttons[1] = new Button("MainMenuButton", "../Resource/Texture/UI/MainMenu/MainMenu.png");
    deathmenubuttons[1]->getTransform().setScale({ 0, 0, 0 });
    deathmenubuttons[1]->getTransform().setPosition({ 0, 0, 0 });
    deathmenubuttons[1]->setOnClickCallback([]() {
        std::cout << "Main Menu Button clicked!" << std::endl;
        GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_LEVEL1;
        });
    objectsList.push_back(deathmenubuttons[1]);

    pausemenubuttons[0] = new Button("ResumeButton", "../Resource/Texture/UI/MainMenu/PlayGame.png");
    pausemenubuttons[0]->getTransform().setScale({ 0, 0, 0 });
    pausemenubuttons[0]->getTransform().setPosition({ 0, 0, 0 });
    objectsList.push_back(pausemenubuttons[0]);

    pausemenubuttons[1] = new Button("SettingsButton", "../Resource/Texture/UI/MainMenu/Setting.png");
    pausemenubuttons[1]->getTransform().setScale({ 0, 0, 0 });
    pausemenubuttons[1]->getTransform().setPosition({ 0, 0, 0 });
    objectsList.push_back(pausemenubuttons[1]);

    pausemenubuttons[2] = new Button("MainMenuButton", "../Resource/Texture/UI/MainMenu/MainMenu.png");
    pausemenubuttons[2]->getTransform().setScale({ 0, 0, 0 });
    pausemenubuttons[2]->getTransform().setPosition({ 0, 0, 0 });
    objectsList.push_back(pausemenubuttons[2]);

    arrow = new TexturedObject("arrow");
    arrow->setTexture("../Resource/Texture/UI/MainMenu/Arrow.png");
    arrow->getTransform().setScale({ 0, 0, 0 });
    arrow->getTransform().setPosition({ 0, 0, 0 });
    objectsList.push_back(arrow);

    baseGunScale = { 0.6f, 0.6f, 0.0f };
    for (int i = 0; i < MAX_BULLETS; ++i) {
        TexturedObject* icon = new TexturedObject();
        icon->setTexture("../Resource/Texture/UI/Ingame/Gun.png");
        icon->getTransform().setScale({ 0, 0, 0 });
        objectsList.push_back(icon);
        gunIcons.push_back(icon);
    }

    selectedButtonIndex = 0;
    deathmenubuttons[0]->setFocused(true);
    deathmenubuttons[1]->setFocused(false);
}

void IngameUI::updateUI(PlayerObject& player) {
    camPos = GameEngine::getInstance()->getRenderer()->getCamera()->getPosition();
    playerObject = player;

    if (isPaused) {
        healthBar->getTransform().setScale({ 0, 0, 0 });
        healthBarFill->getTransform().setScale({ 0, 0, 0 });
        staminaBar->getTransform().setScale({ 0, 0, 0 });
        staminaBarFill->getTransform().setScale({ 0, 0, 0 });
        for (auto* icon : gunIcons) {
            icon->getTransform().setScale({ 0, 0, 0 });
        }

        deathText->getTransform().setScale({ 0, 0, 0 });
        for (auto* btn : deathmenubuttons) {
            btn->getTransform().setScale({ 0, 0, 0 });
            btn->setFocused(false);
        }

        float blackoutWidth = 20.0f;
        float blackoutHeight = 16.0f;
        deathBlackdrop->getTransform().setScale({ blackoutWidth, blackoutHeight, 0.0f });
        deathBlackdrop->getTransform().setPosition({ camPos.x, camPos.y, camPos.z });

        glm::vec3 btnScale = { 1.5f, 0.5f, 0.0f };

        pausemenubuttons[0]->getTransform().setScale(btnScale);
        pausemenubuttons[0]->getTransform().setPosition({ camPos.x, camPos.y - 1.0f, camPos.z });
        pausemenubuttons[1]->getTransform().setScale(btnScale);
        pausemenubuttons[1]->getTransform().setPosition({ camPos.x, camPos.y - 2.0f, camPos.z });
        pausemenubuttons[2]->getTransform().setScale(btnScale);
        pausemenubuttons[2]->getTransform().setPosition({ camPos.x, camPos.y - 3.0f, camPos.z });

        for (int i = 0; i < (int)pausemenubuttons.size(); ++i) {
            pausemenubuttons[i]->setFocused(i == selectedButtonIndex);
        }

        arrow->getTransform().setScale({ 0.213f, 0.213f, 0.0f });
        updateArrowPosition();
        return;
    }

    if (playerObject.getHealth() > 0) {

        deathBlackdrop->getTransform().setScale({ 0, 0, 0 });
        deathText->getTransform().setScale({ 0, 0, 0 });
        for (auto* btn : deathmenubuttons) {
            btn->getTransform().setScale({ 0, 0, 0 });
            btn->setFocused(false);
        }
        arrow->getTransform().setScale({ 0, 0, 0 });

        float healthPercent = (float)playerObject.getHealth() / 100.0f;
        float staminaPercent = (float)playerObject.getStamina() / 100.0f;

        float fullWidthHP = 4.0f;
        float frameHeightHP = 0.40f;
        float paddingHP = 0.08f;
        float innerWidthHP = fullWidthHP - 2.0f * paddingHP;
        float innerHeightHP = frameHeightHP - 2.0f * paddingHP;
        float horizontalOffsetHP = -5.0f;
        float verticalOffsetHP = 3.80f;

        float fullWidthST = 2.5f;
        float frameHeightST = 0.20f;
        float paddingST = 0.06f;
        float innerWidthST = fullWidthST - 2.0f * paddingST;
        float innerHeightST = frameHeightST - 2.0f * paddingST;
        float horizontalOffsetST = -5.75f;
        float verticalOffsetST = 3.50f;

        float leftEdgeX_HP = camPos.x - (fullWidthHP * 0.5f);
        float leftEdgeX_ST = camPos.x - (fullWidthST * 0.5f);

        float healthY = camPos.y - verticalOffsetHP;
        float staminaY = camPos.y - verticalOffsetST;

        {
            float centerX_HP = (leftEdgeX_HP + (fullWidthHP * 0.5f)) + horizontalOffsetHP;
            healthBar->getTransform().setScale({ fullWidthHP, frameHeightHP, 0.0f });
            healthBar->getTransform().setPosition({ centerX_HP, healthY, 0.0f });
        }
        float centerX_ST;
        {
            centerX_ST = (leftEdgeX_ST + (fullWidthST * 0.5f)) + horizontalOffsetST;
            staminaBar->getTransform().setScale({ fullWidthST, frameHeightST, 0.0f });
            staminaBar->getTransform().setPosition({ centerX_ST, staminaY, 0.0f });
        }
        {
            float hpFillWidth = healthPercent * innerWidthHP;
            float hpFillLeftX = (leftEdgeX_HP + paddingHP) + horizontalOffsetHP;
            float hpFillCenterX = hpFillLeftX + (hpFillWidth * 0.5f);

            healthBarFill->getTransform().setScale({ hpFillWidth, innerHeightHP, 0.0f });
            healthBarFill->getTransform().setPosition({ hpFillCenterX, healthY, 0.0f });
        }
        {
            float stFillWidth = staminaPercent * innerWidthST;
            float rightEdgeX_ST = (leftEdgeX_ST + fullWidthST) + horizontalOffsetST;
            float stInnerRightX = rightEdgeX_ST - paddingST;
            float stFillCenterX = stInnerRightX - (stFillWidth * 0.5f);

            staminaBarFill->getTransform().setScale({ stFillWidth, innerHeightST, 0.0f });
            staminaBarFill->getTransform().setPosition({ stFillCenterX, staminaY, 0.0f });
        }
        {
            int currentBullets = playerObject.getCurrentNumOfBullet();
            if (currentBullets < 0)            currentBullets = 0;
            if (currentBullets > MAX_BULLETS) currentBullets = MAX_BULLETS;

            float totalGroupWidth = (MAX_BULLETS - 1) * 0.5f;
            float startX = centerX_ST - (totalGroupWidth * 0.5f);

            for (int i = 0; i < MAX_BULLETS; ++i) {
                TexturedObject* icon = gunIcons[i];
                if (i < currentBullets) {
                    icon->getTransform().setScale(baseGunScale);
                    float iconX = startX + (0.8f * i);
                    float iconY = staminaY + 0.5f;
                    icon->getTransform().setPosition({ iconX, iconY, 0.0f });
                }
                else {
                    icon->getTransform().setScale({ 0, 0, 0 });
                }
            }
        }
    }
    else {
        healthBar->getTransform().setScale({ 0, 0, 0 });
        healthBarFill->getTransform().setScale({ 0, 0, 0 });
        staminaBar->getTransform().setScale({ 0, 0, 0 });
        staminaBarFill->getTransform().setScale({ 0, 0, 0 });
        for (auto* icon : gunIcons) {
            icon->getTransform().setScale({ 0, 0, 0 });
        }

        float blackoutWidth = 20.0f;
        float blackoutHeight = 16.0f;
        deathBlackdrop->getTransform().setScale({ blackoutWidth, blackoutHeight, 0.0f });
        deathBlackdrop->getTransform().setPosition({ camPos.x, camPos.y, camPos.z });

        float textScaleX = 10.6666667f;
        float textScaleY = 6.0f;
        deathText->getTransform().setScale({ textScaleX, textScaleY, 0.0f });
        deathText->getTransform().setPosition({ camPos.x, camPos.y + 1.0f, camPos.z });

        glm::vec3 btnScale = { 1.5f, 0.5f, 0.0f };
        deathmenubuttons[0]->getTransform().setScale(btnScale);
        deathmenubuttons[0]->getTransform().setPosition({ camPos.x, camPos.y - 1.5f, camPos.z });
        deathmenubuttons[1]->getTransform().setScale(btnScale);
        deathmenubuttons[1]->getTransform().setPosition({ camPos.x, camPos.y - 2.5f, camPos.z });

        for (int i = 0; i < (int)deathmenubuttons.size(); ++i) {
            deathmenubuttons[i]->setFocused(i == selectedButtonIndex);
        }

        arrow->getTransform().setScale({ 0.213f, 0.213f, 0.0f });
        updateArrowPosition();
    }
}

void IngameUI::handleInput(InputManager& input) {
    if (input.getButtonDown(SDLK_q)) {
        isPaused = !isPaused;

        if (isPaused) {
            GameEngine::getInstance()->pauseTime();
            selectedButtonIndex = 0;
            pausemenubuttons[0]->setFocused(true);
            pausemenubuttons[1]->setFocused(false);
            pausemenubuttons[2]->setFocused(false);
        }
        else {
            GameEngine::getInstance()->resumeTime();
            for (auto* btn : pausemenubuttons) {
                btn->getTransform().setScale({ 0, 0, 0 });
                btn->setFocused(false);
            }
            deathBlackdrop->getTransform().setScale({ 0, 0, 0 });
        }

        arrow->getTransform().setScale({ 0, 0, 0 });
        return;
    }

    if (isPaused) {
        if (input.getButtonDown(SDLK_w) || input.getButtonDown(SDLK_UP)) {
            pausemenubuttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex - 1 + pausemenubuttons.size()) % pausemenubuttons.size();
            pausemenubuttons[selectedButtonIndex]->setFocused(true);
            updateArrowPosition();
        }
        if (input.getButtonDown(SDLK_s) || input.getButtonDown(SDLK_DOWN)) {
            pausemenubuttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex + 1) % pausemenubuttons.size();
            pausemenubuttons[selectedButtonIndex]->setFocused(true);
            updateArrowPosition();
        }
        if (input.getButtonDown(SDLK_RETURN) || input.getButtonDown(SDLK_SPACE)) {
            pausemenubuttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, true);
        }
        if (input.getButtonUp(SDLK_RETURN) || input.getButtonUp(SDLK_SPACE)) {
            pausemenubuttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, false);
        }
        return;
    }

    if (playerObject.getHealth() <= 0) {
        if (input.getButtonDown(SDLK_w) || input.getButtonDown(SDLK_UP)) {
            deathmenubuttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex - 1 + deathmenubuttons.size())
                % deathmenubuttons.size();
            deathmenubuttons[selectedButtonIndex]->setFocused(true);
            updateArrowPosition();
        }
        if (input.getButtonDown(SDLK_s) || input.getButtonDown(SDLK_DOWN)) {
            deathmenubuttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex + 1) % deathmenubuttons.size();
            deathmenubuttons[selectedButtonIndex]->setFocused(true);
            updateArrowPosition();
        }
        if (input.getButtonDown(SDLK_RETURN) || input.getButtonDown(SDLK_SPACE)) {
            deathmenubuttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, true);
        }
        if (input.getButtonUp(SDLK_RETURN) || input.getButtonUp(SDLK_SPACE)) {
            deathmenubuttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, false);
        }
    }
}

IngameUI::~IngameUI() {
}
