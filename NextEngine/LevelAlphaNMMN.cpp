#include "LevelAlphaNMMN.h"

void LevelAlphaNMMN::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);
}

void LevelAlphaNMMN::levelInit() {
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    

    ParallaxObject* background = new ParallaxObject(0.0f, 0.0f, 550.0f, false, player, true);
    background->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P09_Sky.png");
    objectsList.emplace(objectsList.begin(), background);

    //ParallaxObject* spaceShip = new ParallaxObject(0.0f, 0.0f, 450.0f, false, player, true); // Should be a moving object
    //spaceShip->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P08_Spaceship.png");
    //objectsList.emplace_back(spaceShip);

    ParallaxObject* Mountain1 = new ParallaxObject(0.0f, 1.5f, 250.0f, false, player, true);
    Mountain1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P06_Mountain01.png");
    objectsList.emplace_back(Mountain1);

    ParallaxObject* Mountain2 = new ParallaxObject(0.0f, 1.25f, 200.0f, false, player, true);
    Mountain2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P07_Mountain02.png");
    objectsList.emplace_back(Mountain2);

    ParallaxObject* MidGround1 = new ParallaxObject(0.0f, 0.75f, 150.0f, false, player, true);
    MidGround1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P05_MidGround01.png");
    objectsList.emplace_back(MidGround1);

    ParallaxObject* MidGround2 = new ParallaxObject(0.0f, 0.0f, 80.0f, false, player, true);
    MidGround2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P04_MidGround02.png");
    objectsList.emplace_back(MidGround2);

    ParallaxObject* MidGround3 = new ParallaxObject(0.0f, 0.0f, 50.0f, false, player, true);
    MidGround3->setTexture("../Resource/Texture/OutskirtParallax/Mid3.png");
    objectsList.emplace_back(MidGround3);

    ParallaxObject* Ground = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true);
    Ground->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P02_Ground.png");
    objectsList.emplace_back(Ground);


    Level::importTransformData(objectsList, "alpha1", false);

    player = new PlayerObject();
    player->getTransform().setScale(4.166f, 2.5f);
    player->getColliderComponent()->getTransform().translate(0.0f, -0.44f);
    player->getColliderComponent()->setDimension(0.25f, 0.65f);
    //player->setDrawCollider(true);
    objectsList.emplace_back(player);


    ParallaxObject* Fog = new ParallaxObject(0.0f, 0.0f, 100.0f, false, player, true);
    Fog->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P01_Fog.png");
    Fog->setRenderOpacity(0.25f);
    objectsList.emplace_back(Fog);

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    GameEngine::getInstance()->getRenderer()->setToggleViewport(false);

    // initializing parallax object
    for (DrawableObject* obj : objectsList) {
        ParallaxObject* pObj = dynamic_cast<ParallaxObject*>(obj);
        if (pObj != NULL) {
            pObj->setPlayer(player);
        }
    }

    Dialogue* d1 = new Dialogue(36, player, true); // Must add to the queue in the order that you'll faced them
    d1->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    d1->addSentence("Let Me tell you a story. \nAbout a man. \na man whos manner than any man. \nmanny the man!!!");
    d1->addSentence("GAN");
    d1->addSentence("GULI");
    d1->addSentence("GULI2");
    objectsList.emplace_back(d1);
    dialogueList.push(d1);

    Dialogue* d2 = new Dialogue(36, player, true); // Must add to the queue in the order that you'll faced them
    d2->getTransform().setPosition(glm::vec3(8.0f, 0.0f, 0.0f));
    d2->addSentence("LIN33");
    d2->addSentence("GAN33");
    d2->addSentence("GULI33");
    d2->addSentence("GULI233");
    objectsList.emplace_back(d2);
    dialogueList.push(d2);
    
    /////////////////
    Dialogue* d4intobj = new Dialogue(16, player, false); // Must add to the queue in the order that you'll faced them
    objectsList.emplace_back(d4intobj);
    InteractableObject* it = new InteractableObject("a box of chocolate", player, d4intobj);
    it->setTexture("../Resource/Texture/BoardNeon.png");
    it->getTransform().setPosition(glm::vec3(5.0f, -2.0f, 0.0f));
    it->initAnimation(2,1);
    it->getAnimationComponent()->addState("idle", 0, 0, 1, true);
    it->getAnimationComponent()->addState("clickAble", 1, 0, 1, true);
    it->getAnimationComponent()->setState("idle");
    objectsList.emplace_back(it);
    interactableList.push_back(it);

    startObjects(objectsList);

    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -0.2f, 0));

    UIobject->initUI(objectsList);

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);



    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAlphaNMMN::levelUpdate() {
    updateObjects(objectsList);
    
    GameEngine::getInstance()->getRenderer()->updateCamera(glm::vec3());
     
    
    GameEngine::getInstance()->getRenderer()->updateCamera(camPos);
    UIobject->updateUI(*player, camPos);
}

void LevelAlphaNMMN::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAlphaNMMN::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();

    delete UIobject;
}

void LevelAlphaNMMN::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAlphaNMMN::handleKey(InputManager& input) {
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
        if (!dialogueList.empty()) {
            Dialogue* currentDialogue = dialogueList.front();
            if (currentDialogue->isDialogueActive) {
                currentDialogue->nextSentence();
                if (currentDialogue->sentences.empty()) {
                    dialogueList.pop(); 
                }
            }
        }

        //InteractableObject* keep;
        for (InteractableObject* keep : interactableList) {
            if (keep->getDescriptionActive()) {
                keep->descriptionText->isDialogueActive = !keep->descriptionText->isDialogueActive;
            }
            else {
                keep->descriptionText->isDialogueActive = false;
            }
        }
    }
}