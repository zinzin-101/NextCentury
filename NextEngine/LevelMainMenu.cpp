#include "LevelMainMenu.h"

void LevelMainMenu::levelLoad() {

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

void LevelMainMenu::levelInit() {
    GameEngine::getInstance()->playMusic("BGM_MainMenu.wav", 1);
    UIobject = new UI();
    UIobject->initUI(objectsList);
    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

void LevelMainMenu::levelUpdate() {
    updateObjects(objectsList);
    UIobject->updateUI();
}

void LevelMainMenu::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
}

void LevelMainMenu::levelFree() {
    GameEngine::getInstance()->stopMusic();
    GameEngine::getInstance()->stopSfx();
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();

    delete UIobject;
}

void LevelMainMenu::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
}

void LevelMainMenu::handleKey(InputManager& input) {
    if (input.getButtonDown(SDLK_w)) UIobject->handleInput(SDLK_w);
    if (input.getButtonDown(SDLK_s)) UIobject->handleInput(SDLK_s);
    if (input.getButtonDown(SDLK_a)) UIobject->handleInput(SDLK_a);
    if (input.getButtonDown(SDLK_d)) UIobject->handleInput(SDLK_d);
    if (input.getButtonDown(SDLK_RETURN)) UIobject->handleInput(SDLK_RETURN);
    if (input.getButtonDown(SDLK_SPACE)) UIobject->handleInput(SDLK_SPACE);
    if (input.getButtonDown(SDLK_q)) UIobject->handleInput(SDLK_q);
    if (input.getButtonDown(SDLK_TAB))    UIobject->handleInput(SDLK_TAB);
    if (input.getButtonDown(SDLK_ESCAPE)) UIobject->handleInput(SDLK_ESCAPE);


}