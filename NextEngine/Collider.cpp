#include "Collider.h"
#include "DrawableObject.h"
#include "RayObject.h"
//#include "CollisionHandler.h"

Collider::Collider(DrawableObject* object) : object(object), width(1), height(1) {
	enableCollision = true;
	trigger = false;
	transform = Transform();

	collisionFlag = 0;
}

Collider::Collider(DrawableObject* object, float width, float height) : object(object), width(width), height(height) {
	enableCollision = true;
	trigger = false;
	transform = Transform();

	collisionFlag = 0;
}

Collider::~Collider() {
	for (pair<Collider*, CollisionState> pair : collisionMap) {
		Collider* col = pair.first;
		std::map<Collider*, CollisionState>& otherMap = col->getCollisionMap();

		if (otherMap.count(this) == 0) {
			continue;
		}

		otherMap.erase(this);
	}
}

void Collider::setEnableCollision(bool value) {
	enableCollision = value;
}

void Collider::setTrigger(bool value) {
	trigger = value;
}

void Collider::setTransform(const Transform& transform) {
	this->transform = transform;
}

void Collider::setWidth(float width) {
	this->width = width;
}

void Collider::setHeight(float height) {
	this->height = height;
}

void Collider::setDimension(float width, float height) {
	this->width = width;
	this->height = height;
}

void Collider::setCollisionFlag(COLLISION_FLAG flag) {
	this->collisionFlag = flag;
}

Transform& Collider::getTransform() {
	return transform;
}

Transform Collider::getGlobalTransform(Transform& selfTransform) {
	return selfTransform.getGlobalTransform(this->transform);
}

float Collider::getWidth() const {
	return width * const_cast<Transform*>(&transform)->getScale().x;
}

float Collider::getHeight() const {
	return height * const_cast<Transform*>(&transform)->getScale().y;
}

std::map<Collider*, Collider::CollisionState>& Collider::getCollisionMap() {
	return collisionMap;
}

Collider::CollisionState Collider::getCollisionState(Collider* other) {
	if (collisionMap.find(other) == collisionMap.end()) {
		return Collider::NONE;
	}

	return collisionMap[other];
}

bool Collider::isEnable() const {
	return enableCollision;
}

bool Collider::isTrigger() const {
	return trigger;
}

COLLISION_FLAG Collider::getCollisionFlag() const {
	return collisionFlag;
}

DrawableObject* Collider::getObject() {
	return object;
}