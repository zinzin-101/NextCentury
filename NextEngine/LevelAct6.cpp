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
    saveCurrentGameState();
}

void LevelAct6::levelInit() {
    UIobject = new IngameUI();
    player = new PlayerObject();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    float pictureWidth = 2880.0f;
    float pictureHeight = 360.0f;

    ParallaxObject* sky = new ParallaxObject(32.0f, 0.0f, 550.0f, false, player, true, pictureWidth, pictureHeight);
    sky->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P15_Sky.png");
    objectsList.emplace(objectsList.begin(), sky);

    ParallaxObject* mountain2 = new ParallaxObject(32.0f, 0.0f, 250.0f, false, player, true, pictureWidth, pictureHeight);
    mountain2->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P13_Mountain02.png");
    objectsList.emplace_back(mountain2);

    ParallaxObject* mountain1 = new ParallaxObject(32.0f, 0.0f, 200.0f, false, player, true, pictureWidth, pictureHeight);
    mountain1->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P12_Mountain01.png");
    objectsList.emplace_back(mountain1);

    ParallaxObject* midGround6 = new ParallaxObject(32.0f, 0.0f, 100.0f, false, player, true, pictureWidth, pictureHeight);
    midGround6->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P11_MidGround06.png");
    objectsList.emplace_back(midGround6);

    ParallaxObject* midGround5 = new ParallaxObject(32.0f, 0.0f, 10.0f, false, player, false, pictureWidth, pictureHeight);
    midGround5->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P10_MidGround05.png");
    objectsList.emplace_back(midGround5);
    
    ParallaxObject* midGround4 = new ParallaxObject(32.0f, 0.0f, 10.0f, false, player, false, pictureWidth, pictureHeight);
    midGround4->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P09_MidGround04.png");
    objectsList.emplace_back(midGround4);

    ParallaxObject* midGround3 = new ParallaxObject(32.0f, 0.0f, 10.0f, false, player, false, pictureWidth, pictureHeight);
    midGround3->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P08_MidGround03.png");
    objectsList.emplace_back(midGround3);

    ParallaxObject* midGround2 = new ParallaxObject(32.0f, 0.0f, 10.0f, false, player, false, pictureWidth, pictureHeight);
    midGround2->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P07_MidGround02.png");
    objectsList.emplace_back(midGround2);

    ParallaxObject* midGround1 = new ParallaxObject(32.0f, 0.0f, 0.0f, false, player, false, pictureWidth, pictureHeight);
    midGround1->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P06_MidGround01.png");
    objectsList.emplace_back(midGround1);
    doorKeepTrack = midGround1;
    //cout << midGround1->getTransform().getScale().x << " : " << midGround1->getTransform().getScale().y << endl;

    ParallaxObject* speaker = new ParallaxObject(32.0f, 0.0f, 0.0f, false, player, false, pictureWidth, pictureHeight);
    speaker->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P04_Speaker.png");
    objectsList.emplace_back(speaker);

    ParallaxObject* sign = new ParallaxObject(32.0f, 0.0f, 0.0f, false, player, false, pictureWidth, pictureHeight);
    sign->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P03_ProtestSigns.png");
    objectsList.emplace_back(sign);

    ParallaxObject* barr = new ParallaxObject(32.0f, 0.0f, 0.0f, false, player, false, pictureWidth, pictureHeight);
    barr->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P02_Barrier.png");
    objectsList.emplace_back(barr);

    ParallaxObject* car = new ParallaxObject(32.0f, 0.0f, 0.0f, false, player, false, pictureWidth, pictureHeight);
    car->setTexture("../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P01_Car.png");
    objectsList.emplace_back(car);

    ParallaxObject* ground = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true, 960.0f, pictureHeight);
    ground->setTexture("../Resource/Texture/Act6/IMGT_P01_Ground.png");
    objectsList.emplace_back(ground);

    //for (auto a : objectsList) {
    //    a->getTransform().setScale(17.77778f, 10.0f);
    //    a->getTransform().setPosition(0.0f, -1.0f);
    //}

    Level::importTransformData(objectsList, "act6", false);

    //player->getTransform().setScale(4.166f, 2.5f);
    //player->getColliderComponent()->getTransform().translate(0.0f, -0.44f);
    //player->getColliderComponent()->setDimension(0.25f, 0.65f);

    door = new InteractableObject("../Resource/Texture/StoryStuff/NeonBoardDescription.txt", player, "../Resource/Texture/Act6/Scene6_Enemybase/ENMB_P05_Door3.png", objectsList);
    door->getTransform().setScale(0.98f, 1.78f);
    door->getTransform().setPosition(57.01f, -1.35f);
    door->setActive(false);
    objectsList.emplace_back(door);

    player->getTransform().setPosition(glm::vec3(-6.0f, -1.6f, 0.0f));
    objectsList.emplace_back(player);

    vector<glm::vec3> l;
    l.push_back(glm::vec3(27.5f, 0.6f, 0.0f));
    l.push_back(glm::vec3(33.5f, 0.6f, 0.0f));
    l.push_back(glm::vec3(34.5f, 0.6f, 0.0f));
    l.push_back(glm::vec3(36.0f, 0.7f, 0.0f));
    chat1 = new ChatBubble("../Resource/Texture/StoryStuff/chat1Act6.txt", player, l, objectsList);
    chat2 = new ChatBubble("../Resource/Texture/StoryStuff/chat2Act6.txt", player, l, objectsList);

    thought1 = new ProtagThoughts("../Resource/Texture/StoryStuff/protagAct6.txt", player);
    objectsList.emplace_back(thought1);

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    GameEngine::getInstance()->getRenderer()->setToggleViewport(false);

    // initializing parallax object
    int count = 1;
    for (DrawableObject* obj : objectsList) {
        ParallaxObject* pObj = dynamic_cast<ParallaxObject*>(obj);
        if (pObj != NULL) {
            pObj->setPlayer(player);
        }

        EnemyObject* eObj = dynamic_cast<EnemyObject*>(obj);
        if (eObj != NULL) {
            eObj->setAggroRange(0.0f);
            if (count == 1) {
                eObj->setIsFacingRight(true);
                enemSet1.emplace_back(eObj);
            }
            else if (count > 3) {
                enemSet2.emplace_back(eObj);
            }
            else {
                enemSet1.emplace_back(eObj);
            }
            count++;
        }
    }

    startObjects(objectsList);

    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -0.2f, 0));
    player->setMaxNumOfPotion(1);

    

    camTarget = new SimpleObject();
    camTarget->getTransform().setPosition(glm::vec3(34.0f, 0.0f, 0.0f));
    camTarget->getTransform().setScale(0.0f, 0.0f);
    objectsList.emplace_back(camTarget);

    set1Block = new ColliderObject();
    set1Block->getTransform().setPosition(glm::vec3(40.8f, 0.0f, 0.0f));
    set1Block->getTransform().setScale(1.0f, 10.0f);
    objectsList.emplace_back(set1Block);

    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    UIobject->initUI(objectsList);

    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitBool(true);
    GameEngine::getInstance()->getRenderer()->getCamera()->setDeadLimitMinMax(-5.0f, 80.75f);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct6::levelUpdate() {
    std::cout << "kill count " << killCount << std::endl;

    updateObjects(objectsList);
    GameEngine::getInstance()->getRenderer()->updateCamera();
    

    if (player->getTransform().getPosition().x > 24.8f && !set1FightDone) {
        GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(camTarget);
        //GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(32.0f, 0.0f, 0.0f));
        chat1->runChat(objectsList);
        if (!chat1->hasEnded()) {
            isStop = true;
        }
        else {
            chat2->runChat(objectsList);
            if (!chat2->hasEnded()) {
                turnTime -= GameEngine::getInstance()->getTime()->getDeltaTime();
                if (turnTime < 0.0f) {
                    enemSet1.front()->setIsFacingRight(false);
                }
                isStop = true;
            }
            else {
                thought1->activateDialogue();
                for (EnemyObject* eObj : enemSet1) {
                    eObj->setAggroRange(15.0f); // veri high range cuz it don't matter no more :(
                }
                isStop = false;
            }
        }
        
    }
    else {
        GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    }

    if (killCount == 3) {
        set1Block->setActive(false);
        //set1Block->getColliderComponent()->setEnableCollision(false);
        set1FightDone = true;
        for (EnemyObject* eObj : enemSet2) {
            eObj->setAggroRange(10.0f); // nvm it kinda mattered :)
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
        //cout << "WHAT" << endl;
        door->setActive(true);
    }

    if (end) {
        timefade -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (timefade < 0.0f) {
            loadNextLevel();
        }
    }
    //cout << set2FightDone << endl;
    UIobject->updateUI(player);
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

    delete chat1;
    delete chat2;

    delete UIobject;
}

void LevelAct6::levelUnload() {
    GameEngine::getInstance()->stopMusic();
    GameEngine::getInstance()->stopSfx();
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

    if (!isStop) {
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

        if (input.getButtonDown(SDLK_e)) {
            if (door->getIsClickable()) {
                //sfx
                //transition
                fb->FadeToBlack();
                end = true;
            }
        }
    }
	UIobject->handleInput(input,player);
}

void LevelAct6::signalFromEngine() {
    this->killCount++;
}