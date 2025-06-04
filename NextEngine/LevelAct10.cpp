#include "LevelAct10.h"

void LevelAct10::levelLoad() {
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

void LevelAct10::levelInit() {
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

    TexturedObject* door = new TexturedObject();
    door->getTransform().setPosition(-7.4f, -1.4f);
    door->setTexture("../Resource/Texture/Act10/doorAct10.png");
    door->getTransform().setScale(1.2f, 2.0f);
    objectsList.emplace_back(door);

    TexturedObject* window = new TexturedObject();
    window->getTransform().setPosition(0.0f, 0.0f);
    window->setTexture("../Resource/Texture/Act10/IPTH_P03_Window.png");
    window->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(window);

    TexturedObject* shelf = new TexturedObject();
    shelf->getTransform().setPosition(0.0f, 0.0f);
    shelf->setTexture("../Resource/Texture/Act10/IPTH_P02_Shelfs.png");
    shelf->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(shelf);

    TexturedObject* board = new TexturedObject();
    board->getTransform().setPosition(-1.7f, -0.5f);
    board->setTexture("../Resource/Texture/Act10/board2Act10.png");
    board->getTransform().setScale(2.5f, 1.41f);
    objectsList.emplace_back(board);

    TexturedObject* bed = new TexturedObject();
    bed->getTransform().setPosition(0.0f, 0.0f);
    bed->setTexture("../Resource/Texture/Act10/com2Act10.png");
    bed->getTransform().setScale((640.0f / pictureHeight) * 9.0f, scaleY);
    objectsList.emplace_back(bed);



    //float height = 8.0f;
    //float width = height * 1.467889908256881;
    //for (auto a : objectsList) {
    //    a->getTransform().setScale(width, height);
    //}
    Level::importTransformData(objectsList, "act10", false);


    //player->getTransform().setScale(4.166f, 2.5f);
    //player->getColliderComponent()->getTransform().translate(0.0f, -0.44f);
    //player->getColliderComponent()->setDimension(0.25f, 0.65f);
    player->getTransform().setPosition(glm::vec3(-7.3f, -1.6f, 0.0f));
    objectsList.emplace_back(player);

    stair = new InteractableObject("../Resource/Texture/StoryStuff/ComputerAct7.txt", player, "../Resource/Texture/Act10/stairsAct10.png", objectsList);
    stair->getTransform().setScale(3.0f, 1.06f);
    stair->getTransform().setPosition(7.8f, -1.95f);
    objectsList.emplace_back(stair);

    TexturedObject* backGround = new TexturedObject();
    backGround->getTransform().setPosition(0.0f, 0.0f);
    backGround->setTexture("../Resource/Texture/Act10/IPTH_P04_Frame.png");
    backGround->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(backGround);

    isStop = false;

    //ParallaxObject* Fog = new ParallaxObject(0.0f, -0.1f, 100.0f, false, player, true, pictureWidth, pictureHeight);
    //Fog->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P01_Fog.png");
    //Fog->setRenderOpacity(0.25f);
    //objectsList.emplace_back(Fog);

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

    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -0.2f, 0));

    //ParallaxObject* fog = new ParallaxObject(0.0f, -0.1f, 60.0f, false, player, true, pictureWidth, pictureHeight);
    //fog->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P01_Fog.png");
    //objectsList.emplace_back(fog);

    //UIobject->initUI(objectsList);

    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(-4.0f, 0.0f, 0.0f));

    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitBool(true);
    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitMinMax(-9.0f, 9.0f);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct10::levelUpdate() {
    updateObjects(objectsList);

    GameEngine::getInstance()->getRenderer()->updateCamera();
    if(end){
        timefade -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (timefade < 0.0f) {
            loadNextLevel();
        }
    }
    //UIobject->updateUI(*player, camPos);
}

void LevelAct10::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct10::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();

    delete UIobject;
}

void LevelAct10::levelUnload() {
    GameEngine::getInstance()->stopMusic();
    GameEngine::getInstance()->stopSfx();
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct10::handleKey(InputManager& input) {
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
        if (stair->getIsClickable()) {
            fb->FadeToBlack();
            end = true;
        }
    }
}