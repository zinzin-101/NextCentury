#include "LevelUITest.h"
#include "CollisionHandler.h"

int playerHealth = 0;

void LevelUITest::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);
}

void LevelUITest::levelInit() {
    UIobject = new UI();
    
    // Initialize the background
    background = new TexturedObject();
    background->setTexture("../Resource/Texture/TestBG.png");
    background->getTransform().setScale(glm::vec3(8.0f, 6.0f, 0)); 
    background->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f)); 
    objectsList.push_back(background);

    // Initialize the player
    PlayerInfo playerInfo;
    playerInfo.health = 100;
    playerInfo.damage = 5;
    player = new PlayerObject(playerInfo);
    player->getTransform().setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    player->addColliderComponent();
    player->addPhysicsComponent();
    player->setDrawCollider(true);
    objectsList.push_back(player);

    UIobject->initUI(UIobjectsList);

    EnemyInfo enemyInfo;
    enemyInfo.health = 10;
    enemyInfo.aggroRange = 80;
    enemyInfo.attackRange = 8;
    EnemyObject* enemy = new EnemyObject(enemyInfo);
    enemy->getTransform().setPosition(glm::vec3(3.0f, 2.0f, 0.0f));
    enemy->addColliderComponent();
    player->getColliderComponent()->setWidth(1);
    enemy->setDrawCollider(true);
    enemy->setTarget(player);
    objectsList.push_back(enemy);

    attackHitbox = new SimpleObject();
    attackHitbox->setColor(1.0f, 0.0f, 0.0f); 
    attackHitbox->getTransform().setScale(glm::vec3(1.0f, 1.0f, 1.0f)); 
    attackHitbox->addColliderComponent();
    attackHitbox->getColliderComponent()->setEnableCollision(false); 
    attackHitbox->setDrawCollider(true);
    objectsList.push_back(attackHitbox);

    player->setHealth(100);
    isHitboxActive = false;

    startObjects(objectsList);
}


void LevelUITest::levelUpdate() {
    //updateObjects(objectsList);
    mouseX = GameEngine::getInstance()->getInputHandler()->getMouseX();
    mouseY = GameEngine::getInstance()->getInputHandler()->getMouseY();

    glm::vec3 playerPosition = player->getTransform().getPosition();
    GameEngine::getInstance()->getRenderer()->updateCamera(playerPosition);

    // Check collision between player and enemy
    for (DrawableObject* obj : objectsList) {
        EnemyObject* enemy = dynamic_cast<EnemyObject*>(obj);
        if (enemy) {
            if (checkCollision(player->getColliderComponent(), player->getTransform(),
                enemy->getColliderComponent(), enemy->getTransform())) {
                player->takeDamage(1);  // Reduce player health by 1 on collision
                std::cout << "Player collided with enemy! Health: " << player->getHealth() << std::endl;
            }
        }
    }
    glm::vec3 camPos = GameEngine::getInstance()->getRenderer()->getCamPos();
    UIobject->updateUI(*player,camPos);
    
    

    // Update attack hitbox
    glm::vec3 playerPos = player->getTransform().getPosition();
    attackHitbox->getTransform().setPosition(glm::vec3(playerPos.x + 1.0f, playerPos.y, playerPos.z));

    if (isHitboxActive) {
        hitboxTimer += GameEngine::getInstance()->getTime()->getDeltaTime(); 
        if (hitboxTimer >= 0.2f) {
            isHitboxActive = false;
            attackHitbox->getColliderComponent()->setEnableCollision(false);
            std::cout << "Hitbox deactivated after 0.2 seconds." << std::endl;
        }
        else {
            for (auto it = objectsList.begin(); it != objectsList.end(); ) {
                EnemyObject* enemy = dynamic_cast<EnemyObject*>(*it);
                if (enemy && checkCollision(attackHitbox->getColliderComponent(), attackHitbox->getTransform(),
                    enemy->getColliderComponent(), enemy->getTransform())) {
                    enemy->takeDamage(player->getDamage());
                    std::cout << "Enemy hit by player attack! Enemy Health: " << enemy->getHealth() << std::endl;

                    if (enemy->getHealth() <= 0) {
                        std::cout << "Enemy defeated!" << std::endl;
                        delete enemy; 
                        it = objectsList.erase(it);
                    }
                    else {
                        ++it;
                    }
                    break; 
                }
                else {
                    ++it;
                }
            }
        }
    }
}

void LevelUITest::levelDraw() {
    std::list<DrawableObject*> renderList;

    for (DrawableObject* obj : objectsList) {
        if (obj == attackHitbox) {
            if (isHitboxActive) {
                renderList.push_back(attackHitbox);
            }
        }
        else {
            renderList.push_back(obj);
        }
    }
    for (DrawableObject* obj : UIobjectsList) {
        if (obj == attackHitbox) {
            if (isHitboxActive) {
                renderList.push_back(attackHitbox);
            }
        }
        else {
            renderList.push_back(obj);
        }
    }

    GameEngine::getInstance()->render(renderList);
}
void LevelUITest::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
    for (DrawableObject* obj : UIobjectsList) {
        delete obj;
    }
    UIobjectsList.clear();
}

void LevelUITest::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelUITest::handleKey(char key) {
    //std::cout << "Key pressed: " << key << std::endl;
    UIobject->handleInput(key);
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    switch (key) {
    case 'w': player->getTransform().translate(glm::vec3(0, 5, 0) * dt); break;
    case 's': player->getTransform().translate(glm::vec3(0, -5, 0) * dt); break;
    case 'a': player->getTransform().translate(glm::vec3(-5, 0, 0) * dt); break;
    case 'd': player->getTransform().translate(glm::vec3(5, 0, 0) * dt); break;
    case 'r': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART; break;
    case 'f': GameEngine::getInstance()->getRenderer()->toggleViewport(); break;
    case 'e': GameEngine::getInstance()->getStateController()->gameStateNext = (GameState)((GameEngine::getInstance()->getStateController()->gameStateCurr + 1) % 3); break;
    case 'z': GameEngine::getInstance()->getRenderer()->increaseZoomRatio(zoomRatio); break;
    case 'x': GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(zoomRatio); break;
    case 'h': player->takeDamage(1); break;
    case 'j': player->setHealth(100); break;  
    case 'k': {
        attackHitbox->getColliderComponent()->setEnableCollision(true);
        isHitboxActive = true;
        hitboxTimer = 0.0f; 
        std::cout << "Player attacks with hitbox activated!" << std::endl;
        break;
    }
    }

    glm::vec3 playerPosition = player->getTransform().getPosition();
    GameEngine::getInstance()->getRenderer()->updateCamera(playerPosition);
}

void LevelUITest::handleMouse(int type, int x, int y) {
    std::cout << "Mouse Click at: X = " << x << " Y = " << y << std::endl;
}

void LevelUITest::handleAnalogStick(int type, float amount) {

    if (type == 0) { // x axis
        player->getTransform().translate(glm::vec3(0.3 * amount, 0, 0));
    }
    else if (type == 1) { // y axis
        player->getTransform().translate(glm::vec3(0, -0.3 * amount, 0));
    }
}
