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
    float dt = GameEngine::getInstance()->getTime()->getDeltaTimeRealTime();

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