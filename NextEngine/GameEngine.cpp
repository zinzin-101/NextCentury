#include "GameEngine.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine() {
	renderer = nullptr;
}

GameEngine * GameEngine::getInstance() {
	if (instance == nullptr) {
		instance = new GameEngine();
	}
	return instance;
}

GLRenderer * GameEngine::getRenderer() {
	return this->renderer;
}

GameStateController* GameEngine::getStateController() {
	return this->stateController;
}

Time* GameEngine::getTime() {
	return this->time;
}
InputManager* GameEngine::getInputHandler() {
	return inputHandler;
}
void GameEngine::init(int width, int height) {
	winWidth = width;
	winHeight = height;

	renderer = new GLRenderer(width, height);
	renderer->initGL("../Resource/Shader/vertext.shd", "../Resource/Shader/fragment.shd");
	setDrawArea(-8, 8, -4.5, 4.5);/*(-1 * width/2, width/2, -1 * height/2, height/2);*/
	setBackgroundColor(1.0f, 1.0f, 200.0f / 255);

	stateController = new GameStateController();
	stateController->init(GameState::GS_LEVEL1);

	time = new Time();
	inputHandler = new InputManager();  
}

void GameEngine::render(list<DrawableObject*> renderObjects, bool clear) {
	this->getRenderer()->render(renderObjects, clear);
}

void GameEngine::setDrawArea(float left, float right, float bottom, float top) {
	renderer->setOrthoProjection(left, right, bottom, top);
}

//void GameEngine::setBackgroundColor(float r, float g, float b) {
//	renderer->setClearColor(1.0f, 1.0f, 200.0f / 255);
//}

void GameEngine::setBackgroundColor(float r, float g, float b) {
	renderer->setClearColor(r, g, b);
}

void GameEngine::addMesh(string name, MeshVbo* mesh) {
	renderer->addMesh(name, mesh);
}

void GameEngine::clearMesh() {
	renderer->clearMesh();
}

int GameEngine::getWindowWidth() {
	return winWidth;
}

int GameEngine::getWindowHeight() {
	return winHeight;
}

void GameEngine::setWindowWidth(int w) {
	winWidth = w;
}
void GameEngine::setWindowHeight(int h) {
	winHeight = h;
}

void GameEngine::setSDLWindow(SDL_Window* window){
	this->gameWindow = window;
}
void GameEngine::setGlContext(SDL_GLContext* glContext) {
	this->glContext = glContext;
}

SDL_Window* GameEngine::getSDLWindow() const {
	return gameWindow;
}
SDL_GLContext* GameEngine::getGLContextPtr() const{
	return glContext;
}

void GameEngine::forceLoad(float duration) {
	while (duration > 0.0f) {
		time->updateTick(SDL_GetTicks());
		duration -= time->getDeltaTime();
		;
	}
}