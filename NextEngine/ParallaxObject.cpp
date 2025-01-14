#include "ParallaxObject.h"
#include "GameEngine.h"
#include <iostream>

using namespace std;

PlayerObject* ParallaxObject::player = nullptr;

void ParallaxObject::setPlayer(PlayerObject* player) {
	this->player = player;
}

ParallaxObject::ParallaxObject(float z, bool vertical, PlayerObject* player) : TexturedObject() {
	this->z = z;
	this->vertical = vertical;
	startPos = glm::vec3(0, 0, z);
	this->setName("parallax");
	this->player = player;
}

ParallaxObject::ParallaxObject(float x,float y,float z, bool vertical, PlayerObject* player) : TexturedObject() {
	this->z = z;
	this->vertical = vertical;
	startPos = glm::vec3(x, y, z);
	this->setName("parallax");
	this->player = player;
}

void ParallaxObject::setStartPos(glm::vec3 startPos) {
	this->startPos = startPos;
}

void ParallaxObject::setPlane(float nearPlane, float farPlane) { // the magic numbber
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}
// Parallax doesn't work with start Pos
void ParallaxObject::update(list<DrawableObject*>& objectsList) {
	if (player == nullptr) {
		cout << "Null player" << endl;
		return;
	}
	glm::vec3 camPos = GameEngine::getInstance()->getRenderer()->camPos();
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

	glm::vec3 newPos = startPos + (travelDistance * parallaxFactor);
	//cout << newPos.x << " " << newPos.y << " " << newPos.z << endl;
	if (!vertical) {
		this->getTransform().setPosition(glm::vec3(newPos.x, startPos.y, startPos.z));
		//cout << this->getTransform().getPosition().x;
	}
	else {
		this->getTransform().setPosition(glm::vec3(newPos.x, newPos.y, startPos.z));
		//cout << this->getTransform().getPosition().x;
	}
	return;
}

void ParallaxObject::setTexture(string path) {
	TexturedObject::setTexture(path);
}
void ParallaxObject::initAnimation(int row, int column) {
	TexturedObject::initAnimation(row, column);
}
void ParallaxObject::render(glm::mat4 globalModelTransform) {
	TexturedObject::render(globalModelTransform);
}
Animation* ParallaxObject::getAnimationComponent() {
	return TexturedObject::getAnimationComponent();
}
bool ParallaxObject::isAnimated() {
	return TexturedObject::isAnimated();
}