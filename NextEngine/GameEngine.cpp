#include "GameEngine.h"
#include "Level.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine() {
	renderer = nullptr;

	engineTimer = 0.0f;
	isGamePaused = false;
	prevTimeScale = 1.0f;
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
	audioEngine.init("../Resource/Audio/SoundEffect/", "../Resource/Audio/Music/");
}

void GameEngine::updateEngineComponent() {
	if (engineTimer > 0.0f) {
		engineTimer -= time->getDeltaTimeRealTime();
	if (engineTimer <= 0.0f) {
		isGamePaused = false;
		time->setTimeScale(prevTimeScale);
		}
	}
}

void GameEngine::render(list<DrawableObject*> renderObjects, bool clear) {
	this->getRenderer()->render(renderObjects, clear);
}

void GameEngine::setDrawArea(float left, float right, float bottom, float top) {
	renderer->setOrthoProjection(left, right, bottom, top);
}

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

void GameEngine::freezeGameForSeconds(float duration) {
	while (duration > 0.0f) {
		time->updateTick(SDL_GetTicks());
		duration -= time->getDeltaTimeRealTime();
	}
}

void GameEngine::pauseTimeForSeconds(float duration) {
	engineTimer = duration;
	prevTimeScale = 1.0f;
	isGamePaused = true;
	time->setTimeScale(0.0f);
}

void GameEngine::pauseTime() {
	if (!isGamePaused) {
		prevTimeScale = time->getTimeScale();
		isGamePaused = true;
		time->setTimeScale(0.0f);
	}
}


void GameEngine::resumeTime() {
	if (isGamePaused) {
		isGamePaused = false;
		time->setTimeScale(prevTimeScale);
	}
}

bool GameEngine::getIsGamePaused() const {
	return isGamePaused;
}

void GameEngine::initAudio(const std::string& effectFolder, const std::string& musicFolder) {
	audioEngine.init(effectFolder, musicFolder);
	std::cout << "AudioEngine initialized with Effects:" << effectFolder
		<< " Music:" << musicFolder << std::endl;
}

void GameEngine::playSoundEffect(const std::string& fileName, int loop) {
	audioEngine.playSoundEffectByName(fileName, loop);
}

void GameEngine::playMusic(const std::string& fileName, int loop) {
	audioEngine.playMusicByName(fileName, loop);
}

void GameEngine::stopMusic() {
	audioEngine.stopMusic();
}

void GameEngine::signalToCurrentLevel() {
	stateController->getCurrentLevel()->signalFromEngine();
}