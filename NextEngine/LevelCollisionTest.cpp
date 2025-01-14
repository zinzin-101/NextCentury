#include "Collider.h"
#include "CollisionHandler.h"
#include "LevelCollisionTest.h"

void LevelCollisionTest::levelLoad() {
	SquareMeshVbo* square = new SquareMeshVbo();
	square->loadData();
	GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

	SquareBorderMesh* border = new SquareBorderMesh();
	border->loadData();
	GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);

	//cout << "Load Level" << endl;
}

void LevelCollisionTest::levelInit() {
	// Initialize the background
	background = new TexturedObject("BG");
	background->setTexture("../Resource/Texture/BGParallax/Mountain1920.png");
	background->getTransform().setScale(glm::vec3(10.0f, 10.0f, 0)); // Adjust the scale to fit the screen
	background->getTransform().setPosition(glm::vec3(0.0f, 0.0f, -1.0f)); // Ensure it's behind other objects
	objectsList.push_back(background);

	// Initialize the player
	PlayerInfo playerInfo;
	player = new PlayerObject(playerInfo);
	player->setTexture("../Resource/Texture/playerTexture.png");
	player->getTransform().setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	player->addColliderComponent();
	player->addPhysicsComponent();
	player->setDrawCollider(true);
	objectsList.push_back(player);

	// Initialize Enemy
	EnemyInfo enemyInfo;
	enemyInfo.health = 10;
	enemyInfo.aggroRange = 80;
	enemyInfo.attackRange = 8;
	EnemyObject* enemy = new EnemyObject(enemyInfo);
	enemy->setTexture("../Resource/Texture/enemyTexture.png");
	enemy->getTransform().setPosition(glm::vec3(3.0f, 2.0f, 0.0f));
	enemy->getTransform().rotate(180);
	enemy->addColliderComponent();
	enemy->addPhysicsComponent();
	enemy->setDrawCollider(true);
	objectsList.push_back(enemy);

	// Initialize other objects
	SimpleObject* obj2 = new SimpleObject("obj1");
	obj2->setColor(0.0, 0.0, 1.0);
	obj2->getTransform().setPosition(glm::vec3(2.0f, 2.0f, 0.0f));
	obj2->addColliderComponent();
	obj2->getColliderComponent()->setTrigger(true);
	objectsList.push_back(obj2);

	SimpleObject* obj3 = new SimpleObject("obj2");
	obj3->setColor(0.0, 0.0, 1.0);
	obj3->getTransform().setPosition(glm::vec3(-2.0f, -2.0f, 0.0f));
	objectsList.push_back(obj3);

	player->getTransform().setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	player->addColliderComponent();
	//player->getColliderComponent()->setWidth(10);
	player->addPhysicsComponent();
	player->setDrawCollider(true);

	obj2->addColliderComponent();
	obj2->getColliderComponent()->setWidth(1);
	obj2->getColliderComponent()->setHeight(3);
	obj2->setDrawCollider(true);

	obj3->addColliderComponent();
	obj3->getColliderComponent()->setWidth(15);
	obj3->getColliderComponent()->setHeight(2);
	obj3->setDrawCollider(true);

	ground = obj3;
	ground->getTransform().setScale(1.5f);
	ground->getTransform().translate(glm::vec3(-2,0,0));
	ground->getColliderComponent()->getTransform().translate(glm::vec3(0, -1, 0));
	GameEngine::getInstance()->getRenderer()->setClearColor(0.1, 0.1, 0.1);
	player->getColliderComponent()->setTrigger(true);
	player->getPhysicsComponent()->setEnableGravity(false);
}

void LevelCollisionTest::levelUpdate() {
	/*player->update();*/
	for (DrawableObject* obj : objectsList) {
		obj->update(objectsList);
	}
	handleObjectCollision(objectsList);
	//handleObjectCollision(objectsList, triggerTest);
	/*float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	cout << dt << endl;*/

	glm::vec3 playerPosition = player->getTransform().getPosition();
	GameEngine::getInstance()->getRenderer()->updateCamera(playerPosition);

	//printCollisionState(player, ground);
}

void LevelCollisionTest::levelDraw() {
	GameEngine::getInstance()->render(objectsList);
}

void LevelCollisionTest::levelFree() {
	for (DrawableObject* obj : objectsList) {
		delete obj;
	}
	objectsList.clear();
	//cout << "Free Level" << endl;
}

void LevelCollisionTest::levelUnload() {
	GameEngine::getInstance()->clearMesh();
	//cout << "Unload Level" << endl;
}

void LevelCollisionTest::handleKey(char key) {
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	//cout << dt << endl;
	switch (key) {
	case ' ': player->getPhysicsComponent()->setVelocity(glm::vec2(0, 25)); break;
	case 'w': player->getTransform().translate(glm::vec3(0, 5, 0) * dt); break;
	case 's': player->getTransform().translate(glm::vec3(0, -5, 0) * dt); break;
	case 'a': player->getTransform().translate(glm::vec3(-5, 0, 0) * dt); break;
	case 'd': player->getTransform().translate(glm::vec3(5, 0, 0) * dt); break;
	case 'f': GameEngine::getInstance()->getRenderer()->toggleViewport(); break;
	case 'c': player->getColliderComponent()->setTrigger(!player->getColliderComponent()->isTrigger()); break;
	case 'g': player->getPhysicsComponent()->setEnableGravity(!player->getPhysicsComponent()->isGravityEnable()); break;
	case 'q': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_QUIT; ; break;
	case 'r': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART; ; break;
	case 'e': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_COL_TEST; ; break;
	case 'z': GameEngine::getInstance()->getRenderer()->increaseZoomRatio(0.1f); break;
	case 'x': GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(0.1f); break;
	}
}

void triggerTest(DrawableObject* obj1, DrawableObject* obj2) {
	printCollisionState(obj1, obj2);
}

void printCollisionState(DrawableObject* obj1, DrawableObject* obj2) {
	Collider::CollisionState state = obj1->getColliderComponent()->getCollisionState(obj2->getColliderComponent());

	switch (state) {
	case Collider::ENTER:
		cout << obj1->getName() << " Enter " << obj2->getName() << endl;
		break;
	case Collider::STAY:
		cout << obj1->getName() << " Stay " << obj2->getName() << endl;
		break;
	case Collider::EXIT:
		cout << obj1->getName() << " Exit " << obj2->getName() << endl;
		break;
	case Collider::NONE:
		cout << obj1->getName() << " None " << obj2->getName() << endl;
		break;
	}
}