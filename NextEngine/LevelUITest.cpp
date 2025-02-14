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
    background->getTransform().setScale(glm::vec3(8.0f, 6.0f, 0)); // Adjust the scale to fit the screen
    background->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f)); // Ensure it's behind other objects
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
    
    // Initialize the player's health bar
    /*healthBar = new SimpleObject(); 
    healthBar->setColor(1.0f, 0.0f, 0.0f);
    healthBar->getTransform().setScale(glm::vec3(2.0f, 0.2f, 0.0f));
    healthBar->getTransform().setPosition(glm::vec3(player->getTransform().getPosition().x,
        player->getTransform().getPosition().y + 1.0f, 0.0f));
    objectsList.push_back(healthBar);*/
    UIobject->initUI(objectsList);


    // Initialize Enemy
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

    

    // Initialize other objects
    /*SimpleObject* obj2 = new SimpleObject();
    obj2->setColor(0.0, 0.0, 1.0);
    obj2->getTransform().setPosition(glm::vec3(2.0f, 2.0f, 0.0f));
    objectsList.push_back(obj2);

    SimpleObject* obj3 = new SimpleObject();
    obj3->setColor(0.0, 0.0, 1.0);
    obj3->getTransform().setPosition(glm::vec3(-2.0f, -2.0f, 0.0f));
    objectsList.push_back(obj3);*/

    

    attackHitbox = new SimpleObject();
    attackHitbox->setColor(1.0f, 0.0f, 0.0f); 
    attackHitbox->getTransform().setScale(glm::vec3(1.0f, 1.0f, 1.0f)); 
    attackHitbox->addColliderComponent();
    attackHitbox->getColliderComponent()->setEnableCollision(false); 
    attackHitbox->setDrawCollider(true);
    objectsList.push_back(attackHitbox);

    quitButton = new Button("Test","test");
    quitButton->setState(Button::ButtonState::NORMAL);


    objectsList.push_back(quitButton);




    player->setHealth(100);
    isHitboxActive = false;

    startObjects(objectsList);
}


void LevelUITest::levelUpdate() {
    //updateObjects(objectsList);

    glm::vec3 playerPosition = player->getTransform().getPosition();
    GameEngine::getInstance()->getRenderer()->updateCamera(playerPosition);

    // Update health bar based on player's current health
    float healthPercentage = static_cast<float>(player->getHealth()) / 100;
    float healthBarWidth = healthPercentage * 2.0f;
    //healthBar->getTransform().setScale(glm::vec3(healthBarWidth, 0.2f, 0.0f)); 
    float offsetX = (2.0f - healthBarWidth) / 2.0f;
    //healthBar->getTransform().setPosition(glm::vec3(playerPosition.x - offsetX, playerPosition.y + 1.5f, 0.0f)); 

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
    UIobject->updateUI(*player);
    // Update attack hitbox
    glm::vec3 playerPos = player->getTransform().getPosition();
    attackHitbox->getTransform().setPosition(glm::vec3(playerPos.x + 1.0f, playerPos.y, playerPos.z));

    if (isHitboxActive) {
        hitboxTimer += GameEngine::getInstance()->getTime()->getDeltaTime(); // Increment timer by delta time
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

    GameEngine::getInstance()->render(renderList);
}
void LevelUITest::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
}

void LevelUITest::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    //cout << "Unload Level" << endl;
}

void LevelUITest::handleKey(char key) {
    std::cout << "Key pressed: " << key << std::endl;
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    switch (key) {
    case 'w': player->getTransform().translate(glm::vec3(0, 5, 0) * dt); break;
    case 's': player->getTransform().translate(glm::vec3(0, -5, 0) * dt); break;
    case 'a': player->getTransform().translate(glm::vec3(-5, 0, 0) * dt); break;
    case 'd': player->getTransform().translate(glm::vec3(5, 0, 0) * dt); break;
    case 'r': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART; break;
    case 'f': GameEngine::getInstance()->getRenderer()->toggleViewport(); break;
    case 'e': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_COL_TEST; break;
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
    // Convert screen coordinates to world coordinates for 2D
    float realX = static_cast<float>(x) / GameEngine::getInstance()->getWindowWidth() * 2 - 1;
    float realY = 1 - static_cast<float>(y) / GameEngine::getInstance()->getWindowHeight() * 2;

    std::cout << "X : " << realX << " Y : " << realY << std::endl;

    // Set player position based on mouse click
    //player->getTransform().setPosition(glm::vec3(realX, realY, 0));

    if (checkCollisionPoint(player->getColliderComponent(), player->getTransform(), glm::vec2(realX, realY))) {
        cout << "detected" << endl;
    }

    if (checkCollisionPoint(quitButton->getColliderComponent(), quitButton->getTransform(), glm::vec2(realX, realY))) {
        quitButton->setState(Button::ButtonState::PRESSED);
    }

}

void LevelUITest::handleAnalogStick(int type, float amount) {

    if (type == 0) { // x axis
        player->getTransform().translate(glm::vec3(0.3 * amount, 0, 0));
    }
    else if (type == 1) { // y axis
        player->getTransform().translate(glm::vec3(0, -0.3 * amount, 0));
    }
}
