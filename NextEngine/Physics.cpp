#include "CollisionHandler.h"
#include "GameEngine.h"
#include "Physics.h"
#include <iostream>
#include <math.h>

using namespace std;

Physics::Physics(Transform& transform): enableGravity(true), enablePhysics(true) {
	lastPos = transform.getPosition();
	velocity = glm::vec2();
	acceleration = glm::vec2();
	gravity = glm::vec2(0, DEFAULT_GRAVITY);
	mass = 1.0f;
	force = glm::vec2();
	drag = 0.1f;
}

void Physics::setVelocity(glm::vec2 vel) {
	velocity = vel;
}

void Physics::setAcceleration(glm::vec2 accel) {
	acceleration = accel;
}

void Physics::setGravity(glm::vec2 g) {
	gravity = g;
}

void Physics::setMass(float m) {
	mass = m;
}

void Physics::addForce(glm::vec2 f) {
	acceleration += f / mass;
}

void Physics::addVelocity(glm::vec2 vel) {
	velocity += vel;
}

void Physics::setDrag(float drag) {
	if (drag < 0 || drag > 1) {
		std::cout << "Drag must be within the range of 0-1" << std::endl;
		return;
	}

	this->drag = drag;
}

void Physics::applyGravity() {
	velocity += gravity * GameEngine::getInstance()->getTime()->getDeltaTime();
}
void Physics::update(Transform& transform) {
	if (!enablePhysics) {
		return;
	}

	if (enableGravity) {
		applyGravity();
	}

	float deltaTime = GameEngine::getInstance()->getTime()->getDeltaTime();

	velocity += acceleration;
	acceleration += -acceleration * drag;
	force = mass * (velocity / deltaTime);

	//if (abs(force.x) < 0.05f) {
	//	force.x = 0.0f;
	//}
	//if (abs(force.y) < 0.05f) {
	//	force.y = 0.0f;
	//}

	glm::vec3 velocityVec = glm::vec3(velocity.x, velocity.y, 0.0f);
	//velocityVec *= CONST_DELTA_TIME; // for testing, change later
	velocityVec *= deltaTime;
	lastPos = transform.getPosition();
	transform.translate(velocityVec);
}

glm::vec3 Physics::getLastPosition() {
	return lastPos;
}

glm::vec2 Physics::getVelocity() {
	return velocity;
}

glm::vec2 Physics::getAcceleration() {
	return acceleration;
}
glm::vec2 Physics::getGravity() {
	return gravity;
}

float Physics::getMass() {
	return mass;
}

glm::vec2 Physics::getForce() {
	return force;
}

float Physics::getDrag() {
	return drag;
}

void Physics::setLastPosition(glm::vec3 pos) {
	lastPos = pos;
}

void Physics::setEnablePhysics(bool flag) {
	enablePhysics = flag;
}

void Physics::setEnableGravity(bool flag) {
	enableGravity = flag;
}

bool Physics::isGravityEnable() const {
	return enableGravity;
}

bool Physics::isPhysicsEnable() const {
	return enablePhysics;
}