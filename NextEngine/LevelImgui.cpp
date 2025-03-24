#include "LevelImgui.h"

void LevelImgui::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);
}

void LevelImgui::levelInit() {
    //GameEngine::getInstance()->setDrawArea(-8.0f, 960.0f, -540.0f, 540.0f);
    ParallaxObject* background = new ParallaxObject(0.0f, 7.3f, 550.0f, false, player, true);
    background->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P09_Sky.png");
    objectsList.emplace(objectsList.begin(), background);

    ParallaxObject* spaceShip = new ParallaxObject(0.0f, 7.3f, 450.0f, false, player, true);
    spaceShip->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P08_Spaceship.png");
    objectsList.emplace_back(spaceShip);

    ParallaxObject* Mountain1 = new ParallaxObject(0.0f, 7.3f, 350.0f, false, player, true);
    Mountain1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P06_Mountain01.png");
    objectsList.emplace_back(Mountain1);

    ParallaxObject* Mountain2 = new ParallaxObject(0.0f, 7.3f, 200.0f, false, player, true);
    Mountain2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P07_Mountain02.png");
    objectsList.emplace_back(Mountain2);

    ParallaxObject* MidGround1 = new ParallaxObject(0.0f, 7.3f, 150.0f, false, player, true);
    MidGround1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P05_MidGround01.png");
    objectsList.emplace_back(MidGround1);

    ParallaxObject* MidGround2 = new ParallaxObject(0.0f, 7.3f, 80.0f, false, player, true);
    MidGround2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P04_MidGround02.png");
    objectsList.emplace_back(MidGround2);

    ParallaxObject* MidGround3 = new ParallaxObject(0.0f, 7.3f, 50.0f, false, player, true);
    MidGround3->setTexture("../Resource/Texture/OutskirtParallax/Mid3.png");
    objectsList.emplace_back(MidGround3);

    ParallaxObject* Ground = new ParallaxObject(0.0f, 7.3f, 0.0f, false, player, true);
    Ground->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P02_Ground.png");
    objectsList.emplace_back(Ground);

    ParallaxObject* Fog = new ParallaxObject(0.0f, 7.3f, 100.0f, false, player, true);
    Fog->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P01_Fog.png");
    objectsList.emplace_back(Fog);

    marker = new SimpleObject();
    marker->setName("marker");
    marker->getTransform().setScale(0.2f);
    marker->setColor(0, 0, 0);
    marker->setBrightness(0);
    objectsList.emplace_back(marker);

    PlayerInfo playerInfo = PlayerInfo("Player", 10, MovementInfo(5, 25));
    player = new PlayerObject(playerInfo);
    objectsList.emplace_back(player);
    startObjects(objectsList);
    initPlayer(player, playerInfo);

    player->getTransform().scales(2);
    player->getColliderComponent()->getTransform().translate(0.0f, -1.0f);
    player->getColliderComponent()->setDimension(0.5f, 0.5f);
    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -1.0f, 0));
    player->getDamageCollider()->getTransform().scales(2);

    player->setActive(false);

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);
}

void LevelImgui::levelUpdate() {
    updateObjects(objectsList);
    GameEngine::getInstance()->getRenderer()->updateCamera(glm::vec3());

    #ifdef DEBUG_MODE_ON
    if (enableCameraMove) {
        GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(marker);
        GameEngine::getInstance()->getRenderer()->setToggleViewport(false);
    }
    else {
        GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
        GameEngine::getInstance()->getRenderer()->setToggleViewport(true);
    }
    #endif
}

void LevelImgui::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

    #ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
    #endif
}

void LevelImgui::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
}

void LevelImgui::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelImgui::handleKey(InputManager& input) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    /// Process key ///
    // add key that requires hold duration here
    processHeldKey(input, SDLK_k);
    processHeldMouse(input, SDL_BUTTON_LEFT);

    processHeldKey(input, SDLK_u);
    processHeldMouse(input, SDL_BUTTON_MIDDLE);

    // add key that requires buffering here
    processKeyBuffer(input, SDLK_LSHIFT);

    // handle event here
    if (input.getButton(SDLK_SPACE)) player->jump();
    if (input.getButton(SDLK_a)) player->move(glm::vec2(-1, 0));
    if (input.getButton(SDLK_d)) player->move(glm::vec2(1, 0));
    if (input.getButtonDown(SDLK_j)) player->parryAttack();
    if (input.getMouseButtonDown(SDL_BUTTON_RIGHT)) player->parryAttack();
    //if (input.getButtonDown(SDLK_f)) GameEngine::getInstance()->getRenderer()->toggleViewport();
    if (input.getButtonDown(SDLK_c)) player->getColliderComponent()->setTrigger(!player->getColliderComponent()->isTrigger());
    if (input.getButton(SDLK_z)) GameEngine::getInstance()->getRenderer()->increaseZoomRatio(0.1f);
    if (input.getButton(SDLK_x)) GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(0.1f);
    // test knockback

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

    if (keyBuffer[SDLK_LSHIFT] > 0 && player->getCanMove()) {
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

    if (input.getButton(SDLK_UP)) marker->getTransform().translate(glm::vec3(0, 10, 0) * dt);;
    if (input.getButton(SDLK_DOWN)) marker->getTransform().translate(glm::vec3(0, -10, 0) * dt);
    if (input.getButton(SDLK_LEFT)) marker->getTransform().translate(glm::vec3(-10, 0, 0) * dt);
    if (input.getButton(SDLK_RIGHT)) marker->getTransform().translate(glm::vec3(10, 0, 0) * dt);
}

void LevelImgui::initPlayer(PlayerObject*& player, glm::vec3 position, PlayerInfo playerInfo) {
    if (player == nullptr) {
        player = new PlayerObject(playerInfo);
        objectsList.emplace_back(player);
    }

    player->getTransform().setPosition(position);
    player->setDrawCollider(true); // for debugging
}

void LevelImgui::initPlayer(PlayerObject*& player, PlayerInfo playerInfo) {
    if (player == nullptr) {
        player = new PlayerObject(playerInfo);
        objectsList.emplace_back(player);
    }
    else {
        player->setName(playerInfo.name);
        player->setHealth(playerInfo.health);
        //player->setMovementInfo(playerInfo.movementInfo)
    }

    player->setDrawCollider(true); // for debugging
}