#include "LevelAct1.h"

void LevelAct1::levelLoad() {
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

void LevelAct1::levelInit() {
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    float pictureWidth = 1920.0f;
    float pictureHeight = 360.0f;

    ParallaxObject* sky = new ParallaxObject(0.0f, 0.0f, 100.0f, false, player, true, pictureWidth, pictureHeight);
    sky->setTexture("../Resource/Texture/Act1/City_P01_Sky.png");
    objectsList.emplace(objectsList.begin(), sky);

    ParallaxObject* city1 = new ParallaxObject(0.0f, 0.0f, 50.0f, false, player, true, pictureWidth, pictureHeight);
    city1->setTexture("../Resource/Texture/Act1/City_P02_City1.png");
    objectsList.emplace_back(city1);

    ParallaxObject* city2 = new ParallaxObject(0.0f, 0.0f, 35.0f, false, player, true, pictureWidth, pictureHeight);
    city2->setTexture("../Resource/Texture/Act1/City_P03_City2.png");
    objectsList.emplace_back(city2);

    ParallaxObject* city3 = new ParallaxObject(0.0f, -0.5f, 30.0f, false, player, true, pictureWidth, pictureHeight);
    city3->setTexture("../Resource/Texture/Act1/City_P04_City3.png");
    objectsList.emplace_back(city3);

    ParallaxObject* newsBoard = new ParallaxObject(0.0f, -0.5f, 20.0f, false, player, true, pictureWidth, pictureHeight);
    newsBoard->setTexture("../Resource/Texture/Act1/City_P05_NewsBoardandBins.png");
    objectsList.emplace_back(newsBoard);

    ParallaxObject* lightPole = new ParallaxObject(0.0f, -0.5f, 15.0f, false, player, true, pictureWidth, pictureHeight);
    lightPole->setTexture("../Resource/Texture/Act1/City_P09_Lightpole.png");
    objectsList.emplace_back(lightPole);

    ParallaxObject* car = new ParallaxObject(0.0f, -0.5f, 10.0f, false, player, true, pictureWidth, pictureHeight);
    car->setTexture("../Resource/Texture/Act1/City_P10_Car.png");
    objectsList.emplace_back(car);

    ParallaxObject* fog = new ParallaxObject(0.0f, 1.0f, 60.0f, false, player, true, pictureWidth, pictureHeight);
    fog->setTexture("../Resource/Texture/Act1/City_P13_Fog.png");
    objectsList.emplace_back(fog);

    ParallaxObject* ground = new ParallaxObject(0.0f, -0.5f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    ground->setTexture("../Resource/Texture/Act1/City_P11_Ground.png");
    objectsList.emplace_back(ground);


	//float height = 7.0f; 
	//float width = height * 5.3333333f;
 //   for (auto a : objectsList) {
 //       a->getTransform().setScale(width, height);
 //   }


 //   lightPole->getTransform().setScale(47.999999f, 9.0f);

 //   sky->getTransform().setScale(500.f, 500.f);

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

    ParallaxObject* pole = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    pole->setTexture("../Resource/Texture/Act1/City_P12_FGPole.png");
    objectsList.emplace_back(pole);

    //UIobject->initUI(objectsList);

    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitBool(true);
    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitMinMax(-5.0f, 60.0f);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct1::levelUpdate() {
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

void LevelAct1::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct1::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();

    delete UIobject;
}

void LevelAct1::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct1::handleKey(InputManager& input) {
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