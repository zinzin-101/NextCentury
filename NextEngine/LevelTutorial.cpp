#include "LevelTutorial.h"
#include <iostream>

LevelTutorial::LevelTutorial()
    : fadeState(FadeState::FadingIn), fadeTimer(0.0f) {
}

LevelTutorial::~LevelTutorial() {
}

void LevelTutorial::levelLoad() {
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

void LevelTutorial::levelInit() {
    UIobject = new UI();
    UIobject->initUI(objectsList);
	cout << "LevelTutorial Init" << endl;

    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition({ 0.0f, 0.0f, 0.0f });

    tutorialPage = new TexturedObject("TutorialPage");
    tutorialPage->setTexture("../Resource/Texture/UI/MainMenu/tutorialPage.png");
    tutorialPage->getTransform().setScale({ 16.0f, 9.0f, 0.0f });
    tutorialPage->getTransform().setPosition({ 0.0f, 0.0f, 0.0f });
    tutorialPage->setRenderOrder(1000);  
    tutorialPage->setRenderOpacity(0.0f);
    objectsList.push_back(tutorialPage);

	bg = new SimpleObject("TutorialBackground");
	bg->setColor({ 0.0f, 0.0f, 0.0f });
	bg->getTransform().setScale({ 20.0f, 20.0f, 0.0f });
	bg->setRenderOrder(999);
	objectsList.push_back(bg);

    fadeState = FadeState::FadingIn;
    fadeTimer = 0.0f;
}

void LevelTutorial::levelUpdate() {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTimeRealTime(); 
    fadeTimer += dt;
    float t = fadeTimer / ((fadeState == FadeState::Visible) ? holdDuration : fadeDuration);

    switch (fadeState) {
    case FadeState::FadingIn:
        if (fadeTimer >= fadeDuration) {
            fadeState = FadeState::Visible;
            fadeTimer = 0.0f;
            t = 1.0f;
        }
        tutorialPage->setRenderOpacity(glm::clamp(t, 0.0f, 1.0f));
        break;

    case FadeState::Visible:
        tutorialPage->setRenderOpacity(1.0f);
        if (fadeTimer >= holdDuration) {
            fadeState = FadeState::FadingOut;
            fadeTimer = 0.0f;
        }
        break;

    case FadeState::FadingOut:
        if (fadeTimer >= fadeDuration) {
            fadeState = FadeState::Done;
            t = 1.0f;
        }
        tutorialPage->setRenderOpacity(glm::clamp(1.0f - (fadeTimer / fadeDuration), 0.0f, 1.0f));
        break;

    case FadeState::Done:
        tutorialPage->setRenderOpacity(0.0f);
		bg->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        loadNextLevel();
        break;
    }

    // Update objects and UI
    updateObjects(objectsList);
    if (UIobject) UIobject->updateUI();
}

void LevelTutorial::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
}

void LevelTutorial::levelFree() {
    for (auto obj : objectsList) delete obj;
    objectsList.clear();
    delete UIobject;
}

void LevelTutorial::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
}

void LevelTutorial::handleKey(InputManager& input) {
    if (input.getButtonDown(SDLK_w)) UIobject->handleInput(SDLK_w);
    if (input.getButtonDown(SDLK_s)) UIobject->handleInput(SDLK_s);
    if (input.getButtonDown(SDLK_a)) UIobject->handleInput(SDLK_a);
    if (input.getButtonDown(SDLK_d)) UIobject->handleInput(SDLK_d);
    if (input.getButtonDown(SDLK_RETURN)) UIobject->handleInput(SDLK_RETURN);
    if (input.getButtonDown(SDLK_ESCAPE)) UIobject->handleInput(SDLK_ESCAPE);
}
