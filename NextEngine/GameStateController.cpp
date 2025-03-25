#include "GameStateController.h"
#include "Level.h"
#include "LevelParallax.h"
#include "LevelTest.h"
#include "LevelAnimTest.h"
#include "LevelCollisionTest.h"
#include "LevelImgui.h"
#include "LevelUITest.h"
#include "LevelMapEditorTest.h"
#include "LevelEnemTest.h"
#include "LevelPrototype.h"
#include "LevelPrototypeNMMN.h"

GameStateController::GameStateController() {
	// set the initial game state
	gameStateCurr = gameStatePrev = gameStateNext = GameState::GS_NONE;

	currentLevel = nullptr;
}//

void GameStateController::init(GameState gameStateInit) {
	// reset the current, previoud and next game
	gameStateCurr = gameStatePrev = gameStateNext = gameStateInit;
}

void GameStateController::loadLevel() {
	if ((gameStateCurr == GameState::GS_RESTART) || (gameStateCurr == GameState::GS_QUIT)) {
		return;
	}
	cout << gameStateCurr << endl;
	switch (gameStateCurr) {
		case GameState::GS_LEVEL1:
			currentLevel = new LevelUITest();
			break;
		case GameState::GS_LEVEL2:
			currentLevel = new LevelImgui();
			break;
		case GameState::GS_LEVEL3:
			//cout << "in" << endl;
			currentLevel = new LevelPrototype();
			break;
		default:
			cout << "gGameStateCurr : invalid state!!" << endl;
			exit(1);
	}

	currentLevel->levelLoad();
}

void GameStateController::initLevel() {
	currentLevel->levelInit();
}

void GameStateController::updateLevel() {
	currentLevel->levelUpdate();
}

void GameStateController::drawLevel() {
	currentLevel->levelDraw();
}

void GameStateController::freelevel() {
	currentLevel->levelFree();
}

void GameStateController::unloadLevel() {
	currentLevel->levelUnload();
}

void GameStateController::handleKeyLevel(InputManager& input) {
	currentLevel->handleKey(input);
}

void GameStateController::handleKeyLevel(char key) {
	currentLevel->handleKey(key);
}

void GameStateController::handleMouseLevel(int type, int x, int y) {
	currentLevel->handleMouse(type, x, y);
}

void GameStateController::handleAnalogLevel(int type, float amount) {
	currentLevel->handleAnalogStick(type, amount);
}