#include "Level.h"
#include "CollisionHandler.h"


void Level::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);
}

void Level::levelInit() {
    /// Will be implemented in inherited level when used ///
}


void Level::levelUpdate() {
    /// Will be implemented in inherited level when used ///
}

void Level::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
}

void Level::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
}

void Level::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    //cout << "Unload Level" << endl;
}

void Level::handleKey(char key) {
    /// Will be implemented in inherited level when used ///
}

void Level::handleKey(InputManager& input) {
    /// Will be implemented in inherited level when used ///
}

void Level::processHeldKey(InputManager& input, SDL_Keycode key) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    if (input.getButtonDown(key)) {
        keyHeldDuration[key] = 0.0f;
        return;
    }

    if (input.getButton(key)) {
        keyHeldDuration[key] += dt;
        return;
    }
}

void Level::processKeyBuffer(InputManager& input, SDL_Keycode key) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    
    if (input.getButtonDown(key)) {
        keyBuffer[key]++;
        return;
    }
}

void Level::processHeldMouse(InputManager& input, unsigned int mouse) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    if (input.getMouseButtonDown(mouse)) {
        mouseHeldDuration[mouse] = 0.0f;
        return;
    }

    if (input.getMouseButton(mouse)) {
        mouseHeldDuration[mouse] += dt;
        return;
    }
}

void Level::processMouseBuffer(InputManager& input, unsigned int mouse) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    if (input.getButtonDown(mouse)) {
        mouseBuffer[mouse]++;
        return;
    }
}

void Level::clearKeyBuffer(SDL_Keycode key) {
    keyBuffer[key] = 0;
}

void Level::handleMouse(int type, int x, int y) {
    /// Will be implemented in inherited level when used ///
}

void Level::handleAnalogStick(int type, float amount) {

    /// Usage ///
    /*
    if (type == 0) { // x axis
        player->getTransform().translate(glm::vec3(0.3 * amount, 0, 0));
    }
    else if (type == 1) { // y axis
        player->getTransform().translate(glm::vec3(0, -0.3 * amount, 0));
    }
    */

    /// Will be implemented in inherited level when used ///
}

void Level::startObjects(list<DrawableObject*>& objectsList) {
    for (DrawableObject* obj : objectsList) {
        obj->start(objectsList);
    }
}

void Level::updateObjects(list<DrawableObject*>& objectsList) {
    for (DrawableObject* obj : objectsList) {
        if (obj->getIsActive()) {
            obj->update(objectsList);
        }
    }

    handleObjectCollision(objectsList);

    for (std::list<DrawableObject*>::iterator itr = objectsList.begin(); itr != objectsList.end(); itr++) {
        DrawableObject* obj = *itr;
        if (obj->getMarkedForDelete()) {
            delete obj;
            itr = objectsList.erase(itr);
            itr--;
            continue;
        }
    }
}

void Level::initPlayer(PlayerObject*& player, PlayerInfo playerInfo) {
    if (player == nullptr) {
        player = new PlayerObject(playerInfo);
        objectsList.emplace_back(player);
    }
    else {
        player->setName(playerInfo.name);
        player->setHealth(playerInfo.health);
        //player->setMovementInfo(playerInfo.movementInfo);
        player->setDamage(playerInfo.damage);
    }

    player->setDrawCollider(true); // for debugging
    //player->getTransform().setScale(glm::vec3(500.0f, 500.0f , 1.0f));
}
