#include "LevelAct7.h"

void LevelAct7::levelLoad() {
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

void LevelAct7::levelInit() {
    GameEngine::getInstance()->playMusic("BGM_Act2.wav", 1);
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    player = new PlayerObject();

    float pictureWidth = 960.0f;
    float pictureHeight = 360.0f;
    float scaleX = (pictureWidth / pictureHeight) * 9.0f;
    float scaleY = 9.0f;

    TexturedObject* backGround1 = new TexturedObject();
    backGround1->setTexture("../Resource/Texture/Act7/BR_P01_RoomBase.png");
    backGround1->getTransform().setPosition(0.0f, 0.0f);
    backGround1->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace(objectsList.begin(), backGround1);

    TexturedObject* backGround2 = new TexturedObject();
    backGround2->setTexture("../Resource/Texture/Act7/BR_P02_Mid1.png");
    backGround2->getTransform().setPosition(0.0f, 0.0f);
    backGround2->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(backGround2);

    board = new InteractableObject("../Resource/Texture/StoryStuff/BoardAct7.txt", player, "../Resource/Texture/Act7/boardAct7.png", objectsList);
    board->getTransform().setScale(3.0f, 2.132f);
    board->getTransform().setPosition(1.0f, -0.54f);
    objectsList.emplace_back(board);

    computer = new InteractableObject("../Resource/Texture/StoryStuff/ComputerAct7.txt", player, "../Resource/Texture/Act7/computerAct7.png", objectsList);
    computer->getTransform().setScale(2.0f, 0.94f);
    computer->getTransform().setPosition(3.08f, -1.15f);
    objectsList.emplace_back(computer);

    TexturedObject* midGround1 = new TexturedObject();
    midGround1->setTexture("../Resource/Texture/Act7/BR_P03_Mid2.png");
    midGround1->getTransform().setPosition(0.0f, -0.0f);
    midGround1->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(midGround1);

    TexturedObject* midGround2 = new TexturedObject();
    midGround2->setTexture("../Resource/Texture/Act7/BR_P04_ComputerLight.png");
    midGround2->getTransform().setPosition(0.0f, -0.0f);
    midGround2->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(midGround2);

    TexturedObject* midGround3 = new TexturedObject();
    midGround3->setTexture("../Resource/Texture/Act7/BR_P05_BlackFrame.png");
    midGround3->getTransform().setPosition(0.0f, -0.0f);
    midGround3->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(midGround3);

    Level::importTransformData(objectsList, "act7", false);

    door = new InteractableObject("../Resource/Texture/StoryStuff/NeonBoardDescription.txt", player, "../Resource/Texture/Act7/doorAct7.png", objectsList);
    door->getTransform().setScale(1.28f, 2.12f);
    door->getTransform().setPosition(7.8f, -1.31f);
    objectsList.emplace_back(door);

    shelf = new InteractableObject("../Resource/Texture/StoryStuff/ShelfAct7.txt", player, "../Resource/Texture/Act7/shelfAct7.png", objectsList);
    shelf->getTransform().setScale(1.224f, 2.5f);
    shelf->getTransform().setPosition(-3.7f, -1.125f);
    objectsList.emplace_back(shelf);

    repeat = new ProtagThoughts("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/repeat.txt", player);
    objectsList.emplace_back(repeat);

    map = new GotItemText("- Got Map", player, objectsList);
    map->setAppearPos(4.0f, 0.0f);

    medicine = new GotItemText("+ 1 Medicine", player, objectsList);
    medicine->setAppearPos(4.0f, 0.0f);

    player->getTransform().setPosition(glm::vec3(-8.0f, -1.0f, 0.0f));
    objectsList.emplace_back(player);

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(-4.0f, 0.0f, 0.0f));
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
    player->setMaxNumOfPotion(1);
    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitBool(true);
    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitMinMax(-9.0f, 9.0f);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(4.5f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct7::levelUpdate() {
    updateObjects(objectsList);
    GameEngine::getInstance()->getRenderer()->updateCamera();
    map->update(objectsList);
    // at the very start of the game freeze everything and chatEnemy is "YOU HAVE MAGIC" THEN fight...

    if (end) {
        timefade -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (timefade < 0.0f) {
            loadNextLevel();
        }
    }

    // Placeholder death logic

    UIobject->updateUI(player);
}

void LevelAct7::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct7::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
    delete UIobject;
    delete map;
    delete medicine;
}

void LevelAct7::levelUnload() {
    GameEngine::getInstance()->stopMusic();
    GameEngine::getInstance()->stopSfx();
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct7::handleKey(InputManager& input) {
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
            if (shelf->isClickedOnce && computer->isClickedOnce && board->isClickedOnce && door->getIsClickable()) {
                fb->FadeToBlack();
                end = true;
            }
            else if (door->getIsClickable()){
                repeat->reActivateDialogue("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/repeat.txt");
            }
            if (shelf->getIsClickable()) {
                shelf->setDescriptionActive(!shelf->getDescriptionActive());
            }
            if (board->getIsClickable() && computer->getIsClickable()) {
                
            }
            else if (board->getIsClickable()) {
                board->setDescriptionActive(!board->getDescriptionActive());
                if (board->isClickedOnce && !board->getDescriptionActive()) {
                    medicine->activateAppear();
                    player->setMaxNumOfPotion(2);
                }
            }
            else if (computer->getIsClickable()) {
                computer->setDescriptionActive(!computer->getDescriptionActive());
                if (computer->isClickedOnce && !computer->getDescriptionActive()) {
                    map->activateAppear();
                }
            }
        }
    }
}