#include "LevelAct3.h"

void LevelAct3::levelLoad() {
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

void LevelAct3::levelInit() {
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    player = new PlayerObject();

    float pictureWidth = 640.0f;
    float pictureHeight = 360.0f;
    float scaleX = (pictureWidth / pictureHeight) * 9.0f;
    float scaleY = 9.0f;

    TexturedObject* backGround = new TexturedObject();
    backGround->getTransform().setPosition(0.0f, 0.0f);
    backGround->setTexture("../Resource/Texture/Act3/MCR_P01_RoomBase.png");
    backGround->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(backGround);

    board = new InteractableObject("../Resource/Texture/StoryStuff/BoardAct3.txt", player, "../Resource/Texture/Act3/BoardSheet.png", objectsList);
    board->getTransform().setPosition(glm::vec3(2.55f, -0.3f, 0.0f));
    board->getTransform().setScale(glm::vec3(1.5f, 0.91f, 0.0f));
    objectsList.emplace_back(board);
    board->getBackdropText()->getTransform().setScale(glm::vec3(7.0f, 3.0f, 0.0f));
    board->setPosOffset(glm::vec3(0.25f, 0.0f, 0.0f));

    TexturedObject* midGround1 = new TexturedObject();
    midGround1->getTransform().setPosition(0.0f, 0.0f);
    midGround1->setTexture("../Resource/Texture/Act3/MCR_P02_Mid1.png");
    midGround1->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(midGround1);

    door = new InteractableObject("../Resource/Texture/StoryStuff/NeonBoardDescription.txt", player, "../Resource/Texture/Act3/DoorSheet.png", objectsList);
    door->getTransform().setPosition(glm::vec3(-1.2f, -1.3f, 0.0f));
    door->getTransform().setScale(glm::vec3(1.3f, 2.1f, 0.0f));
    objectsList.emplace_back(door);

    shelf = new InteractableObject("../Resource/Texture/StoryStuff/NeonBoardDescription.txt", player, "../Resource/Texture/Act3/ShelfSheet.png", objectsList);
    shelf->getTransform().setPosition(glm::vec3(0.8f, -1.1f, 0.0f));
    shelf->getTransform().setScale(glm::vec3(1.25f, 2.57f, 0.0f));
    objectsList.emplace_back(shelf);

    TexturedObject* midGround2 = new TexturedObject();
    midGround2->getTransform().setPosition(0.0f, 0.0f);
    midGround2->setTexture("../Resource/Texture/Act3/MCR_P04_Mid2.png");
    midGround2->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(midGround2);

    TexturedObject* windowLight = new TexturedObject();
    windowLight->getTransform().setPosition(0.0f, 0.0f);
    windowLight->setTexture("../Resource/Texture/Act3/MCR_P03_WindowLightray.png");
    windowLight->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(windowLight);

    TexturedObject* docs = new TexturedObject();
    docs->getTransform().setPosition(0.0f, 0.0f);
    docs->setTexture("../Resource/Texture/Act3/MCR_P05_Doc.png");
    docs->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(docs);

    desk = new InteractableObject("../Resource/Texture/StoryStuff/DeskAct3.txt", player, "../Resource/Texture/Act3/DeskSheet.png", objectsList);
    desk->getTransform().setPosition(glm::vec3(3.5f, -1.49f, 0.0f));
    desk->getTransform().setScale(glm::vec3(2.5f, 1.77f, 0.0f));
    objectsList.emplace_back(desk);

    chest = new InteractableObject("../Resource/Texture/StoryStuff/NeonBoardDescription.txt", player, "../Resource/Texture/Act3/ChestSheet.png", objectsList);
    chest->getTransform().setPosition(glm::vec3(-3.75f, -2.25f, 0.0f));
    chest->getTransform().setScale(glm::vec3(0.5f, 0.2f, 0.0f));
    objectsList.emplace_back(chest);

    done = new ProtagThoughts("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/done.txt", player);
    objectsList.emplace_back(done);

    repeatNotDone = new ProtagThoughts("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/repeat.txt", player);
    objectsList.emplace_back(repeatNotDone);

    medicineOnTable = new GotItemText("- Got Medicine", player, objectsList);
    medicineOnTable->setAppearPos(4.35f, 0.0f);
    fakePassport = new GotItemText("- Got Forged permit", player, objectsList);
    fakePassport->setAppearPos(4.0f, 0.0f);
    magicPistol = new GotItemText("- Magic Pistol", player, objectsList);
    magicPistol->setAppearPos(4.0f, 0.0f);
    duoBlade = new GotItemText("- Duo Blade", player, objectsList);
    duoBlade->setAppearPos(4.0f, -1.0f);

    Level::importTransformData(objectsList, "act3", false);

    //player->getTransform().setScale(4.166f, 2.5f);
    //player->getColliderComponent()->getTransform().translate(0.0f, -0.44f);
    //player->getColliderComponent()->setDimension(0.25f, 0.65f);
    player->getTransform().setPosition(glm::vec3(-1.0f, -1.0f, 0.0f));
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

    TexturedObject* backGround2 = new TexturedObject();
    backGround2->getTransform().setPosition(0.0f, 0.0f);
    backGround2->setTexture("../Resource/Texture/Act3/MCR_P06_BlackFrame.png");
    backGround2->getTransform().setScale(scaleX, scaleY);
    objectsList.emplace_back(backGround2);

    repeatNotDone = new ProtagThoughts("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/repeat.txt", player);
    objectsList.emplace_back(repeatNotDone);
    done = new ProtagThoughts("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/done.txt", player);
    objectsList.emplace_back(done);

    //UIobject->initUI(objectsList);

    fb = new FadeBlack(1.0f);
    objectsList.emplace_back(fb);
    fb->FadeToTransparent();

    GameEngine::getInstance()->getRenderer()->getCamera()->setOffset(glm::vec3(0.0f, -0.5f, 0.0f));
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);

    GameEngine::getInstance()->freezeGameForSeconds(0.5f);

}

void LevelAct3::levelUpdate() {
    updateObjects(objectsList);
    medicineOnTable->update(objectsList);
    fakePassport->update(objectsList);
    magicPistol->update(objectsList);
    duoBlade->update(objectsList);
    //GameEngine::getInstance()->getRenderer()->updateCamera();

    if(done->getDialogueObject()->isEnd){
        if (!end) {
            fb->FadeToBlack();
            end = true;
        }
    }

    if (end) {
        fadetime -= GameEngine::getInstance()->getTime()->getDeltaTime();
        if (fadetime < 0.0f) {
            GameEngine::getInstance()->getStateController()->gameStateNext = (GameState)((GameEngine::getInstance()->getStateController()->gameStateCurr + 1) % 9);
        }
    }

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

void LevelAct3::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct3::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();

    delete UIobject;

    delete medicineOnTable;
    delete fakePassport;
    delete magicPistol;
    delete duoBlade;
}

void LevelAct3::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelAct3::handleKey(InputManager& input) {
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

    if (input.getButtonDown(SDLK_e)) {

        //cout << board->isClickedOnce << " : " << desk->isClickedOnce << " : " << chest->isClickedOnce << " : " << shelf->isClickedOnce << endl;

        if (door->getIsClickable()) {
            if (((board->isClickedOnce && desk->isClickedOnce) && (chest->isClickedOnce && shelf->isClickedOnce))) {
                done->reActivateDialogue("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/done.txt");
            }
            else {
                repeatNotDone->reActivateDialogue("../Resource/Texture/StoryStuff/ProtagThoughtsAct3/repeat.txt");
            }
        }

        if (chest->getIsClickable()) {
            magicPistol->activateAppear();
            duoBlade->activateAppear();
            chest->isClickedOnce = true;
        }

        if (shelf->getIsClickable()) {
            fakePassport->activateAppear();
            shelf->isClickedOnce = true;
        }

        if ((board->getIsClickable() && desk->getIsClickable()) || board->getIsClickable()) {
            board->setDescriptionActive(!board->getDescriptionActive());
            if (board->isClickedOnce && !board->getDescriptionActive()) {
                medicineOnTable->activateAppear();
            }
        }
        else if (desk->getIsClickable()) {
            desk->setDescriptionActive(!desk->getDescriptionActive());
        }
    }
}