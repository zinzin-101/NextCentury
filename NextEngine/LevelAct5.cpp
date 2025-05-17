#include "LevelAct5.h"

void LevelAct5::levelLoad() {
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

void LevelAct5::levelInit() {
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    float pictureWidth = 640.0f;
    float pictureHeight = 436.0f;

    ParallaxObject* sky = new ParallaxObject(0.0f, 0.0f, 550.0f, false, player, true, pictureWidth, pictureHeight);
    sky->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P09_Sky.png");
    objectsList.emplace(objectsList.begin(), sky);

    ParallaxObject* mountain1 = new ParallaxObject(0.0f, 0.5f, 250.0f, false, player, true, pictureWidth, pictureHeight);
    mountain1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P06_Mountain01.png");
    objectsList.emplace_back(mountain1);

    ParallaxObject* mountain2 = new ParallaxObject(0.0f, 0.5f, 200.0f, false, player, true, pictureWidth, pictureHeight);
    mountain2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P07_Mountain02.png");
    objectsList.emplace_back(mountain2);

    ParallaxObject* midGround1 = new ParallaxObject(0.0f, 0.2f, 100.0f, false, player, true, pictureWidth, pictureHeight);
    midGround1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P05_MidGround01.png");
    objectsList.emplace_back(midGround1);

    ParallaxObject* midGround2 = new ParallaxObject(0.0f, -0.1f, 15.0f, false, player, true, pictureWidth, pictureHeight);
    midGround2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P04_MidGround02.png");
    objectsList.emplace_back(midGround2);

    ParallaxObject* midGround3 = new ParallaxObject(0.0f, -0.1f, 10.0f, false, player, true, pictureWidth, pictureHeight);
    midGround3->setTexture("../Resource/Texture/OutskirtParallax/Mid3.png");
    objectsList.emplace_back(midGround3);

    //ParallaxObject* spaceShip = new ParallaxObject(0.0f, -0.0f, 90.0f, false, player, true, pictureWidth, pictureHeight); // SHOULD BE AN OBJECT FLYING ON ITS OWN
    //spaceShip->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P08_Spaceship.png");
    //objectsList.emplace_back(spaceShip);

    ParallaxObject* ground = new ParallaxObject(0.0f, -0.1f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    ground->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P02_Ground.png");
    objectsList.emplace_back(ground);

    //float height = 8.0f;
    //float width = height * 1.467889908256881;
    //for (auto a : objectsList) {
    //    a->getTransform().setScale(width, height);
    //}
    Level::importTransformData(objectsList, "alpha1", false);

    player = new PlayerObject();
    player->getTransform().setScale(4.166f, 2.5f);
    player->getColliderComponent()->getTransform().translate(0.0f, -0.44f);
    player->getColliderComponent()->setDimension(0.25f, 0.65f);
    objectsList.emplace_back(player);

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

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct5::levelUpdate() {
    updateObjects(objectsList);

    GameEngine::getInstance()->getRenderer()->updateCamera();


    // Placeholder death logic
    for (std::list<DrawableObject*>::iterator itr = objectsList.begin(); itr != objectsList.end(); ++itr) {
        EnemyObject* enemy = dynamic_cast<EnemyObject*>(*itr);
        if (enemy != NULL) {
            if (enemy->getHealth() <= 0) {
                DrawableObject::destroyObject(enemy);
            }
        }
    }

    //UIobject->updateUI(*player, camPos);
}

void LevelAct5::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct5::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();

    delete UIobject;
}

void LevelAct5::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct5::handleKey(InputManager& input) {
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
}