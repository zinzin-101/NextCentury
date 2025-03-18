#include "CollisionHandler.h"
#include "EnemyObject.h"
#include "HitScanDamage.h"
#include "RayObject.h"
#include "LevelPrototype.h"
#include "LightSource.h"
#include "DamageCollider.h"

static ostream& operator<<(ostream& out, glm::vec3 pos);

void LevelPrototype::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);
}

void LevelPrototype::levelInit() {
    //GameEngine::getInstance()->setDrawArea(-8.0f, 960.0f, -540.0f, 540.0f);
    EnemyInfo enemyInfo = EnemyInfo("EnemyNormal", 5, MovementInfo(3, 25), 30, 3.0f, 2.0f, 1.0f);
    mapLoader.addEnemyType(EnemyType::NORMAL, enemyInfo);
    map<EnemyType, EnemyInfo>& enemyMap = mapLoader.getEnemyTypeMap();
    for (auto pair : enemyMap) {
        cout << "test loop" << endl;
        cout << pair.first << " , " << pair.second.name << endl;    
    }

    ParallaxObject* background = new ParallaxObject(0.0f, 7.3f, 550.0f, false, player, true);
    background->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P09_Sky.png");
    objectsList.emplace(objectsList.begin(), background);

    ParallaxObject* spaceShip = new ParallaxObject(0.0f, 7.3f, 450.0f, false, player, true);
    spaceShip->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P08_Spaceship.png");
    objectsList.push_back(spaceShip);

    ParallaxObject* Mountain1 = new ParallaxObject(0.0f, 7.3f, 350.0f, false, player, true);
    Mountain1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P06_Mountain01.png");
    objectsList.push_back(Mountain1);

    ParallaxObject* Mountain2 = new ParallaxObject(0.0f, 7.3f, 200.0f, false, player, true);
    Mountain2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P07_Mountain02.png");
    objectsList.push_back(Mountain2);

    ParallaxObject* MidGround1 = new ParallaxObject(0.0f, 7.3f, 150.0f, false, player, true);
    MidGround1->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P05_MidGround01.png");
    objectsList.push_back(MidGround1);

    ParallaxObject* MidGround2 = new ParallaxObject(0.0f, 7.3f, 80.0f, false, player, true);
    MidGround2->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P04_MidGround02.png");
    objectsList.push_back(MidGround2);

    ParallaxObject* MidGround3 = new ParallaxObject(0.0f, 7.3f, 50.0f, false, player, true);
    MidGround3->setTexture("../Resource/Texture/OutskirtParallax/Mid3.png");
    objectsList.push_back(MidGround3);

    ParallaxObject* Ground = new ParallaxObject(0.0f, 7.3f, 0.0f, false, player, true);
    Ground->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P02_Ground.png");
    objectsList.push_back(Ground);

    mapLoader.readData("prototypemap.txt");
    mapLoader.appendDataToScene(objectsList, player);

    ParallaxObject* Fog = new ParallaxObject(0.0f, 7.3f, 100.0f, false, player, true);
    Fog->setTexture("../Resource/Texture/OutskirtParallax/OSKT_P01_Fog.png");
    objectsList.push_back(Fog);

    marker = new SimpleObject();
    marker->setName("marker");
    marker->getTransform().setScale(0.2f);
    marker->setColor(0, 0, 0);
    objectsList.emplace_back(marker);

    PlayerInfo playerInfo = PlayerInfo("Player", 10, MovementInfo(5, 25));
    startObjects(objectsList);
    initPlayer(player, playerInfo);

    for (DrawableObject* obj : objectsList) {
        cout << obj->getName() << endl;
        EnemyObject* enemy = dynamic_cast<EnemyObject*>(obj);
        if (enemy != NULL) {
            enemy->setDamage(10);
        }
    }

   /* healthBar = new SimpleObject();
    healthBar->setColor(1.0f, 0.0f, 0.0f);
    healthBar->getTransform().setScale(glm::vec3(2.0f, 0.2f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(player->getTransform().getPosition().x,
        player->getTransform().getPosition().y + 1.0f, 0.0f));
    objectsList.push_back(healthBar);*/

    for (DrawableObject* obj : objectsList) {
        EnemyObject* enemy = dynamic_cast<EnemyObject*>(obj);
        if (enemy != NULL) {
            enemy->setTarget(player);
            enemy->setHealth(37);
            //enemy->setCanAttack(false); // debug
            enemy->setDrawCollider(true); // for debugging
            cout << "enemy found" << endl;

            enemy->getTransform().scales(2);
            enemy->getColliderComponent()->getTransform().translate(0.0f, -1.0f);
            enemy->getColliderComponent()->setDimension(0.5f, 0.5f);
            enemy->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -1.0f, 0));
            enemy->getDamageCollider()->getTransform().scales(2);
        }
    }

    player->setHealth(100);
    viewMarker = false;
    //marker->addColliderComponent();
    //marker->addPhysicsComponent();
    //marker->getPhysicsComponent()->setEnableGravity(false);
    //marker->getColliderComponent()->setTrigger(true);
    //marker->setDrawCollider(true);
    //marker->getColliderComponent()->getTransform().setScale(2.0f);
    //marker->getColliderComponent()->setDimension(50, 50);

    ray = new RayObject(glm::vec3(), glm::vec3(1, 1, 0), 2);
    //ray = new HitScanDamage<EnemyObject>(player->getTransform().getPosition(), glm::vec3(1, 1, 0), 4, 1, 9999.0f);
    //objectsList.emplace_back(ray);
    ray->setDrawCollider(true);
    ray->setName("ray");

    player->getTransform().scales(2);
    player->getColliderComponent()->getTransform().translate(0.0f, -1.0f);
    player->getColliderComponent()->setDimension(0.5f, 0.5f);
    player->getDamageCollider()->setFollowOffset(glm::vec3(1.0f, -1.0f, 0));
    player->getDamageCollider()->getTransform().scales(2);

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(player);
    GameEngine::getInstance()->getRenderer()->toggleViewport();

    LightSource* testLight = new LightSource(1.0f, 4.0f);
    testLight->getTransform().setPosition(player->getTransform().getPosition());
    testLight->getTransform().translate(0, 1);
    objectsList.emplace_back(testLight);
}

void LevelPrototype::levelUpdate() {
    updateObjects(objectsList);
    glm::vec3 followPos = viewMarker ? marker->getTransform().getPosition() : player->getTransform().getPosition();
    GameEngine::getInstance()->getRenderer()->updateCamera(followPos);
    if (ray != nullptr) ray->getTransform().setPosition(marker->getTransform().getPosition());
    // Update health bar position and size
    float healthPercentage = static_cast<float>(player->getHealth()) / 100;
    float healthBarWidth = healthPercentage * 2.0f;
    //healthBar->getTransform().setScale(glm::vec3(healthBarWidth, 0.2f, 0.0f));
    float offsetX = (2.0f - healthBarWidth) / 2.0f;
    //healthBar->getTransform().setPosition(glm::vec3(player->getTransform().getPosition().x - offsetX, player->getTransform().getPosition().y + 0.7f, 0.0f));

    // Handle player's attack hitbox
    //if (player->isHitboxActive()) {
    //    for (DrawableObject* obj : objectsList) {
    //        // Attempt to cast the DrawableObject to EnemyObject
    //        EnemyObject* enemy = dynamic_cast<EnemyObject*>(obj);
    //        if (enemy != nullptr) { // Only proceed if the cast succeeds
    //            if (player->getAttackHitbox() != nullptr) {
    //                if (checkCollision(player->getAttackHitbox()->getColliderComponent(),
    //                    player->getAttackHitbox()->getTransform(),
    //                    enemy->getColliderComponent(),
    //                    enemy->getTransform())) {
    //                    enemy->takeDamage(player->getDamage());
    //                }
    //            }
    //        }
    //    }
    //}

    // Update other objects
    for (auto it = objectsList.begin(); it != objectsList.end(); ) {
        EnemyObject* enemy = dynamic_cast<EnemyObject*>(*it);
        if (enemy) {
            //glm::vec3 enemyPos = enemy->getTransform().getPosition();
            //glm::vec3 playerPos = player->getTransform().getPosition();

            //if (attackHitbox == nullptr) {
            //    attackHitbox = new SimpleObject();
            //    attackHitbox->addColliderComponent();
            //    objectsList.push_back(attackHitbox);
            //}
            //attackHitbox->getTransform().setPosition(glm::vec3(enemyPos.x + 1.0f, enemyPos.y, enemyPos.z));

            if (enemy->getHealth() <= 0) {
                /*delete enemy;
                it = objectsList.erase(it);  */
                DrawableObject::destroyObject(enemy);
                //continue;  
            }
        }
        ++it;
    }
}

void LevelPrototype::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
}

void LevelPrototype::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
}

void LevelPrototype::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelPrototype::handleKey(InputManager& input) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    /// Process key ///
    // add key that requires hold duration here
    processHeldKey(input, SDLK_k);
    processHeldMouse(input, SDL_BUTTON_LEFT);

    processHeldKey(input, SDLK_u);
    processHeldMouse(input, SDL_BUTTON_MIDDLE);

    // add key that requires buffering here
    processKeyBuffer(input, SDLK_LSHIFT);

    // handle event here
    if (input.getButton(SDLK_SPACE)) player->jump();
    if (input.getButton(SDLK_a)) player->move(glm::vec2(-1, 0));
    if (input.getButton(SDLK_d)) player->move(glm::vec2(1, 0));
    if (input.getButtonDown(SDLK_j)) player->parryAttack();
    if (input.getMouseButtonDown(SDL_BUTTON_RIGHT)) player->parryAttack();
    if (input.getButton(SDLK_UP)) marker->getTransform().translate(glm::vec3(0, 10, 0) * dt);;
    if (input.getButton(SDLK_DOWN)) marker->getTransform().translate(glm::vec3(0, -10, 0) * dt);
    if (input.getButton(SDLK_LEFT)) marker->getTransform().translate(glm::vec3(-10, 0, 0) * dt);
    if (input.getButton(SDLK_RIGHT)) marker->getTransform().translate(glm::vec3(10, 0, 0) * dt);
    if (input.getButtonDown(SDLK_f)) GameEngine::getInstance()->getRenderer()->toggleViewport();
    if (input.getButtonDown(SDLK_c)) player->getColliderComponent()->setTrigger(!player->getColliderComponent()->isTrigger());
    if (input.getButtonDown(SDLK_g)) viewMarker = !viewMarker;
    if (input.getButtonDown(SDLK_r)) GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART;
    //if (input.getButtonDown(SDLK_e)) GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_LEVEL1;
    if (input.getButtonDown(SDLK_e)) GameEngine::getInstance()->getStateController()->gameStateNext = (GameState)((GameEngine::getInstance()->getStateController()->gameStateCurr + 1) % 3);
    if (input.getButton(SDLK_z)) GameEngine::getInstance()->getRenderer()->increaseZoomRatio(0.1f);
    if (input.getButton(SDLK_x)) GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(0.1f);
    // test knockback
    if (input.getButton(SDLK_b)) player->knockback(glm::vec2(10,25), 0.5f);

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

    if (keyBuffer[SDLK_LSHIFT] > 0 && player->getCanMove()) {
        clearKeyBuffer(SDLK_LSHIFT);

        if (input.getButton(SDLK_a)){
            player->dodge(-1.0f);
        }
        else if (input.getButton(SDLK_d)) {
            player->dodge(1.0f);
        }
        else {
            player->dodge();
        }
    }


    //cout << dt << endl;
    // 
    //switch (key) {
    //case ' ': player->jump(); break;
    //case 'a': player->move(glm::vec2(-1, 0)); break;
    //case 'd': player->move(glm::vec2(1, 0)); break;
    //case '!': player->dodge(); break;
    //case '9': marker->getTransform().translate(glm::vec3(0, 10, 0) * dt); break;
    //case '0': marker->getTransform().translate(glm::vec3(0, -10, 0) * dt); break;
    //case '-': marker->getTransform().translate(glm::vec3(-10, 0, 0) * dt); break;
    //case '=': marker->getTransform().translate(glm::vec3(10, 0, 0) * dt); break;
    //case 'f': GameEngine::getInstance()->getRenderer()->toggleViewport(); break;
    //case 'c': player->getColliderComponent()->setTrigger(!player->getColliderComponent()->isTrigger()); break;
    ////case 'c': DrawableObject::destroyObject(marker); break;
    //case 'g': viewMarker = !viewMarker; break;
    //case 'r': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART; ; break;
    //case 'e': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_LEVEL1; ; break;
    //case 'z': GameEngine::getInstance()->getRenderer()->increaseZoomRatio(0.1f); break;
    //case 'x': GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(0.1f); break;
    //case 'k': player->attack(); break;
    //}

}

void LevelPrototype::handleMouse(int type, int x, int y) {
    /// Will be implemented in inherited level when used ///
}

void LevelPrototype::handleAnalogStick(int type, float amount) {

    if (type == 0) { // x axis
        player->getTransform().translate(glm::vec3(0.3 * amount, 0, 0));
    }
    else if (type == 1) { // y axis
        player->getTransform().translate(glm::vec3(0, -0.3 * amount, 0));
    }

}

void LevelPrototype::initPlayer(PlayerObject*& player, glm::vec3 position, PlayerInfo playerInfo) {
    if (player == nullptr) {
        player = new PlayerObject(playerInfo);
        objectsList.emplace_back(player);
    }

    player->getTransform().setPosition(position);
    player->setDrawCollider(true); // for debugging
}

void LevelPrototype::initPlayer(PlayerObject*& player, PlayerInfo playerInfo) {
    if (player == nullptr) {
        player = new PlayerObject(playerInfo);
        objectsList.emplace_back(player);
    }
    else {
        player->setName(playerInfo.name);
        player->setHealth(playerInfo.health);
        //player->setMovementInfo(playerInfo.movementInfo)
    }

    player->setDrawCollider(true); // for debugging
}

void LevelPrototype::instantiateEnemy(glm::vec3 position, EnemyInfo enemyInfo, EnemyType type) {
    /// add different type later ///

    EnemyObject* enemy = new EnemyObject(enemyInfo);
    enemy->getTransform().setPosition(position);
    if (player != nullptr) {
        enemy->setTarget(player);
    }
    objectsList.emplace_back(enemy);
}

static ostream& operator<<(ostream& out, glm::vec3 pos) {
    out << pos.x << "," << pos.y;
    return out;
}