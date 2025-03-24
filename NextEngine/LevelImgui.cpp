#include "LevelImgui.h"

void LevelImgui::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);
}

void LevelImgui::levelInit() {
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    marker = new SimpleObject();

    GameEngine::getInstance()->getRenderer()->getCamera()->setTarget(marker);
    GameEngine::getInstance()->getRenderer()->setToggleViewport(false);
}

void LevelImgui::levelUpdate() {
    updateObjects(objectsList);
    GameEngine::getInstance()->getRenderer()->updateCamera(glm::vec3());
}

void LevelImgui::levelDraw() {
    GameEngine::getInstance()->render(objectsList);

    #ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
    #endif
}

void LevelImgui::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
}

void LevelImgui::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
    //cout << "Unload Level" << endl;
}

void LevelImgui::handleKey(InputManager& input) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    if (input.getButton(SDLK_z)) GameEngine::getInstance()->getRenderer()->increaseZoomRatio(0.1f);
    if (input.getButton(SDLK_x)) GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(0.1f);


    if (input.getButton(SDLK_LSHIFT)) {
        if (input.getButton(SDLK_w)) marker->getTransform().translate(glm::vec3(0, 3, 0) * dt);
        if (input.getButton(SDLK_s)) marker->getTransform().translate(glm::vec3(0, -3, 0) * dt);
        if (input.getButton(SDLK_a)) marker->getTransform().translate(glm::vec3(-3, 0, 0) * dt);
        if (input.getButton(SDLK_d)) marker->getTransform().translate(glm::vec3(3, 0, 0) * dt);
    }
    else {
        if (input.getButton(SDLK_w)) marker->getTransform().translate(glm::vec3(0, 10, 0) * dt);
        if (input.getButton(SDLK_s)) marker->getTransform().translate(glm::vec3(0, -10, 0) * dt);
        if (input.getButton(SDLK_a)) marker->getTransform().translate(glm::vec3(-10, 0, 0) * dt);
        if (input.getButton(SDLK_d)) marker->getTransform().translate(glm::vec3(10, 0, 0) * dt);
    }
}

void LevelImgui::initPlayer(PlayerObject*& player, glm::vec3 position, PlayerInfo playerInfo) {
    if (player == nullptr) {
        player = new PlayerObject(playerInfo);
        objectsList.emplace_back(player);
    }

    player->getTransform().setPosition(position);
    player->setDrawCollider(true); // for debugging
}

void LevelImgui::initPlayer(PlayerObject*& player, PlayerInfo playerInfo) {
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