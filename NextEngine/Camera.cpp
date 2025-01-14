#include "Camera.h"
#include <iostream>

Camera::Camera()
    : position(0.0f, 0.0f,0.0f),  
    target(0.0f, 0.0f, 0.0f),   
    up(0.0f, 1.0f, 0.0f) {       
}

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;

}

void Camera::setTarget(const glm::vec3& targetPos) {
    target = targetPos;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getTarget() const {
    return target;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, target, up);
}

void Camera::moveLeft(float distance) {
    position.x -= distance;
    target.x -= distance;
}

void Camera::moveRight(float distance) {
    position.x += distance;
    target.x += distance;
}

void Camera::moveUp(float distance) {
    position.y += distance;
    target.y += distance;
}

void Camera::moveDown(float distance) {
    position.y -= distance;
    target.y -= distance;
}
