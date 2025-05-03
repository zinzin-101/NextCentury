#include "LevelAct6.h"

void LevelAct6::levelLoad() {
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

void LevelAct6::levelInit() {
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    TexturedObject* sky = new TexturedObject();
    sky->setTexture("../Resource/Texture/Act6/IMGT_P10_Sky.png");
    objectsList.emplace(objectsList.begin(), sky);

    TexturedObject* mountain1 = new TexturedObject();
    mountain1->setTexture("../Resource/Texture/Act6/IMGT_P07_Mountain01.png");
    objectsList.emplace_back(mountain1);

    TexturedObject* mountain2 = new TexturedObject();
    mountain2->setTexture("../Resource/Texture/Act6/IMGT_P08_Mountain02.png");
    objectsList.emplace_back(mountain2);

    TexturedObject* midGround1 = new TexturedObject();
    midGround1->setTexture("../Resource/Texture/Act6/IMGT_P06_MidGround01.png");
    objectsList.emplace_back(midGround1);

    TexturedObject* midGround2 = new TexturedObject();
    midGround2->setTexture("../Resource/Texture/Act6/IMGT_P05_MidGround02.png");
    objectsList.emplace_back(midGround2);

    TexturedObject* midGround3 = new TexturedObject();
    midGround3->setTexture("../Resource/Texture/Act6/IMGT_P04_MidGround03.png");
    objectsList.emplace_back(midGround3);

    TexturedObject* spaceShip = new TexturedObject();
    spaceShip->setTexture("../Resource/Texture/Act6/IMGT_P09_Spaceship.png");
    objectsList.emplace_back(spaceShip);

    TexturedObject* fence = new TexturedObject();
    fence->setTexture("../Resource/Texture/Act6/IMGT_P03_Fence.png");
    objectsList.emplace_back(fence);

    TexturedObject* watchTower = new TexturedObject();
    watchTower->setTexture("../Resource/Texture/Act6/IMGT_P02_WatchTower.png");
    objectsList.emplace_back(watchTower);

    TexturedObject* ground = new TexturedObject();
    ground->setTexture("../Resource/Texture/Act6/IMGT_P01_Ground.png");
    objectsList.emplace_back(ground);

    for (auto a : objectsList) {
        a->getTransform().setScale(17.77778f, 10.0f);
        a->getTransform().setPosition(0.0f, -1.0f);
    }


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

    //UIobject->initUI(objectsList);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct6::levelUpdate() {
    updateObjects(objectsList);

    GameEngine::getInstance()->getRenderer()->updateCamera(glm::vec3());


    // Placeholder death logic
    for (std::list<DrawableObject*>::iterator itr = objectsList.begin(); itr != objectsList.end(); ++itr) {
        EnemyObject* enemy = dynamic_cast<EnemyObject*>(*itr);
        if (enemy != NULL) {
            if (enemy->getHealth() <= 0) {
                DrawableObject::destroyObject(enemy);
            }
        }
    }

    GameEngine::getInstance()->getRenderer()->updateCamera(camPos);
    //UIobject->updateUI(*player, camPos);
}

void LevelAct6::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct6::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();

    delete UIobject;
}

void LevelAct6::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct6::handleKey(InputManager& input) {
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