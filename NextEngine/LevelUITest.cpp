#include "LevelUITest.h"
#include "CollisionHandler.h"

static ostream& operator<<(ostream& out, glm::vec3 pos) {
    out << pos.x << "," << pos.y;
    return out;
}

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

    // Initialize the background for the Main Menu.
    background = new TexturedObject();
    background->setTexture("../Resource/Texture/MainMenuBG.png"); // Use a main menu background texture
    background->getTransform().setScale(glm::vec3(8.0f, 6.0f, 0.0f));
    background->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    objectsList.push_back(background);

    // Initialize the UI with the main menu buttons.
    UIobject->initUI(UIobjectsList);

    // If you have any objects that need starting, add them here.
    startObjects(objectsList);

    // Center the camera.
    glm::vec3 centerCamPos = glm::vec3(0.0f, 0.0f, 0.0f);
    GameEngine::getInstance()->getRenderer()->updateCamera(centerCamPos);
}

void LevelUITest::levelUpdate() {
    // Ensure the camera remains centered.
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);
    GameEngine::getInstance()->getRenderer()->updateCamera(camPos);
}

void LevelUITest::levelDraw() {
    std::list<DrawableObject*> renderList;
    GameEngine::getInstance()->setDrawArea(-8, 8, -4.5f, 4.5f);
    for (DrawableObject* obj : objectsList) {
        renderList.push_back(obj);
    }
    GameEngine::getInstance()->render(renderList);
    renderList.clear();
    GameEngine::getInstance()->setDrawArea(-960, 960, -540, 540);
    for (DrawableObject* obj : UIobjectsList) {
        renderList.push_back(obj);
    }
    GameEngine::getInstance()->render(renderList, false);
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

    if (UIobject != nullptr) {
        delete UIobject;
        UIobject = nullptr;
    }
}

void LevelUITest::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
}

void LevelUITest::handleKey(InputManager& input) {
    if (input.getButtonDown(SDLK_a)) UIobject->handleInput(SDLK_a);
    if (input.getButtonDown(SDLK_d)) UIobject->handleInput(SDLK_d);
    if (input.getButtonDown(SDLK_RETURN)) UIobject->handleInput(SDLK_RETURN);
    if (input.getButtonDown(SDLK_SPACE)) UIobject->handleInput(SDLK_SPACE);
}
