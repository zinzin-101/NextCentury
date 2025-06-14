#include "ParallaxObject.h"
#include "GameEngine.h"
#include <iostream>

using namespace std;

PlayerObject* ParallaxObject::player = nullptr;

void ParallaxObject::setPlayer(PlayerObject* player) {
	this->player = player;
}

ParallaxObject::ParallaxObject(float z, bool vertical, PlayerObject* player, bool loopable) : TexturedObject() {
	
	this->z = z;
	this->vertical = vertical;
	startPos = glm::vec3(0, 0, z);
	this->setName("parallax");
	this->player = player;
	this->loopable = loopable;
	if (loopable) {
		loopObjectR = new ParallaxObject(this->z, this->vertical, this->player, false); // must be false or else.... infinite loop naa
		loopObjectR->getTransform().setPosition(glm::vec3(this->getTransform().getPosition().x + 5, this->getTransform().getPosition().y, 0));
		cout << "loop created1" << endl;
	}
	else {
		loopObjectR = nullptr;
	}
}
// I use this one
ParallaxObject::ParallaxObject(float x,float y,float z, bool vertical, PlayerObject* player, bool loopable, float pictureWidth, float pictureHeight) : TexturedObject() {
	cout << "parallax created" << endl;
	this->getTransform().setScale(glm::vec3((pictureWidth / pictureHeight) * 9.0f, 9.0f, 0.0f)); // MUST CHANGE TO "SET SCALE TO RESOLUTION" not this magic number
	this->z = z;
	this->vertical = vertical;
	startPos = glm::vec3(x, y, z);
	this->setName("parallax");
	this->player = player;
	this->loopable = loopable;
	width = (pictureWidth / pictureHeight) * 9.0f;
	
	if (loopable) {
		loopObjectR = new ParallaxObject(x + ((pictureWidth / pictureHeight) * 9.0f)/* THIS OFF SET SHOULD BE Picture size FIX CAM */, y, z, this->vertical, this->player, false, pictureWidth, pictureHeight); // must be false or else.... infinite loop naa
		loopObjectL = new ParallaxObject(x - ((pictureWidth / pictureHeight) * 9.0f)/* THIS OFF SET SHOULD BE Picture size FIX CAM */, y, z, this->vertical, this->player, false, pictureWidth, pictureHeight);
		//loopObject->getTransform().setPosition(glm::vec3(this->getTransform().getPosition().x + 10.3f, this->getTransform().getPosition().y, 0));
		//loopObject->getTransform().setScale(glm::vec3(10.0f, 10.0f, 0.0f));
		cout << "loop created" << endl;
		//this->offsetLoop = 9.0f;
	}
	else {
		loopObjectR = nullptr;
		loopObjectL = nullptr;
	}
}

void ParallaxObject::setStartPos(glm::vec3 startPos) {
	this->startPos = startPos;
}

void ParallaxObject::setPlane(float nearPlane, float farPlane) { // the magic numbber
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}

void ParallaxObject::start(list<DrawableObject*>& objectsList) {
	// Might use to set player pointer but it will be O(n^2), so right now the player is set through level init
	// mf really cared about BigO 
}

// Parallax doesn't work with start Pos
void ParallaxObject::update(list<DrawableObject*>& objectsList) {
	if (player == nullptr) {
		cout << "Null player" << endl;
		return;
	}
	glm::vec3 camPos = GameEngine::getInstance()->getRenderer()->getCamPos();
	glm::vec3 playerPos = player->getTransform().getPosition();

	glm::vec3 travelDistance = camPos - startPos;
	
	float distanceFromPlayer = this->getTransform().getPosition().z - playerPos.z;
	float plane;
	if (distanceFromPlayer > 0) {
		plane = farPlane;
	}
	else {
		plane = nearPlane;
	}
	float clippingPlane = camPos.z + plane;
	
	float parallaxFactor = abs(distanceFromPlayer) / clippingPlane;

	//glm::vec3 newPos = startPos + (travelDistance * parallaxFactor);

	glm::vec3 newPos = startPos  + (camPos * parallaxFactor);
	//if (loopable) {
	//	cout << "CAM" << << endl;
	//}
	
	//cout << newPos.x << " " << newPos.y << " " << newPos.z << endl;

	if (abs(camPos.x - this->getTransform().getPosition().x) > width * 1.5f) {
		if (camPos.x < this->getTransform().getPosition().x) {
			offsetLoop -= width * 3;
		}
		else {
			offsetLoop += width * 3;
		}
	}

	if (!vertical) {
		this->getTransform().setPosition(glm::vec3(newPos.x + offsetLoop, startPos.y, startPos.z));
		//cout << this->getTransform().getPosition().x;
	}
	else {
		this->getTransform().setPosition(glm::vec3(newPos.x + offsetLoop, newPos.y, startPos.z));
		//cout << this->getTransform().getPosition().x;
	}
	
	//cout << "Ppos" << this->getTransform().getPosition().x << endl;

	// check loop
	if (loopable) {
		loopObjectR->update(objectsList);
		loopObjectL->update(objectsList);
	}
	//cout << "parX : " << this->getTransform().getPosition().x << endl;
	return;
}

void ParallaxObject::setTexture(string path) {
	TexturedObject::setTexture(path);
	if (loopable) {
		loopObjectR->setTexture(path);
		loopObjectL->setTexture(path);
	}
}

void ParallaxObject::initAnimation(int row, int column) {
	TexturedObject::initAnimation(row, column);
}
void ParallaxObject::render(glm::mat4 globalModelTransform) {
	TexturedObject::render(globalModelTransform);
	if (loopable) {
		loopObjectR->render(globalModelTransform);
		loopObjectL->render(globalModelTransform);
	}
}
Animation* ParallaxObject::getAnimationComponent() {
	return TexturedObject::getAnimationComponent();
}
bool ParallaxObject::isAnimated() {
	return TexturedObject::isAnimated();
}

void ParallaxObject::setRenderOpacity(float opacity) {
	this->DrawableObject::setRenderOpacity(opacity);
	loopObjectL->DrawableObject::setRenderOpacity(opacity);
	loopObjectR->DrawableObject::setRenderOpacity(opacity);
}

ParallaxObject::~ParallaxObject() {
	if (loopObjectR != nullptr) {
		delete loopObjectR;
	}
	if (loopObjectL != nullptr) {
		delete loopObjectL;
	}
}