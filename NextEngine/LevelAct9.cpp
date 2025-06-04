#include "LevelAct9.h"

void LevelAct9::levelLoad() {
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

void LevelAct9::levelInit() {
    GameEngine::getInstance()->playMusic("BGM_Act2.wav", 1);
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    player = new PlayerObject();

    float pictureWidth = 960.0f;
    float pictureWidthB = 2880.0f;
    float pictureHeight = 360.0f;
    
    ParallaxObject* sky = new ParallaxObject(32.0f, 0.0f, 550.0f, false, player, true, pictureWidthB, pictureHeight);
    sky->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P15_Sky.png");
    objectsList.emplace(objectsList.begin(), sky);

    ParallaxObject* mountain2 = new ParallaxObject(32.0f, 0.0f, 250.0f, false, player, true, pictureWidthB, pictureHeight);
    mountain2->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P13_Mountain02.png");
    objectsList.emplace_back(mountain2);

    ParallaxObject* mountain1 = new ParallaxObject(32.0f, 0.0f, 200.0f, false, player, true, pictureWidthB, pictureHeight);
    mountain1->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P12_Mountain01.png");
    objectsList.emplace_back(mountain1);

    ParallaxObject* midGround6 = new ParallaxObject(32.0f, 0.0f, 100.0f, false, player, true, pictureWidthB, pictureHeight);
    midGround6->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P11_MidGround06.png");
    objectsList.emplace_back(midGround6);

    ParallaxObject* mid1 = new ParallaxObject(0.0f, 0.0f, 10.0f, false, player, true, pictureWidth, pictureHeight);
    mid1->setTexture("../Resource/Texture/Act9/ENMB02_P04_MidGround01.png");
    objectsList.emplace_back(mid1);

    ParallaxObject* fence = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    fence->setTexture("../Resource/Texture/Act9/ENMB02_P05_Fence.png");
    objectsList.emplace_back(fence);

    ParallaxObject* car = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    car->setTexture("../Resource/Texture/Act9/ENMB02_P01_Car.png");
    objectsList.emplace_back(car);

    ParallaxObject* sign = new ParallaxObject(32.0f, 0.0f, 0.0f, false, player, false, pictureWidthB, pictureHeight);
    sign->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P03_ProtestSigns.png");
    objectsList.emplace_back(sign);

    ParallaxObject* house = new ParallaxObject((pictureWidth / pictureHeight) * 9.0f, 0.0f, 0.0f, false, player, false, pictureWidth, pictureHeight);
    house->setTexture("../Resource/Texture/Act9/ENMB02_P03_House.png");
    objectsList.emplace_back(house);

    ParallaxObject* ground = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    ground->setTexture("../Resource/Texture/Act6/IMGT_P01_Ground.png");
    objectsList.emplace_back(ground);
    
 //   lightPole->getTransform().setScale(47.999999f, 9.0f);

 //   sky->getTransform().setScale(500.f, 500.f);

    Level::importTransformData(objectsList, "act9", false);

    door = new InteractableObject("../Resource/Texture/StoryStuff/NeonBoardDescription.txt", player, "../Resource/Texture/Act9/doorAct9.png", objectsList);
    door->getTransform().setScale(0.83f, 1.66f);
    door->getTransform().setPosition(30.46f, -1.38f);
    door->setActive(false);
    objectsList.emplace_back(door);

    //player->getTransform().setScale(4.166f, 2.5f);
    //player->getColliderComponent()->getTransform().translate(0.0f, -0.44f);
    //player->getColliderComponent()->setDimension(0.25f, 0.65f);
    objectsList.emplace_back(player);

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    GameEngine::getInstance()->getRenderer()->setToggleViewport(false);

    // initializing parallax object
    for (DrawableObject* obj : objectsList) {
        ParallaxObject* pObj = dynamic_cast<ParallaxObject*>(obj);
        if (pObj != NULL) {
            pObj->setPlayer(player);
        }

        //LivingEntity* lObj = dynamic_cast<LivingEntity*>(obj);
        //if (lObj != NULL) {
        //    lObj->setAffectedByLighting(true);
        //}
    }

    startObjects(objectsList);

    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -0.2f, 0));
    player->getTransform().setPosition(glm::vec3(-2.0f, -1.6f, 0.0f));

    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    UIobject->initUI(objectsList);

    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(3.8f, 0.0f, 0.0f));

    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitBool(true);
    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitMinMax(-1.0f, 33.25f);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);
}

void LevelAct9::levelUpdate() {
    updateObjects(objectsList);

    GameEngine::getInstance()->getRenderer()->updateCamera();

    //Dialogue logics

    if (end) {
        timefade -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (timefade < 0.0f) {
            loadNextLevel();
        }
    }

    bool k = false;
    for (std::list<DrawableObject*>::iterator itr = objectsList.begin(); itr != objectsList.end(); ++itr) {
        EnemyObject* enemy = dynamic_cast<EnemyObject*>(*itr);
        if (enemy != NULL) {
            k = true;
        }
        else {

        }
    }
    if (!k) {
        door->setActive(true);
    }
    UIobject->updateUI(player);
}

void LevelAct9::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct9::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();

    delete UIobject;
}

void LevelAct9::levelUnload() {
    GameEngine::getInstance()->stopMusic();
    GameEngine::getInstance()->stopSfx();
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct9::handleKey(InputManager& input) {
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

    //Dialogue interact
    if (input.getButtonDown(SDLK_e)) {
        if (door->getIsClickable() && door->getIsActive()) {
            end = true;
            fb->FadeToBlack();
        }
    }
}

void LevelAct9::signalFromEngine() {
    this->killCount++;
}