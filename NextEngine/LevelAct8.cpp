#include "LevelAct8.h"

void LevelAct8::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);


    addLoadingScreen(objectsList);
    levelDraw();
    removeLoadingScreen(objectsList);
    saveCurrentGameState();
}

void LevelAct8::levelInit() {
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    player = new PlayerObject();

    float pictureWidth = 960.0f;
    float pictureHeight = 360.0f;
    float scaleX = (pictureWidth / pictureHeight) * 9.0f;
    float scaleY = 9.0f;

    TexturedObject* backGround1 = new TexturedObject();
    backGround1->setTexture("../Resource/Texture/Act8/BRC_P01_RoomBase.png");
    backGround1->getTransform().setPosition(0.0f, 0.0f);
    backGround1->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace(objectsList.begin(), backGround1);

    TexturedObject* backGround2 = new TexturedObject();
    backGround2->setTexture("../Resource/Texture/Act8/BRC_P02_Mid1.png");
    backGround2->getTransform().setPosition(0.0f, 0.0f);
    backGround2->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(backGround2);

    TexturedObject* midGround1 = new TexturedObject();
    midGround1->setTexture("../Resource/Texture/Act8/BRC_P03_BlackFrame.png");
    midGround1->getTransform().setPosition(0.0f, -0.0f);
    midGround1->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(midGround1);

    Level::importTransformData(objectsList, "act8", false);

    door = new InteractableObject("../Resource/Texture/StoryStuff/NeonBoardDescription.txt", player, "../Resource/Texture/Act7/doorAct7.png", objectsList);
    door->getTransform().setScale(1.28f, 2.12f);
    door->getTransform().setPosition(7.8f, -1.31f);
    objectsList.emplace_back(door);

    shelf = new InteractableObject("../Resource/Texture/StoryStuff/ShelfAct8.txt", player, "../Resource/Texture/Act8/shelfAct8.png", objectsList);
    shelf->getTransform().setScale(3.225f, 2.5f);
    shelf->getTransform().setPosition(2.95f, -1.125f);
    objectsList.emplace_back(shelf);
    shelf->setActive(false);

    p1 = new ProtagThoughts("../Resource/Texture/StoryStuff/protagAct8.txt", player, 24);
    objectsList.emplace_back(p1);

    player->getTransform().setPosition(glm::vec3(-8.0f, 0.0f, 0.0f));
    objectsList.emplace_back(player);

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(-1.2f, 0.0f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(false);

    // initializing parallax object
    for (DrawableObject* obj : objectsList) {
        ParallaxObject* pObj = dynamic_cast<ParallaxObject*>(obj);
        if (pObj != NULL) {
            pObj->setPlayer(player);
        }
    }

    startObjects(objectsList);

    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -0.2f, 0));
    player->setIsFacingRight(true);

    UIobject->initUI(objectsList);

    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    combatBlock = new ColliderObject();
    combatBlock->getTransform().setPosition(7.4f, 0.0f);
    combatBlock->getTransform().setScale(1.0f, 10.0f);
    objectsList.emplace_back(combatBlock);

    player->setMaxNumOfPotion(2);

    repeat = new ProtagThoughts("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/repeat.txt", player);
    objectsList.emplace_back(repeat);

    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitBool(true);
    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitMinMax(-9.0f, 9.0f);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(4.5f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct8::levelUpdate() {
    updateObjects(objectsList);
    //GameEngine::getInstance()->getRenderer()->updateCamera();
    // at the very start of the game freeze everything and chatEnemy is "YOU HAVE MAGIC" THEN fight...

    if (end) {
        timefade -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (timefade < 0.0f) {
            loadNextLevel();
        }
    }

    // Placeholder death logic
    bool k = false;
    for (std::list<DrawableObject*>::iterator itr = objectsList.begin(); itr != objectsList.end(); ++itr) {
        EnemyObject* enemy = dynamic_cast<EnemyObject*>(*itr);
        if (enemy != NULL) {
            //if (enemy->getHealth() <= 0) {
            //    DrawableObject::destroyObject(enemy);
            //}
            k = true;
        }
    }
    if (!k) {
        combatBlock->setActive(false);
        shelf->setActive(true);
        GameEngine::getInstance()->getRenderer()->updateCamera();
    }

    UIobject->updateUI(player);
}

void LevelAct8::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct8::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
    delete UIobject;
}

void LevelAct8::levelUnload() {
    GameEngine::getInstance()->stopMusic();
    GameEngine::getInstance()->stopSfx();
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct8::handleKey(InputManager& input) {
    // For debugging
    if (input.getButton(SDLK_z)) GameEngine::getInstance()->getRenderer()->increaseZoomRatio(0.1f);
    if (input.getButton(SDLK_x)) GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(0.1f);

    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    /// Process key ///
    // add key that requires hold duration here
    processHeldKey(input, SDLK_k);
    processHeldMouse(input, SDL_BUTTON_LEFT);

    processHeldKey(input, SDLK_u);
    processHeldMouse(input, SDL_BUTTON_MIDDLE);

    // add key that requires buffering here
    processKeyBuffer(input, SDLK_SPACE);
    processKeyBuffer(input, SDLK_LSHIFT);

    if (!isStop) {
        // handle event here
        if (GameEngine::getInstance()->getTime()->getTimeScale() != 0) {
            if (input.getButton(SDLK_a) && !input.getButton(SDLK_d)) player->move(glm::vec2(-1, 0));
            if (input.getButton(SDLK_d) && !input.getButton(SDLK_a)) player->move(glm::vec2(1, 0));
            if (input.getButtonDown(SDLK_j)) player->parryAttack();
            if (input.getMouseButtonDown(SDL_BUTTON_RIGHT)) player->parryAttack();
        }

        /// Use processed key here ///
        if (keyHeldDuration[SDLK_k] < PlayerStat::DURATION_TO_START_HEAVY_ATTACK) {
            if (input.getButtonUp(SDLK_k)) {
                player->normalAttack();
            }
        }
        else {
            if (input.getButtonUp(SDLK_k)) {
                player->heavyAttack();
            }
            else if (input.getButton(SDLK_k)) {
                player->startHeavyAttack();
            }
        }

        if (mouseHeldDuration[SDL_BUTTON_LEFT] < PlayerStat::DURATION_TO_START_HEAVY_ATTACK) {
            if (input.getMouseButtonUp(SDL_BUTTON_LEFT)) {
                player->normalAttack();
            }
        }
        else {
            if (input.getMouseButtonUp(SDL_BUTTON_LEFT)) {
                player->heavyAttack();
            }
            else if (input.getMouseButton(SDL_BUTTON_LEFT)) {
                player->startHeavyAttack();
            }
        }

        if (input.getButtonUp(SDLK_u)) {
            player->rangeAttack(objectsList);
        }
        else if (input.getButton(SDLK_u)) {
            player->startRangeAttack(dt);
        }

        if (input.getMouseButtonUp(SDL_BUTTON_MIDDLE)) {
            player->rangeAttack(objectsList);
        }
        else if (input.getMouseButton(SDL_BUTTON_MIDDLE)) {
            player->startRangeAttack(dt);
        }

        if ((isKeyInBuffer(SDLK_LSHIFT) || (isKeyInBuffer(SDLK_SPACE))) && player->getCanMove()) {
            clearKeyBuffer(SDLK_SPACE);
            clearKeyBuffer(SDLK_LSHIFT);

            if (input.getButton(SDLK_a)) {
                player->dodge(-1.0f);
            }
            else if (input.getButton(SDLK_d)) {
                player->dodge(1.0f);
            }
            else {
                player->dodge();
            }
        }


        if (input.getButtonDown(SDLK_e)) {
            if (shelf->isClickedOnce && door->getIsClickable()) {
                fb->FadeToBlack();
                end = true;
            }
            else if(door->getIsClickable()) {
                repeat->reActivateDialogue("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/repeat.txt");
            }

            if (shelf->getIsClickable() && shelf->getIsActive()) {
                shelf->setDescriptionActive(!shelf->getDescriptionActive());
                if (shelf->isClickedOnce && !shelf->getDescriptionActive()) {
                    p1->activateDialogue();
                }
            }
        }
        if (input.getButtonDown(SDLK_r)) {
            player->useHealthPotion();
        }
    }
    UIobject->handleInput(input, player);
}