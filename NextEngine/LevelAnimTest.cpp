#include "LevelAnimTest.h"

void LevelAnimTest::levelLoad() {
	SquareMeshVbo * square = new SquareMeshVbo();
	square->loadData();
	GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);
	
	//cout << "Load Level" << endl;
}
float timeK = 0;
float dt = 0;

void LevelAnimTest::levelInit() {
	AudioEngine* auds = new AudioEngine();
	auds->init("../Resource/Audio/SoundEffect", "../Resource/Audio/Music");
	audio = auds;
	audio->playMusicByIndex(0, -1);

	TexturedObject * obj = new TexturedObject();	
	obj->setTexture("../Resource/Texture/dog.png");
	obj->getTransform().setScale(glm::vec3(1.5f, -1.5f,0));
	obj->initAnimation(4, 4);
	obj->getAnimationComponent()->addState("down", 0, 4, true);
	obj->getAnimationComponent()->addState("left", 1, 4, true);
	obj->getAnimationComponent()->addState("right", 2, 4, true);
	obj->getAnimationComponent()->addState("up", 3, 4, true);
	objectsList.push_back(obj);

	player = obj;
	player->addColliderComponent();
	player->getAnimationComponent()->setState("down");
	//cout << "Init Level" << endl;
}

void LevelAnimTest::levelUpdate() {
	dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	timeK += dt;
	if (timeK > 0.12f) {
		
		player->getAnimationComponent()->updateCurrentState();
		d = 0;
		timeK = 0;
	}
}

void LevelAnimTest::levelDraw() {
	GameEngine::getInstance()->render(objectsList);
	player->drawCollider();
	//cout << "Draw Level" << endl;
}

void LevelAnimTest::levelFree() {
	audio->stopMusic();
	//audio->destroy();
	for (DrawableObject* obj : objectsList) {
		delete obj;
	}
	objectsList.clear();
	//cout << "Free Level" << endl;
}

void LevelAnimTest::levelUnload() {
	GameEngine::getInstance()->clearMesh();
	//cout << "Unload Level" << endl;
}

void LevelAnimTest::handleKey(char key) {
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	float speed = 10.0f;
	speed *= dt;
	switch (key) {
		case 'w': 
			player->getTransform().translate(glm::vec3(0, speed, 0));
			player->getAnimationComponent()->setState("up"); 
			audio->playSoundEffectByIndex(0, 0); 
			break;
		case 's': player->getTransform().translate(glm::vec3(0, -speed, 0));
			player->getAnimationComponent()->setState("down"); 
			audio->playSoundEffectByIndex(1, 0); 
			break;
		case 'a': player->getTransform().translate(glm::vec3(-speed, 0, 0));
			player->getAnimationComponent()->setState("left"); 
			audio->playSoundEffectByIndex(2, 0); 
			break;
		case 'd': player->getTransform().translate(glm::vec3(speed, 0, 0));
			player->getAnimationComponent()->setState("right"); 
			audio->playSoundEffectByIndex(3, 0); 
			break;
		case 'q': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_QUIT; ; break;
		case 'r': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART; ; break;
		case 'e': GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_LEVEL2; ; break;
	}
}

void LevelAnimTest::handleMouse(int type, int x, int y) {
	float realX, realY;

	// Calculate Real X Y 
	realX = x;
	realY = y;

	GameEngine::getInstance()->getWindowWidth();
	GameEngine::getInstance()->getWindowHeight();

	cout << "X : " << realX << " Y : " << realY << endl;

	player->getTransform().setPosition(glm::vec3(realX, realY, 0));
}

void LevelAnimTest::handleAnalogStick(int type, float amount) {

	if (type == 0) {//x axis
		player->getTransform().translate(glm::vec3(0.3 * amount, 0, 0));
	}
	else if (type == 1) {//y axis
		player->getTransform().translate(glm::vec3(0, -0.3 * amount, 0));
	}
}


