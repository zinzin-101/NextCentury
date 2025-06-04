#include "LevelCredit.h"
#include "GameEngine.h"  
#include "SquareMeshVbo.h"
#include "SquareBorderMesh.h"
#include "DrawableObject.h"
#include "TexturedObject.h"
#include "SimpleObject.h"
#include "InputManager.h"

void LevelCredit::levelLoad() {
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

void LevelCredit::levelInit() {
    blackdrop = new SimpleObject("Blackdrop");
    blackdrop->setColor({ 0.0f, 0.0f, 0.0f });
    blackdrop->getTransform().setScale({ 20.0f, 20.0f, 0.0f });
    objectsList.push_back(blackdrop);

    creditText = new TexturedObject("CreditText");
    creditText->setTexture("../Resource/Texture/UI/Credit/Credits.png");

    creditText->getTransform().setScale(glm::vec3(1.0f*12.0f, (1348.0f / 640.0f) * 12.0f, 0.0f));
    creditText->getTransform().setPosition(glm::vec3(0.0f, -17.0f, 0.0f));

    GameEngine::getInstance()->getRenderer()->getCamera()->setPosition(glm::vec3(0.0f,0.0f,0.0f));

    objectsList.push_back(creditText);
}

void LevelCredit::levelUpdate() {
    if (creditText) {
        creditText->getTransform().translate(0.0f, 0.008f);
    }
}

void LevelCredit::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
}

void LevelCredit::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
}

void LevelCredit::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
}

void LevelCredit::handleKey(InputManager& input) {
	if (input.getButtonDown(SDLK_ESCAPE)) {
		GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_MAINMENU;
	}
}
