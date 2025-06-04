#include "LevelAct2.h"

void LevelAct2::levelLoad() {
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

void LevelAct2::levelInit() {
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    player = new PlayerObject();
    player->setWieldWeaponSprite(false);

    float pictureWidth = 1000.0f;
    float pictureHeight = 360.0f;

    ParallaxObject* backGround1 = new ParallaxObject(0.0f, 0.0f, 100.0f, false, player, true, pictureWidth, pictureHeight);
    backGround1->setTexture("../Resource/Texture/Act2/RSDT_P01_Background01.png");
    objectsList.emplace(objectsList.begin(), backGround1);

    ParallaxObject* backGround2 = new ParallaxObject(0.0f, 0.0f, 75.0f, false, player, true, pictureWidth, pictureHeight);
    backGround2->setTexture("../Resource/Texture/Act2/RSDT_P02_Background02.png");
    objectsList.emplace_back(backGround2);

    ParallaxObject* midGround1 = new ParallaxObject(0.0f, -0.5f, 50.0f, false, player, true, pictureWidth, pictureHeight);
    midGround1->setTexture("../Resource/Texture/Act2/RSDT_P03_MidGround01.png");
    objectsList.emplace_back(midGround1);

    ParallaxObject* midGround2 = new ParallaxObject(0.0f, -0.5f, 25.0f, false, player, true, pictureWidth, pictureHeight);
    midGround2->setTexture("../Resource/Texture/Act2/RSDT_P04_MidGround02.png");
    objectsList.emplace_back(midGround2);
    doorKeepTrack = midGround2;

    ParallaxObject* midGround3 = new ParallaxObject(0.0f, -0.5f, 12.5f, false, player, true, pictureWidth, pictureHeight);
    midGround3->setTexture("../Resource/Texture/Act2/RSDT_P06_MidGround03.png");
    objectsList.emplace_back(midGround3);

    //ParallaxObject* door = new ParallaxObject(0.0f, -0.5f, 15.0f, false, player, true, pictureWidth, pictureHeight);    I THINK DOOR IS AN INTERACTABLE
    //door->setTexture("../Resource/Texture/Act2/RSDT_P05_Door.png");
    //objectsList.emplace_back(door);

    door = new InteractableObject("../Resource/Texture/StoryStuff/NeonBoardDescription.txt", player, "../Resource/Texture/Act2/DoorAct2.png", objectsList); // don't need text
    door->getTransform().setScale(glm::vec3(1.22f, 2.12f, 0.0f));
    objectsList.emplace_back(door);

    //ParallaxObject* barrier = new ParallaxObject(0.0f, -0.5f, 10.0f, false, player, true, pictureWidth, pictureHeight);
    //barrier->setTexture("../Resource/Texture/Act2/RSDT_P07_Barrier.png");
    //objectsList.emplace_back(barrier);

    ParallaxObject* foreGround = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    foreGround->setTexture("../Resource/Texture/Act2/RSDT_P08_Foreground01.png");
    objectsList.emplace_back(foreGround);

    ParallaxObject* ground = new ParallaxObject(0.0f, -0.5f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    ground->setTexture("../Resource/Texture/Act1/City_P11_Ground.png");
    objectsList.emplace_back(ground);

    Level::importTransformData(objectsList, "act2", false);

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
    }

    startObjects(objectsList);

    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -0.2f, 0));
    player->getTransform().setPosition(glm::vec3(-6.0f, -1.6f, 0.0f));

    UIobject->initUI(objectsList);
    UIobject->setDeactivateGunUI(true);
    UIobject->setDeactivatePotionUI(true);

    start = new ProtagThoughts("../Resource/Texture/StoryStuff/ProtagThoughtsAct2/start.txt", player);
    objectsList.emplace_back(start);

    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitBool(true);
    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitMinMax(-5.0f, 37.5f);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);
}

void LevelAct2::levelUpdate() {
    updateObjects(objectsList);

    GameEngine::getInstance()->getRenderer()->updateCamera();

    door->getTransform().setPosition(glm::vec3(34.01f + doorKeepTrack->getTransform().getPosition().x, -1.82f, 0.0f));
    if (player->getTransform().getPosition().x > -3.0f) {
        start->activateDialogue();
    }

    if (end) {
        timefade -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (timefade < 0.0f) {
            loadNextLevel();
        }
    }

    UIobject->updateUI(player);
}

void LevelAct2::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct2::levelFree() {
    GameEngine::getInstance()->stopMusic();
    GameEngine::getInstance()->stopSfx();
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();

    delete UIobject;
}

void LevelAct2::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct2::handleKey(InputManager& input) {
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

    if (input.getButtonDown(SDLK_e)) {
        if (door->getIsClickable()) {
            //sfx
            //transition
            fb->FadeToBlack();
            end = true;
        }
    }
    UIobject->handleInput(input, player);
}