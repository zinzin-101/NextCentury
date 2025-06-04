#include "LevelAct12.h"

void LevelAct12::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);


    addLoadingScreen(objectsList);
    levelDraw();
    removeLoadingScreen(objectsList);
}

void LevelAct12::levelInit() {
    UIobject = new IngameUI();
    player = new PlayerObject();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    float pictureWidth = 960.0f;
    float pictureHeight = 360.0f;
    float scaleX = (pictureWidth / pictureHeight) * 9.0f;
    float scaleY = 9.0f;

    TexturedObject* backGround1 = new TexturedObject();
    backGround1->setTexture("../Resource/Texture/Act7/BR_P01_RoomBase.png");
    backGround1->getTransform().setPosition(0.0f, 0.0f);
    backGround1->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace(objectsList.begin(), backGround1);

    TexturedObject* door2 = new TexturedObject();
    door2->getTransform().setPosition(-7.4f, -1.4f);
    door2->setTexture("../Resource/Texture/Act10/doorAct10.png");
    door2->getTransform().setScale(1.2f, 2.0f);
    objectsList.emplace_back(door2);

    door = new InteractableObject("../Resource/Texture/StoryStuff/medicineAct11.txt", player, "../Resource/Texture/Act11/doorAct12.png", objectsList);
    door->getTransform().setScale(1.218f, 2.0f);
    door->getTransform().setPosition(-7.4f, -1.4f);
    door->setActive(false);
    objectsList.emplace_back(door);

    TexturedObject* desk = new TexturedObject();
    desk->getTransform().setPosition(0.0f, 0.0f);
    desk->setTexture("../Resource/Texture/Act11/BS_P02_Desk.png");
    desk->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(desk);

    TexturedObject* shelf = new TexturedObject();
    shelf->getTransform().setPosition(0.0f, 0.0f);
    shelf->setTexture("../Resource/Texture/Act11/BS_P04_Shelf.png");
    shelf->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(shelf);

    TexturedObject* bed = new TexturedObject();
    bed->getTransform().setPosition(0.0f, 0.0f);
    bed->setTexture("../Resource/Texture/Act11/BS_P01_Bed.png");
    bed->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(bed);

    TexturedObject* board = new TexturedObject();
    board->getTransform().setPosition(0.0f, 0.0f);
    board->setTexture("../Resource/Texture/Act11/BS_P05_Board.png");
    board->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(board);

    TexturedObject* ishelf = new TexturedObject();
    ishelf->getTransform().setScale(1.13f, 1.54f);
    ishelf->setTexture("../Resource/Texture/Act11/shelfAct12.png");
    ishelf->getTransform().setPosition(-5.0f, -1.65f);
    objectsList.emplace_back(ishelf);

    TexturedObject* medicine = new TexturedObject();
    medicine->getTransform().setScale(1.1f, 0.52f);
    medicine->setTexture("../Resource/Texture/Act11/medicineAct12.png");
    medicine->getTransform().setPosition(2.9f, -1.34f);
    objectsList.emplace_back(medicine);

    Level::importTransformData(objectsList, "act12", false);

    player->getTransform().setPosition(glm::vec3(4.0f, -1.6f, 0.0f));
    player->setIsFacingRight(false);
    objectsList.emplace_back(player);

    TexturedObject* backGround = new TexturedObject();
    backGround->getTransform().setPosition(0.0f, 0.0f);
    backGround->setTexture("../Resource/Texture/Act10/IPTH_P04_Frame.png");
    backGround->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(backGround);

    isStop = false;

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    GameEngine::getInstance()->getRenderer()->setToggleViewport(false);

    // initializing parallax object
    for (DrawableObject* obj : objectsList) {
        ParallaxObject* pObj = dynamic_cast<ParallaxObject*>(obj);
        if (pObj != NULL) {
            pObj->setPlayer(player);
        }
    }

    startObjects(objectsList);

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -0.2f, 0));

    //UIobject->initUI(objectsList);

    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    vector<glm::vec3> l;
    l.push_back(glm::vec3(-7.0f, 0.8f, 0.0f));
    preFight = new ChatBubble("../Resource/Texture/StoryStuff/preBossFight.txt", player, l, objectsList);
    
    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(-5.0f, 0.0f, 0.0f));

    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitBool(true);
    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitMinMax(-9.0f, 9.0f);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct12::levelUpdate() {
    updateObjects(objectsList);
    preFight->runChat(objectsList);
    if (preFight->hasEnded()) {
        GameEngine::getInstance()->getRenderer()->updateCamera();
    }
    if (end) {
        timefade -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (timefade < 6.1f && !once) {
            fb->FadeToBlack();
            // play walking down stair sound
            once = true;
        }
        if (timefade < 0.0f) {
            loadNextLevel();
        }
    }

    

    //UIobject->updateUI(*player, camPos);
}

void LevelAct12::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct12::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
    delete preFight;
    delete UIobject;
}

void LevelAct12::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct12::handleKey(InputManager& input) {
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

    // handle event here
    if (!isStop) {
        if (input.getButton(SDLK_a) && !input.getButton(SDLK_d)) player->move(glm::vec2(-1, 0));
        if (input.getButton(SDLK_d) && !input.getButton(SDLK_a)) player->move(glm::vec2(1, 0));
        if (input.getButtonDown(SDLK_j)) player->parryAttack();
        if (input.getMouseButtonDown(SDL_BUTTON_RIGHT)) player->parryAttack();


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
    }

    if (input.getButtonDown(SDLK_e)) {
        
    }
}