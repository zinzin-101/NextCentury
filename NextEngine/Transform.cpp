#include "Transform.h"
#include "gtc/matrix_transform.hpp"

Transform::Transform() {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->rotation = 0.0f;
	this->scale = glm::vec3(1.0, 1.0, 1.0);
}

Transform::Transform(glm::vec3 position, float rotation, glm::vec3 scale):
	position(position), rotation(rotation), scale(scale) {
}

void Transform::setPosition(glm::vec3 position) {
	this->position = position;
}

void Transform::setPosition(float x, float y) {
	this->position.x = x;
	this->position.y = y;
}

void Transform::setRotationRad(float rotation) {
	this->rotation = rotation;
}

void Transform::setRotationDeg(float rotation) {
	rotation = glm::radians(rotation);
	this->rotation = rotation;
}

void Transform::setRotation(float angle) {
	setRotationDeg(angle);
}

void Transform::setScale(glm::vec3 scale) {
	this->scale = scale;
}

void Transform::setScale(float scale) {
	this->scale = glm::vec3(1.0, 1.0, 1.0) * scale;
}

void Transform::setScale(float scaleX, float scaleY) {
	this->scale = glm::vec3(scaleX, scaleY, 1.0);
}

void Transform::scales(glm::vec3 scale) {
	this->scale.x *= scale.x;
	this->scale.y *= scale.y;
}

void Transform::scales(float scale) {
	this->scale *= scale;
}

void Transform::scales(float scaleX, float scaleY) {
	this->scale.x *= scaleX;
	this->scale.y *= scaleY;
}

glm::vec3 Transform::getPosition() {
	return this->position;
}

float Transform::getRotationRad() {
	return this->rotation;
}

float Transform::getRotationDeg() {
	float degrees = glm::degrees(rotation);
	return degrees;
}

float Transform::getRotation() {
	return getRotationDeg();
}
glm::vec3 Transform::getScale() {
	return this->scale;
}

void Transform::translate(glm::vec3 translate) {
	this->position += translate;
}

void Transform::translate(float x, float y) {
	this->position.x += x;
	this->position.y += y;
}

void Transform::rotateRad(float rotate) {
	this->rotation += rotate;
}

void Transform::rotateDeg(float rotate) {
	rotate = glm::radians(rotate);
	this->rotation += rotate;
}

void Transform::rotate(float angle) {
	rotateDeg(angle);
}

Transform Transform::getGlobalTransform(Transform& objectTransform) {
	Transform globalTransform(
		this->position + objectTransform.position,
		this->rotation + objectTransform.rotation,
		this->scale * objectTransform.scale
	);

	return globalTransform;
}

glm::mat4 Transform::getTransformMat4() {
	glm::mat4 _transform = glm::mat4(1.0);
	_transform = glm::translate(_transform, glm::vec3(position.x, position.y, 0));
	_transform = glm::rotate(_transform, rotation, glm::vec3(0, 0, 1));
	_transform = glm::scale(_transform, glm::vec3(scale.x, scale.y, 1));
	return _transform;
}

Transform Transform::operator+(const Transform& other) {
	return Transform(
		this->position + other.position,
		this->rotation + other.rotation,
		this->scale * other.scale
	);
}

Transform Transform::operator-(const Transform& other) {
	return Transform(
		this->position - other.position,
		this->rotation - other.rotation,
		this->scale / other.scale
	);
}

Transform Transform::operator*(const float& multiplier) {
	return Transform(
		this->position * multiplier,
		this->rotation,
		this->scale * multiplier
	);
}