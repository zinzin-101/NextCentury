#include "CollisionHandler.h"
#include "LevelParallax.h"
#include "MapLoader.h"

static void loadMap(MapLoader& loader, int i, list<DrawableObject*>& objList, PlayerObject*& player);
static void appendMap(MapLoader& loader, int i, list<DrawableObject*>& objList, PlayerObject*& player);
static ostream& operator<<(ostream& out, glm::vec3 pos);

void LevelParallax::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);

}

void LevelParallax::levelInit() {


    PlayerInfo pinf = PlayerInfo();
    player = new PlayerObject(pinf);


    if (player != nullptr) {
        cout << "player not null" << endl;
    }

    /*TextObject* txt = new TextObject();
    SDL_Color black = { 0, 0, 0 };
    txt->loadText("Them text", black, 72);
    objectsList.push_back(txt);*/

}
void LevelParallax::levelUpdate() {
    updateObjects(objectsList);
    //player->getTransform().setScale(186, 186);
    for (DrawableObject* obj : objectsList) {
        obj->update(objectsList);
    }
    handleObjectCollision(objectsList);
    //cout << enemy->getIsStun();


    glm::vec3 playerPosition = player->getTransform().getPosition();
    GameEngine::getInstance()->getRenderer()->updateCamera(playerPosition);
}

void LevelParallax::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
}

void LevelParallax::levelFree() {
    GameEngine::getInstance()->setDrawArea(-8.0f, 8.0f, -4.5f, 4.5f);
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
}

void LevelParallax::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    //cout << "Unload Level" << endl;
}

void LevelParallax::handleKey(char key) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    //cout << dt << endl;
    switch (key) {
    case ' ': if (player->getColliderComponent()->getCollisionFlag() && COLLISION_DOWN) player->getPhysicsComponent()->setVelocity(glm::vec2(0, 25)); break;
    case 'w': player->getTransform().translate(glm::vec3(0, 5, 0) * dt); break;
    case 's': player->getTransform().translate(glm::vec3(0, -5, 0) * dt); break;
    case 'a': player->getTransform().translate(glm::vec3(-5, 0, 0) * dt); break;
    case 'd': player->getTransform().translate(glm::vec3(5, 0, 0) * dt); break;
        //case 'f': enemy->addStatus(LivingEntity::Status(LivingEntity::MOODENG, 1.f)); break;
    case 'f': GameEngine::getInstance()->getRenderer()->toggleViewport(); break;
    case 'c': player->getColliderComponent()->setTrigger(!player->getColliderComponent()->isTrigger()); break;
    case 'g': player->getPhysicsComponent()->setEnableGravity(!player->getPhysicsComponent()->isGravityEnable()); break;
    case 'q': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_QUIT; ; break;
    case 'r': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART; ; break;
    case 'e': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_LEVEL2; ; break;
    case 'z': GameEngine::getInstance()->getRenderer()->increaseZoomRatio(10.0f); break;
    case 'x': GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(10.0f); break;
    }
}

void LevelParallax::handleMouse(int type, int x, int y) {
    /// Will be implemented in inherited level when used ///
}

void LevelParallax::handleAnalogStick(int type, float amount) {

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