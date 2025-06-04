#include "IngameUI.h"
#include "GameEngine.h"
#include <SDL.h>
#include <iostream>

static void applyTransforms(Transform& t,
    const glm::vec3& basePos,
    const glm::vec3& baseScale)
{
    t.setScale(baseScale);
    t.setPosition(basePos);
}

void IngameUI::updateArrowPosition(PlayerObject* playerObject) {
    if (!arrow || !playerObject) return;

    auto* activeList = (isPaused
        ? &pausemenubuttons
        : (playerObject->getHealth() <= 0 ? &deathmenubuttons : nullptr));

    if (!activeList || activeList->empty()) {
        arrow->getTransform().setScale(glm::vec3(0.0f));
        return;
    }

    Transform& bt = (*activeList)[selectedButtonIndex]->getTransform();
    glm::vec3  bpos = bt.getPosition();
    glm::vec3  bscale = bt.getScale();
    glm::vec3  ascale = arrow->getTransform().getScale();
    constexpr float margin = 0.1f;

    float arrowX = bpos.x - (bscale.x * 0.5f) - (ascale.x * 0.5f) - margin;

    applyTransforms(
        arrow->getTransform(),
        { arrowX, bpos.y, bpos.z },
        arrowBaseScale
    );
}

void IngameUI::hideAllUI() {
    healthBar->getTransform().setScale(glm::vec3(0.0f));
    healthBarFill->getTransform().setScale(glm::vec3(0.0f));
    staminaBar->getTransform().setScale(glm::vec3(0.0f));
    staminaBarFill->getTransform().setScale(glm::vec3(0.0f));
    gunIcons->getTransform().setScale(glm::vec3(0.0f));
    ammoIcon->getTransform().setScale(glm::vec3(0.0f));
    potionIcon0->getTransform().setScale(glm::vec3(0.0f));
    potionIcon1->getTransform().setScale(glm::vec3(0.0f));
    potionIcon2->getTransform().setScale(glm::vec3(0.0f));
    potionIcon3->getTransform().setScale(glm::vec3(0.0f));
	pauseText->getTransform().setScale(glm::vec3(0.0f));
}

void IngameUI::updateHealthStaminaUI(PlayerObject* playerObject) {
    float healthPct = (float)playerObject->getHealth() / 100.0f;
    float staminaPct = (float)playerObject->getStamina() / 100.0f;

    glm::vec3 HB_frameScale = healthBarBaseScale;
    glm::vec3 HB_fillUnitScale = healthBarFillBaseScale;
    glm::vec3 SB_frameScale = staminaBarBaseScale;
    glm::vec3 SB_fillUnitScale = staminaBarFillBaseScale;

    glm::vec3 HB_framePos = {
        camPos.x + healthBarBasePos.x,
        camPos.y + healthBarBasePos.y,
        camPos.z + healthBarBasePos.z
    };
    glm::vec3 SB_framePos = {
        camPos.x + staminaBarBasePos.x,
        camPos.y + staminaBarBasePos.y,
        camPos.z + staminaBarBasePos.z
    };

    constexpr float paddingX = 0.025f;

    applyTransforms(
        healthBar->getTransform(),
        HB_framePos,
        HB_frameScale
    );

    {
        float fullWidth_HP = HB_frameScale.x;
        float innerWidth_HP = fullWidth_HP - 2.0f * paddingX;
        if (innerWidth_HP < 0.0f) innerWidth_HP = 0.0f;

        float fillWidth_HP = healthPct * innerWidth_HP;

        glm::vec3 final_HP_fillScale = {
            fillWidth_HP,
            HB_fillUnitScale.y,
            0.0f
        };

        float frameLeftEdge_HP = HB_framePos.x - (fullWidth_HP * 0.5f);
        float fillCenterX_HP = frameLeftEdge_HP + paddingX + (fillWidth_HP * 0.5f);
        glm::vec3 fillPos_HP = { fillCenterX_HP,
                                      HB_framePos.y + (healthBarFillBasePos.y - healthBarBasePos.y),
                                      HB_framePos.z };

        applyTransforms(
            healthBarFill->getTransform(),
            fillPos_HP,
            final_HP_fillScale
        );
    }

    applyTransforms(
        staminaBar->getTransform(),
        SB_framePos,
        SB_frameScale
    );

    {
        float fullWidth_ST = SB_frameScale.x;
        float innerWidth_ST = fullWidth_ST - 2.0f * paddingX;
        if (innerWidth_ST < 0.0f) innerWidth_ST = 0.0f;

        float fillWidth_ST = staminaPct * innerWidth_ST;

        glm::vec3 final_ST_fillScale = {
            fillWidth_ST,
            SB_fillUnitScale.y,
            0.0f
        };

        float frameLeftEdge_ST = SB_framePos.x - (fullWidth_ST * 0.5f);
        float fillCenterX_ST = frameLeftEdge_ST + paddingX + (fillWidth_ST * 0.5f);
        glm::vec3 fillPos_ST = { fillCenterX_ST,
                                      SB_framePos.y + (staminaBarFillBasePos.y - staminaBarBasePos.y),
                                      SB_framePos.z };

        applyTransforms(
            staminaBarFill->getTransform(),
            fillPos_ST,
            final_ST_fillScale
        );
    }
}
void IngameUI::updateAmmoUI(PlayerObject* playerObject) {
    if (hideGunUI) return;
    Animation* ammoAnim = ammoIcon->getAnimationComponent();
    ammoAnim->setPaused(false);

    int count = playerObject->getCurrentNumOfBullet();
    if (count < 0)          count = 0;
    if (count > MAX_BULLETS) count = MAX_BULLETS;

    std::string newState = "ammo" + std::to_string(count);
    if (newState != currentAmmoState) {
        ammoAnim->setState(newState);
        currentAmmoState = newState;
    }
    ammoAnim->updateCurrentState();

    {
        glm::vec3 worldPos = {
            camPos.x + ammoIconBasePos.x,
            camPos.y + ammoIconBasePos.y,
            camPos.z + ammoIconBasePos.z
        };
        applyTransforms(
            ammoIcon->getTransform(),
            worldPos,
            ammoIconBaseScale
        );
    }

    {
        glm::vec3 worldGunPos = {
            camPos.x + gunIconsBasePos.x,
            camPos.y + gunIconsBasePos.y,
            camPos.z + gunIconsBasePos.z
        };
        applyTransforms(
            gunIcons->getTransform(),
            worldGunPos,
            gunIconsBaseScale
        );
    }
}

void IngameUI::showPauseMenu() {
    hideAllUI();

    applyTransforms(
        deathBlackdrop->getTransform(),
        { camPos.x + deathBlackdropBasePos.x,
          camPos.y + deathBlackdropBasePos.y,
          camPos.z + deathBlackdropBasePos.z },
        deathBlackdropBaseScale
    );

    applyTransforms(
        pauseText->getTransform(),
        { camPos.x + pauseTextBasePos.x,
          camPos.y + pauseTextBasePos.y,
          camPos.z + pauseTextBasePos.z },
        pauseTextBaseScale
    );

    glm::vec3 btnScale = { 1.5f, 0.5f, 0.0f };
    for (int i = 0; i < (int)pausemenubuttons.size(); ++i) {
        float y = camPos.y - (1.0f + i);
        applyTransforms(
            pausemenubuttons[i]->getTransform(),
            { camPos.x, y, camPos.z },
            btnScale
        );
        pausemenubuttons[i]->setFocused(i == selectedButtonIndex);
    }

    applyTransforms(
        arrow->getTransform(),
        arrowBasePos,
        arrowBaseScale
    );
}


void IngameUI::showDeathMenu(PlayerObject* playerObject) {
    if (isdeathMenuDeactivate)
        return;
    hideAllUI();

    applyTransforms(
        deathBlackdrop->getTransform(),
        { camPos.x + deathBlackdropBasePos.x, camPos.y + deathBlackdropBasePos.y, camPos.z + deathBlackdropBasePos.z },
        deathBlackdropBaseScale
    );

    applyTransforms(
        deathText->getTransform(),
        { camPos.x + deathTextBasePos.x, camPos.y + deathTextBasePos.y, camPos.z + deathTextBasePos.z },
        deathTextBaseScale
    );

    glm::vec3 btnScale = { 1.5f, 0.5f, 0.0f };
    for (int i = 0; i < (int)deathmenubuttons.size(); ++i) {
        float y = camPos.y - (1.5f + i);
        applyTransforms(
            deathmenubuttons[i]->getTransform(),
            { camPos.x, y, camPos.z },
            btnScale
        );
        deathmenubuttons[i]->setFocused(i == selectedButtonIndex);
    }

    applyTransforms(
        arrow->getTransform(),
        arrowBasePos,
        arrowBaseScale
    );
}

void IngameUI::updatePotionUI(PlayerObject* playerObject) {
    if (hidePotionUI) return;
    if (!playerObject) return;

    int count = playerObject->getCurrentNumOfPotion();
    if (count < 0) count = 0;
    if (count > 3) count = 3;   

    potionIcon0->getTransform().setScale(glm::vec3(0.0f));
    potionIcon1->getTransform().setScale(glm::vec3(0.0f));
    potionIcon2->getTransform().setScale(glm::vec3(0.0f));
    potionIcon3->getTransform().setScale(glm::vec3(0.0f));

    TexturedObject* toShow = nullptr;
    switch (count) {
    case 0: toShow = potionIcon0; break;
    case 1: toShow = potionIcon1; break;
    case 2: toShow = potionIcon2; break;
    case 3: toShow = potionIcon3; break;
    }

    if (toShow) {
        glm::vec3 worldPos = {
            camPos.x + potionIconBasePos.x,
            camPos.y + potionIconBasePos.y,
            camPos.z + potionIconBasePos.z
        };
        applyTransforms(
            toShow->getTransform(),
            worldPos,
            potionIconBaseScale
        );
    }
}

void IngameUI::initUI(std::list<DrawableObject*>& objectsList) {
    deathmenubuttons.resize(2);
    pausemenubuttons.resize(3);

    healthBar = new TexturedObject();
    healthBar->setTexture("../Resource/Texture/UI/Ingame/MC_HP_Bar.png");
    applyTransforms(
        healthBar->getTransform(),
        healthBarBasePos,
        healthBarBaseScale
    );
	healthBar->setRenderOrder(50); 
    objectsList.push_back(healthBar);


    healthBarFill = new SimpleObject();
    healthBarFill->setColor({ 0.0353f, 0.7216f, 0.6706f });
    applyTransforms(
        healthBarFill->getTransform(),
        healthBarFillBasePos,
        healthBarFillBaseScale
    );
	healthBarFill->setRenderOrder(51);
    objectsList.push_back(healthBarFill);

    staminaBar = new TexturedObject();
    staminaBar->setTexture("../Resource/Texture/UI/Ingame/MC_Mana_Bar.png");
    applyTransforms(
        staminaBar->getTransform(),
        staminaBarBasePos,
        staminaBarBaseScale
    );
	staminaBar->setRenderOrder(52);
    objectsList.push_back(staminaBar);

    staminaBarFill = new SimpleObject();
    staminaBarFill->setColor({ 1.0f, 1.0f, 1.0f });
    applyTransforms(
        staminaBarFill->getTransform(),
        staminaBarFillBasePos,
        staminaBarFillBaseScale
    );
	staminaBarFill->setRenderOrder(53);
    objectsList.push_back(staminaBarFill);

    deathBlackdrop = new SimpleObject();
    deathBlackdrop->setColor({ 0.0f, 0.0f, 0.0f });
    deathBlackdrop->setRenderOpacity(0.8f);
    deathBlackdrop->getTransform().setScale(glm::vec3(0.0f));
	deathBlackdrop->setRenderOrder(95); 
    objectsList.push_back(deathBlackdrop);

    deathText = new TexturedObject();
    deathText->setTexture("../Resource/Texture/UI/DeathScreen/DeathText.png");
    deathText->getTransform().setScale(glm::vec3(0.0f));
	deathText->setRenderOrder(101);
    objectsList.push_back(deathText);

    // 4) Death-menu buttons
    deathmenubuttons[0] = new Button("RestartButton", "../Resource/Texture/UI/MainMenu/Retry.png");
    deathmenubuttons[0]->getTransform().setScale(glm::vec3(0.0f));
    deathmenubuttons[0]->setOnClickCallback([]() {
        GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART;
        });
	deathmenubuttons[0]->setRenderOrder(102);
    objectsList.push_back(deathmenubuttons[0]);

    deathmenubuttons[1] = new Button("MainMenuButton", "../Resource/Texture/UI/MainMenu/MainMenu.png");
    deathmenubuttons[1]->getTransform().setScale(glm::vec3(0.0f)); 
    deathmenubuttons[1]->setOnClickCallback([]() {
        GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_MENU;
        });
	deathmenubuttons[1]->setRenderOrder(103);
    objectsList.push_back(deathmenubuttons[1]);

    // 5) Pause-menu buttons
    pausemenubuttons[0] = new Button("ResumeButton", "../Resource/Texture/UI/MainMenu/PlayGame.png");
    pausemenubuttons[0]->getTransform().setScale(glm::vec3(0.0f)); 
    pausemenubuttons[0]->setRenderOrder(103);
    objectsList.push_back(pausemenubuttons[0]);

    pausemenubuttons[1] = new Button("SettingsButton", "../Resource/Texture/UI/MainMenu/Setting.png");
    pausemenubuttons[1]->getTransform().setScale(glm::vec3(0.0f)); 
    pausemenubuttons[1]->setRenderOrder(104);
    objectsList.push_back(pausemenubuttons[1]);

    pausemenubuttons[2] = new Button("MainMenuButton", "../Resource/Texture/UI/MainMenu/MainMenu.png");
    pausemenubuttons[2]->getTransform().setScale(glm::vec3(0.0f)); 
    pausemenubuttons[2]->setRenderOrder(105);
    objectsList.push_back(pausemenubuttons[2]);

	potionIcon0 = new TexturedObject("PotionIcon0");
	potionIcon0->setTexture("../Resource/Texture/UI/Ingame/Heal0.png");
	potionIcon0->getTransform().setScale(glm::vec3(0.0f));
	potionIcon0->setRenderOrder(54);
	objectsList.push_back(potionIcon0);

    potionIcon1 = new TexturedObject("PotionIcon1");
    potionIcon1->setTexture("../Resource/Texture/UI/Ingame/Heal1.png");
    potionIcon1->getTransform().setScale(glm::vec3(0.0f));
	potionIcon1->setRenderOrder(55);
    objectsList.push_back(potionIcon1);

    potionIcon2 = new TexturedObject("PotionIcon2");
    potionIcon2->setTexture("../Resource/Texture/UI/Ingame/Heal2.png");
    potionIcon2->getTransform().setScale(glm::vec3(0.0f));
	potionIcon2->setRenderOrder(56);
    objectsList.push_back(potionIcon2);

    potionIcon3 = new TexturedObject("PotionIcon3");
    potionIcon3->setTexture("../Resource/Texture/UI/Ingame/Heal3.png");
    potionIcon3->getTransform().setScale(glm::vec3(0.0f));
	potionIcon3->setRenderOrder(57);
    objectsList.push_back(potionIcon3);

    arrow = new TexturedObject("arrow");
    arrow->setTexture("../Resource/Texture/UI/MainMenu/Arrow.png");
    arrow->getTransform().setScale(glm::vec3(0.0f));
	arrow->setRenderOrder(58);
    objectsList.push_back(arrow);

    gunIcons = new TexturedObject("GunIcons");
    gunIcons->setTexture("../Resource/Texture/UI/Ingame/gunNew.png");
    gunIcons->getTransform().setScale(glm::vec3(0.0f)); 
	gunIcons->setRenderOrder(59);
    objectsList.push_back(gunIcons);

    ammoIcon = new TexturedObject("AmmoIcon");
    ammoIcon->setTexture("../Resource/Texture/UI/Ingame/Bullet2SpritesheetFix.png");
    ammoIcon->initAnimation(MAX_BULLETS + 1, 1);

	pauseText = new TexturedObject("PauseText");
	pauseText->setTexture("../Resource/Texture/UI/Setting/PAUSE.png");
	pauseText->getTransform().setScale(glm::vec3(0.0f));
	pauseText->setRenderOrder(112);
	objectsList.push_back(pauseText);

    Animation* ammoAnim = ammoIcon->getAnimationComponent();
    for (int i = 0; i <= MAX_BULLETS; ++i) {
        std::string s = "ammo" + std::to_string(i);
        ammoAnim->addState(s, i, 0, 4, true);
    }
    ammoAnim->setState("ammo0");

    ammoIcon->getTransform().setScale(glm::vec3(0.0f)); 
	ammoIcon->setRenderOrder(61);
    objectsList.push_back(ammoIcon);
}

void IngameUI::updateUI(PlayerObject* playerObject) {
    camPos = GameEngine::getInstance()->getRenderer()->getCamera()->getPosition();

    if (isPaused) {
        hideAllUI();
        showPauseMenu();
        ammoIcon->getAnimationComponent()->setPaused(true);
        return;
	}
	else pauseText->getTransform().setScale(glm::vec3(0.0f));

    if (playerObject && playerObject->getHealth() > 0) {
        deathBlackdrop->getTransform().setScale(glm::vec3(0.0f));
        deathText->getTransform().setScale(glm::vec3(0.0f));
        for (auto* btn : deathmenubuttons) {
            btn->getTransform().setScale(glm::vec3(0.0f));
            btn->setFocused(false);
        }
        arrow->getTransform().setScale(glm::vec3(0.0f));

        updateHealthStaminaUI(playerObject);

        updateAmmoUI(playerObject);

        updatePotionUI(playerObject);
    }
    else {
        hideAllUI();
        showDeathMenu(playerObject);
        ammoIcon->getAnimationComponent()->setPaused(true);
    }
}

void IngameUI::handleInput(InputManager& input, PlayerObject* playerObject) {
    if (input.getButtonDown(SDLK_ESCAPE)) {
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
                btn->getTransform().setScale(glm::vec3(0.0f));
                btn->setFocused(false);
            }
            deathBlackdrop->getTransform().setScale(glm::vec3(0.0f));
        }
        arrow->getTransform().setScale(glm::vec3(0.0f));
        return;
    }

    if (isPaused) {
        if (input.getButtonDown(SDLK_w) || input.getButtonDown(SDLK_UP)) {
            pausemenubuttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex - 1 + pausemenubuttons.size()) % pausemenubuttons.size();
            pausemenubuttons[selectedButtonIndex]->setFocused(true);
            updateArrowPosition(playerObject);
        }
        if (input.getButtonDown(SDLK_s) || input.getButtonDown(SDLK_DOWN)) {
            pausemenubuttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex + 1) % pausemenubuttons.size();
            pausemenubuttons[selectedButtonIndex]->setFocused(true);
            updateArrowPosition(playerObject);
        }
        if (input.getButtonDown(SDLK_RETURN) || input.getButtonDown(SDLK_SPACE)) {
            pausemenubuttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, true);
        }
        if (input.getButtonUp(SDLK_RETURN) || input.getButtonUp(SDLK_SPACE)) {
            pausemenubuttons[selectedButtonIndex]->handleKeyboardInput(SDLK_RETURN, false);
        }
        return;
    }

    if (playerObject && playerObject->getHealth() <= 0) {
        if (input.getButtonDown(SDLK_w) || input.getButtonDown(SDLK_UP)) {
            deathmenubuttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex - 1 + deathmenubuttons.size()) % deathmenubuttons.size();
            deathmenubuttons[selectedButtonIndex]->setFocused(true);
            updateArrowPosition(playerObject);
        }
        if (input.getButtonDown(SDLK_s) || input.getButtonDown(SDLK_DOWN)) {
            deathmenubuttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex + 1) % deathmenubuttons.size();
            deathmenubuttons[selectedButtonIndex]->setFocused(true);
            updateArrowPosition(playerObject);
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
