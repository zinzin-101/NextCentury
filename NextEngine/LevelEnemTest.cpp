#include "CollisionHandler.h"
#include "LevelEnemTest.h"
#include "MapLoader.h"

static void loadMap(MapLoader& loader, int i, list<DrawableObject*>& objList, PlayerObject*& player);
static void appendMap(MapLoader& loader, int i, list<DrawableObject*>& objList, PlayerObject*& player);
static ostream& operator<<(ostream& out, glm::vec3 pos);

void LevelEnemTest::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);
    
}

void LevelEnemTest::levelInit() {
    GameEngine::getInstance()->getRenderer()->setClearColor(0.2, 0.2, 0.2);
    GameEngine::getInstance()->setDrawArea(-960.0f, 960.0f, -540.0f, 540.0f);  // IMPORTANT ORTHO IS NOW 1920 / 1080 (I ALSO PUT IT BACK TO 16/9 IN LEVEL FREE() )

    AudioEngine* auds = new AudioEngine();
    auds->init("../Resource/Audio/SoundEffect", "../Resource/Audio/Music");
    audio = auds;
    //audio->playMusicByIndex(1, 0);
    ////////////////////////////////////

    //ParallaxObject* background = new ParallaxObject(0.0f ,0.0f, 0.0f, false, player, true);
    //background->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P09_Sky.png");
    ////background->getTransform().setScale(glm::vec3(640.0f, 436.0f, 0)); // Adjust the scale to fit the screen
    ////background->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    //objectsList.emplace(objectsList.begin(), background);

    //ParallaxObject* spaceShip = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true);
    //spaceShip->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P08_Spaceship.png");
    ////spaceShip->getTransform().setScale(glm::vec3(640.0f, 436.0f, 0));
    //objectsList.push_back(spaceShip);

    //ParallaxObject* Mountain1 = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true);
    //Mountain1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P06_Mountain01.png");
    ////Mountain1->getTransform().setScale(glm::vec3(640.0f, 436.0f, 0));
    //objectsList.push_back(Mountain1);

    //ParallaxObject* Mountain2 = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true);
    //Mountain2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P07_Mountain02.png");
    ////Mountain2->getTransform().setScale(glm::vec3(640.0f, 436.0f, 0));
    //objectsList.push_back(Mountain2);

    //ParallaxObject* MidGround1 = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true);
    //MidGround1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P05_MidGround01.png");
    ////MidGround1->getTransform().setScale(glm::vec3(640.0f, 436.0f, 0));
    //objectsList.push_back(MidGround1);

    //ParallaxObject* MidGround2 = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true);
    //MidGround2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P04_MidGround02.png");
    ////MidGround2->getTransform().setScale(glm::vec3(640.0f, 436.0f, 0));
    //objectsList.push_back(MidGround2);

    ParallaxObject* MidGround3 = new ParallaxObject(0.0f, 0.0f, 500.0f, false, player, true);
    MidGround3->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P03_MidGround03.png");
    //MidGround3->getTransform().setScale(glm::vec3(640.0f, 436.0f, 0));
    objectsList.push_back(MidGround3);

    /////////////////////////////
    mapLoader.readData("mapEnemTest.txt");
    mapLoader.appendDataToScene(objectsList, player);
    /////////////////////////////

    //ParallaxObject* Ground = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true);
    //Ground->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P02_Ground.png");
    ////Ground->getTransform().setScale(glm::vec3(640.0f, 436.0f, 0));
    //objectsList.push_back(Ground);

    //ParallaxObject* Fog = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true);
    //Fog->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P01_Fog.png");
    ////Fog->getTransform().setScale(glm::vec3(640.0f, 436.0f, 0));
    //objectsList.push_back(Fog);

    if (player != nullptr) {
        cout << "player not null" << endl;
    }

    cout << "player pos: " << player->getTransform().getPosition() << endl;
    player->setName("Player");
    player->setTexture("../Resource/Texture/SIZENextCentury_Player_Idle-Sheet.png");
    player->initAnimation(6, 1);
    player->getTransform().setScale(1, 1);

    EnemyInfo enemyInfo;
    enemyInfo.health = 10;
    enemyInfo.aggroRange = 80;
    enemyInfo.attackRange = 8;
    EnemyObject* enemy = new EnemyObject(enemyInfo);
    enemy->setTexture("../Resource/Texture/incineratorSizeFlip.png");
    enemy->setName("enem");
    enemy->initAnimation(6, 2);
    enemy->getAnimationComponent()->addState("Idle", 0, 6);
    enemy->getAnimationComponent()->addState("Attacking", 1, 5);
    enemy->getTransform().setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
    enemy->addColliderComponent();
    enemy->addPhysicsComponent();
    enemy->getPhysicsComponent()->setDrag(0.3f);
    enemy->setDrawCollider(true);
    objectsList.push_back(enemy);
    enemy->getPhysicsComponent()->setEnableGravity(true);
    enemy->findTarget("Player", objectsList);
    enemy->getAnimationComponent()->setState("Idle");
    //enemy->setCurrentState();

    startObjects(objectsList);
}
void LevelEnemTest::levelUpdate() {
    updateObjects(objectsList);

    for (DrawableObject* obj : objectsList) {
        obj->update(objectsList);
    }
    handleObjectCollision(objectsList);
    //cout << enemy->getIsStun();


    glm::vec3 playerPosition = player->getTransform().getPosition();
    GameEngine::getInstance()->getRenderer()->updateCamera(playerPosition);
}

void LevelEnemTest::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
}

void LevelEnemTest::levelFree() {
    GameEngine::getInstance()->setDrawArea(-8.0f, 8.0f, -4.5f, 4.5f);
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
}

void LevelEnemTest::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    //cout << "Unload Level" << endl;
}

void LevelEnemTest::handleKey(char key) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    //cout << dt << endl;
    switch (key) {
    case ' ': if (player->getColliderComponent()->getCollisionFlag() && COLLISION_DOWN) player->getPhysicsComponent()->setVelocity(glm::vec2(0, 25)); break;
    case 'w': player->getTransform().translate(glm::vec3(0, 5, 0) * dt); break;
    case 's': player->getTransform().translate(glm::vec3(0, -5, 0) * dt); break;
    case 'a': player->getTransform().translate(glm::vec3(-500, 0, 0) * dt); break;
    case 'd': player->getTransform().translate(glm::vec3(500, 0, 0) * dt); break;
    //case 'f': enemy->addStatus(LivingEntity::Status(LivingEntity::MOODENG, 1.f)); break;
    case 'f': GameEngine::getInstance()->getRenderer()->toggleViewport(); break;
    case 'c': player->getColliderComponent()->setTrigger(!player->getColliderComponent()->isTrigger()); break;
    case 'g': player->getPhysicsComponent()->setEnableGravity(!player->getPhysicsComponent()->isGravityEnable()); break;
    case 'q': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_QUIT; ; break;
    case 'r': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART; ; break;
    case 'e': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_LEVEL2; ; break;
    case 'z': GameEngine::getInstance()->getRenderer()->increaseZoomRatio(10.0f); break;
    case 'x': GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(10.0f); break;
    case '1': loadMap(mapLoader, 1, objectsList, player); break;
    case '2': loadMap(mapLoader, 2, objectsList, player); break;
    case '3': loadMap(mapLoader, 3, objectsList, player); break;
    case '4': appendMap(mapLoader, 5, objectsList, player); break;
    }
}

void LevelEnemTest::handleMouse(int type, int x, int y) {
    /// Will be implemented in inherited level when used ///
}

void LevelEnemTest::handleAnalogStick(int type, float amount) {
    
    if (type == 0) { // x axis
        player->getTransform().translate(glm::vec3(0.3 * amount, 0, 0));
    }
    else if (type == 1) { // y axis
        player->getTransform().translate(glm::vec3(0, -0.3 * amount, 0));
    }
    
}

static void loadMap(MapLoader& loader, int i, list<DrawableObject*>& objList, PlayerObject*& player) {
    string name = "map" + to_string(i) + ".txt";
    loader.readData(name);
    loader.loadDataToScene(objList, player);

    if (player != nullptr) {
        cout << "player not null" << endl;
    }
}

static void appendMap(MapLoader& loader, int i, list<DrawableObject*>& objList, PlayerObject*& player) {
    string name = "map" + to_string(i) + ".txt";
    loader.readData(name);
    loader.appendDataToScene(objList, player);

    if (player != nullptr) {
        cout << "player not null" << endl;
    }
}

static ostream& operator<<(ostream& out, glm::vec3 pos) {
    out << pos.x << "," << pos.y;
    return out;
}