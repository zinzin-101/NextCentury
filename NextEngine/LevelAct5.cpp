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
    saveCurrentGameState();
}

void LevelAct5::levelInit() {
    GameEngine::getInstance()->playMusic("BGM_Act2.wav", 1);
    UIobject = new IngameUI();
    player = new PlayerObject();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    float pictureWidth = 960.0f;
    float pictureHeight = 360.0f;

    ParallaxObject* sky = new ParallaxObject(0.0f, 0.0f, 550.0f, false, player, true, pictureWidth, pictureHeight);
    sky->setTexture("../Resource/Texture/Act6/IMGT_P10_Sky.png");
    objectsList.emplace(objectsList.begin(), sky);

    ParallaxObject* mountain1 = new ParallaxObject(0.0f, 0.0f, 250.0f, false, player, true, pictureWidth, pictureHeight);
    mountain1->setTexture("../Resource/Texture/Act6/IMGT_P08_Mountain02.png");
    objectsList.emplace_back(mountain1);

    ParallaxObject* mountain2 = new ParallaxObject(0.0f, 0.0f, 200.0f, false, player, true, pictureWidth, pictureHeight);
    mountain2->setTexture("../Resource/Texture/Act6/IMGT_P07_Mountain01.png");
    objectsList.emplace_back(mountain2);

    ParallaxObject* midGround1 = new ParallaxObject(0.0f, 0.0f, 100.0f, false, player, true, pictureWidth, pictureHeight);
    midGround1->setTexture("../Resource/Texture/Act6/IMGT_P06_MidGround01.png");
    objectsList.emplace_back(midGround1);

    ParallaxObject* midGround2 = new ParallaxObject(2.0f, -0.0f, 15.0f, false, player, true, pictureWidth, pictureHeight);
    midGround2->setTexture("../Resource/Texture/Act6/IMGT_P05_MidGround02.png");
    objectsList.emplace_back(midGround2);

    ParallaxObject* midGround3 = new ParallaxObject(2.0f, -0.0f, 10.0f, false, player, true, pictureWidth, pictureHeight);
    midGround3->setTexture("../Resource/Texture/Act6/IMGT_P04_MidGround03.png");
    objectsList.emplace_back(midGround3);

    TexturedObject* fakeEnem1 = new TexturedObject();
    fakeEnem1->setTexture("../Resource/Texture/Purifier2.png");
    fakeEnem1->initAnimation(6, 8);
    fakeEnem1->getAnimationComponent()->addState("Idle", 0, 0, 6, true);
    fakeEnem1->getAnimationComponent()->setState("Idle");
    fakeEnem1->getTransform().setPosition(glm::vec3(7.5f, -1.35f, 0.0f));
    fakeEnem1->getTransform().setScale(-1.579f, 2.05f);
    objectsList.emplace_back(fakeEnem1);

    TexturedObject* fakeEnem2 = new TexturedObject();
    fakeEnem2->setTexture("../Resource/Texture/Purifier2.png");
    fakeEnem2->initAnimation(6, 8);
    fakeEnem2->getAnimationComponent()->addState("Idle", 0, 0, 6, true);
    fakeEnem2->getAnimationComponent()->setState("Idle");
    fakeEnem2->getTransform().setPosition(glm::vec3(9.5f, 0.2f, 0.0f));
    fakeEnem2->getTransform().setScale(-1.579f, 2.05f);
    objectsList.emplace_back(fakeEnem2);

    ParallaxObject* watchTower = new ParallaxObject(4.2f, -0.6f, 10.0f, false, player, false, 960.0f, 360.0f);
    watchTower->setTexture("../Resource/Texture/Act6/IMGT_P02_WatchTower.png");
    watchTower->getTransform().setScale(18.66f, 7.0f);
    objectsList.emplace_back(watchTower);

    //ParallaxObject* spaceShips = new ParallaxObject(0.0f, -0.0f, 90.0f, false, player, true, pictureWidth, pictureHeight); // SHOULD BE AN OBJECT FLYING ON ITS OWN
    //spaceShips->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P08_Spaceship.png");
    //objectsList.emplace_back(spaceShips);

    TexturedObject* spaceShip = new TexturedObject(); // SHOULD BE AN OBJECT FLYING ON ITS OWN
    spaceShip->setTexture("../Resource/Texture/Act6/IMGT_P09_Spaceship.png");
    spaceShip->getTransform().setScale(13.21f, 9.0f);
    spaceShip->getTransform().setPosition(glm::vec3( -10.0f, 1.0f, 0.0f));
    spS = spaceShip;
    objectsList.emplace_back(spaceShip);

    ParallaxObject* ground = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    ground->setTexture("../Resource/Texture/Act6/IMGT_P01_Ground.png");
    objectsList.emplace_back(ground);

    //float height = 8.0f;
    //float width = height * 1.467889908256881;
    //for (auto a : objectsList) {
    //    a->getTransform().setScale(width, height);
    //}
    Level::importTransformData(objectsList, "act5", false);

    
    //player->getTransform().setScale(4.166f, 2.5f);
    //player->getColliderComponent()->getTransform().translate(0.0f, -0.44f);
    //player->getColliderComponent()->setDimension(0.25f, 0.65f);
    player->getTransform().setPosition(glm::vec3(-6.0f, -1.6f, 0.0f));
    player->setMaxNumOfPotion(1);
    objectsList.emplace_back(player);

    vector<glm::vec3> l;
    l.push_back(glm::vec3(6.5f, 0.5f, 0.0f));
    l.push_back(glm::vec3(7.5f, 2.0f, 0.0f));
    immigrats1 = new ChatBubble("../Resource/Texture/StoryStuff/chat1Act5.txt", player, l, objectsList);

    eToPermit = new ProtagThoughts("../Resource/Texture/StoryStuff/protagAct5.txt", player);
    objectsList.emplace_back(eToPermit);

    immigrats2 = new ChatBubble("../Resource/Texture/StoryStuff/chat2Act5.txt", player, l, objectsList);

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

    UIobject->initUI(objectsList);

    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitBool(true);
    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitMinMax(-5.0f, 40.75f);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);
    GameEngine::getInstance()->getTime()->setTimeScale(1.0f);
}

void LevelAct5::levelUpdate() {
    updateObjects(objectsList);

    GameEngine::getInstance()->getRenderer()->updateCamera();

    if (player->getTransform().getPosition().x > 5.8f) {

        if (!immigrats2->hasEnded()) {
            isStop = true;
        }
        else {
            isStop = false;
        }
        if (!immigrats1->hasEnded()) {
            immigrats1->runChat(objectsList);
        }
        else {
            if (!eToPermit->getDialogueObject()->isEnd) {
                eToPermit->activateDialogue();
            }
            else {
                immigrats2->runChat(objectsList);
            }
        }
    }

    if (player->getTransform().getPosition().x > 17.5f) {
        if (!end) {
            fb->FadeToBlack();
            end = true;
        }
        timefade -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (timefade < 0.0f) {
            loadNextLevel();
        }
    }

    spS->getTransform().setPosition(spS->getTransform().getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));
    UIobject->updateUI(player);
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

    delete immigrats1;
    delete immigrats2;

    delete UIobject;
}

void LevelAct5::levelUnload() {
    GameEngine::getInstance()->stopMusic();
    GameEngine::getInstance()->stopSfx();
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

    if (input.getButtonDown(SDLK_f)) {
        if (eToPermit->getDialogueObject()->isDialogueActive) {
            eToPermit->stopDialogue();
        }
        else {
            immigrats1->skipSentence();
            immigrats2->skipSentence();
        }
    }
    if (input.getButtonDown(SDLK_e)) {
        player->useHealthPotion();
    }
    UIobject->handleInput(input, player);
}