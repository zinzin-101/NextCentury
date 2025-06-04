#include "LevelAct4.h"

void LevelAct4::levelLoad() {
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

void LevelAct4::levelInit() {
    GameEngine::getInstance()->playMusic("BGM_Fight.wav", 1);
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    player = new PlayerObject();

    float pictureWidth = 1000.0f;
    float pictureHeight = 360.0f;
    float scaleX = (pictureWidth / pictureHeight) * 9.0f;
    float scaleY = 9.0f;

	TexturedObject* backGround1 = new TexturedObject();
    backGround1->setTexture("../Resource/Texture/Act2/RSDT_P01_Background01.png");
    backGround1->getTransform().setPosition(0.0f, 0.0f);
    backGround1->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace(objectsList.begin(), backGround1);

    TexturedObject* backGround2 = new TexturedObject();
    backGround2->setTexture("../Resource/Texture/Act2/RSDT_P02_Background02.png");
    backGround2->getTransform().setPosition(0.0f, 0.0f);
    backGround2->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(backGround2);

    TexturedObject* midGround1 = new TexturedObject();
    midGround1->setTexture("../Resource/Texture/Act2/RSDT_P03_MidGround01.png");
    midGround1->getTransform().setPosition(0.0f, -0.5f);
    midGround1->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(midGround1);

    TexturedObject* midGround2 = new TexturedObject();
    midGround2->setTexture("../Resource/Texture/Act2/RSDT_P04_MidGround02.png");
    midGround2->getTransform().setPosition(0.0f, -0.5f);
    midGround2->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(midGround2);

    TexturedObject* midGround3 = new TexturedObject();
    midGround3->setTexture("../Resource/Texture/Act2/RSDT_P06_MidGround03.png");
    midGround3->getTransform().setPosition(0.0f, -0.5f);
    midGround3->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(midGround3);


    TexturedObject* barrier = new TexturedObject();
    barrier->setTexture("../Resource/Texture/Act2/RSDT_P07_Barrier.png");
    barrier->getTransform().setPosition(0.0f, -0.5f);
    barrier->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(barrier);

    TexturedObject* foreGround = new TexturedObject();
    foreGround->setTexture("../Resource/Texture/Act2/RSDT_P08_Foreground01.png");
    foreGround->getTransform().setPosition(0.0f, 0.0f);
    foreGround->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(foreGround);

    TexturedObject* ground = new TexturedObject();
    ground->setTexture("../Resource/Texture/Act1/City_P11_Ground.png");
    ground->getTransform().setPosition(0.0f, -0.5f);
    ground->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(ground);

    Level::importTransformData(objectsList, "act4", false);

    player->getTransform().setPosition(glm::vec3(8.9f, -1.6f, 0.0f));
    objectsList.emplace_back(player);

    vector<glm::vec3> l;
    l.push_back(glm::vec3(0.5f, 0.0f, 0.0f));
    chat1 = new ChatBubble("../Resource/Texture/StoryStuff/chat1Act4.txt", player, l, objectsList);

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(4.5f, 0.0f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(false);

    for (DrawableObject* obj : objectsList) {
        ParallaxObject* pObj = dynamic_cast<ParallaxObject*>(obj);
        if (pObj != NULL) {
            pObj->setPlayer(player);
        }
        EnemyObject* eObj = dynamic_cast<EnemyObject*>(obj);
        if (eObj != NULL) {
            enem = eObj;
        }
    }
    enem->setAggroRange(0.0f);
    enem->setIsFacingRight(true);

    startObjects(objectsList);

    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -0.2f, 0));
    player->setIsFacingRight(false);

    UIobject->initUI(objectsList);

    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct4::levelUpdate() {
    updateObjects(objectsList);
    chat1->runChat(objectsList);

    if (!chat1->hasEnded()) {
        isStop = true;
        //GameEngine::getInstance()->getTime()->setTimeScale(0);
    }
    else {
        isStop = false;
        enem->setAggroRange(10.0f);
        //GameEngine::getInstance()->getTime()->setTimeScale(1);
    }
    if (player->getTransform().getPosition().x > 11.5f && killCount == 1) {
        if (!end) {
            fb->FadeToBlack();
            end = true;
        }
        timefade -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (timefade < 0.0f) {
            loadNextLevel();
        }
    }
    // at the very start of the game freeze everything and chatEnemy is "YOU HAVE MAGIC" THEN fight...

    UIobject->updateUI(player);
}

void LevelAct4::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct4::levelFree() {
    delete UIobject;
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
    delete chat1;
    //
}

void LevelAct4::levelUnload() {
    GameEngine::getInstance()->stopMusic();
    GameEngine::getInstance()->stopSfx();
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct4::handleKey(InputManager& input) {
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
			if (input.getButtonDown(SDLK_v)) {
				player->setHealth(0);
			}
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
    }
	UIobject->handleInput(input,player);   
}

void LevelAct4::signalFromEngine() {
    this->killCount++;
}