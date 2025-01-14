//#include "LevelTest.h"
//
//void LevelTest::levelLoad() {
//    SquareMeshVbo* square = new SquareMeshVbo();
//    square->loadData();
//    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);
//}
//
//void LevelTest::levelInit() {
//    // Initialize the background
//    background = new TexturedObject();
//    background->setTexture("../Resource/Texture/TestBG.png");
//    background->getTransform().setScale(glm::vec3(8.0f, -6.0f, 0)); // Adjust the scale to fit the screen
//    background->getTransform().setPosition(glm::vec3(0.0f, 0.0f, -1.0f)); // Ensure it's behind other objects
//    objectsList.push_back(background);
//
//    // Initialize the player
//    SimpleObject* obj = new SimpleObject();
//    obj->setColor(0.0, 1.0, 0.0);
//    objectsList.push_back(obj);
//    player = obj;
//
//    // Initialize other objects
//    SimpleObject* obj2 = new SimpleObject();
//    obj2->setColor(0.0, 0.0, 1.0);
//    obj2->getTransform().setPosition(glm::vec3(2.0f, 2.0f, 0.0f));
//    objectsList.push_back(obj2);
//
//    SimpleObject* obj3 = new SimpleObject();
//    obj3->setColor(0.0, 0.0, 1.0);
//    obj3->getTransform().setPosition(glm::vec3(-2.0f, -2.0f, 0.0f));
//    objectsList.push_back(obj3);
//}
//
//void LevelTest::levelUpdate() {
//    glm::vec3 playerPosition = player->getTransform().getPosition(); // Get player position
//    GameEngine::getInstance()->getRenderer()->updateCamera(playerPosition); // Update camera with player position
//
//
//    // Here you should also handle the rendering
//    GameEngine::getInstance()->render(objectsList); // Render the objects
//
//    std::cout << "Player Position(LevelUpdate): " << playerPosition.x << ", " << playerPosition.y << std::endl;
//
//}
//
//void LevelTest::levelDraw() {
//    GameEngine::getInstance()->render(objectsList);
//}
//
//void LevelTest::levelFree() {
//    for (DrawableObject* obj : objectsList) {
//        delete obj;
//    }
//    objectsList.clear();
//}
//
//void LevelTest::levelUnload() {
//    GameEngine::getInstance()->clearMesh();
//}
//
//void LevelTest::handleKey(char key) {
//    switch (key) {
//    case 'w': player->getTransform().translate(glm::vec3(0, 0.3, 0));cout << player->getTransform().getPosition().x << endl; break;
//    case 's': player->getTransform().translate(glm::vec3(0, -0.3, 0)); break;
//    case 'a': player->getTransform().translate(glm::vec3(-0.3, 0, 0)); break;
//    case 'd': player->getTransform().translate(glm::vec3(0.3, 0, 0)); break;
//    case 'q': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_QUIT; break;
//    case 'r': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART; break;
//    case 'e': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_LEVEL1; break;
//    }
//
//    // Update camera after handling input
//    glm::vec3 playerPosition = player->getTransform().getPosition(); // Get updated player position
//    GameEngine::getInstance()->getRenderer()->updateCamera(playerPosition); // Update camera with player position
//}
//
//void LevelTest::handleMouse(int type, int x, int y) {
//    float realX, realY;
//
//    // Calculate Real X Y 
//    realX = x;
//    realY = y;
//
//    player->getTransform().setPosition(glm::vec3(realX, realY, 0));
//}
