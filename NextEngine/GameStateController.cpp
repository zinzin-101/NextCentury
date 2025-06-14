#include "GameStateController.h"
#include "Level.h"
#include "LevelAct1.h"
#include "LevelAct2.h"
#include "LevelAct3.h"
#include "LevelAct4.h"
#include "LevelAct5.h"
#include "LevelAct6.h"
#include "LevelAct7.h"
#include "LevelAct8.h"
#include "LevelAct9.h"
#include "LevelAct10.h"
#include "LevelAct11.h"
#include "LevelAct12.h"
#include "LevelAct13.h"
#include "LevelAct14.h"
#include "LevelMainMenu.h"
#include "Level_Secret.h"
#include "LevelCredit.h"

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
		case GameState::GS_MAINMENU:
			currentLevel = new LevelMainMenu();
			//currentLevel = new LevelAct4();
			break;
		case GameState::GS_ACT1:
			currentLevel = new LevelAct1();
			break;
		case GameState::GS_ACT2:
			currentLevel = new LevelAct2();
			break;
		case GameState::GS_ACT3:
			currentLevel = new LevelAct3();
			break;
		case GameState::GS_ACT4:
			currentLevel = new LevelAct4();
			break;
		case GameState::GS_ACT5:
			currentLevel = new LevelAct5();
			break;
		case GameState::GS_ACT6:
			currentLevel = new LevelAct6();
			break;
		case GameState::GS_ACT7:
			currentLevel = new LevelAct7();
			break;
		case GameState::GS_ACT8:
			currentLevel = new LevelAct8();
			break;
		case GameState::GS_ACT9:
			currentLevel = new LevelAct9();
			break;
		case GameState::GS_ACT10:
			currentLevel = new LevelAct10();
			break;
		case GameState::GS_ACT11:
			currentLevel = new LevelAct11();
			break;
		case GameState::GS_ACT12:
			currentLevel = new LevelAct12();
			break;
		case GameState::GS_ACT13:
			currentLevel = new LevelAct13();
			break;
		case GameState::GS_ACT14:
			currentLevel = new LevelAct14();
			break;
		case GameState::GS_CREDIT:
			currentLevel = new LevelCredit();
			break;
		case GameState::GS_SECRET:
			currentLevel = new Level_Secret();
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

Level* GameStateController::getCurrentLevel() const {
	return currentLevel;
}