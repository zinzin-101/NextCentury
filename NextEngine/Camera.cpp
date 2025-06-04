#include "Camera.h"
#include "GameEngine.h"
#include "Random.h"
#include <iostream>

Camera::Camera()
    : position(0.0f, 0.0f,0.0f),  
    target(nullptr),   
    up(0.0f, 1.0f, 0.0f), 
    deadZoneX(2.0f), deadZoneY(0.0f){

    isShaking = false;
    shakeDurationRemaining = 0.0f;
}

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Camera::setDeadLimitBool(bool isDeadLimit) {
    isUsingDeadLimit = isDeadLimit;
}

void Camera::setDeadLimitMinMax(float min, float max) {
    deadLimitPosMin = min;
    deadLimitPosMax = max;
}

void Camera::followTarget() {
    //cout << "target is set" << target->getTransform().getPosition().x << endl;
    if (target == nullptr) {
        cout << "can't follow target is null" << endl;
        return;
    }
    glm::vec3 newPos;
    float x = position.x;;
    float y = target->getTransform().getPosition().y + offset.y;
    //cout << y << endl;
    
    if (GameEngine::getInstance()->getRenderer()->getIsViewportEnabled()) {
        bool movable = true;
        if (isUsingDeadLimit) {
            if (abs(target->getTransform().getPosition().x - deadLimitPosMin) < deadZoneX + 1) { // Might need to change deadzone to some other number
                movable = false;
            }
            if (abs(target->getTransform().getPosition().x - deadLimitPosMax) < deadZoneX + 1) {
                movable = false;
            }
        }
        
        if (movable) {
            if (abs(target->getTransform().getPosition().x - position.x) > deadZoneX) {

                if (target->getTransform().getPosition().x - position.x > 0) {
                    x = target->getTransform().getPosition().x - deadZoneX;
                }
                else {
                    x = target->getTransform().getPosition().x + deadZoneX;
                }
            }
            else {
                x = position.x;
            }
        }
    }
    else {
        x = target->getTransform().getPosition().x;
        y = target->getTransform().getPosition().y;
    }

    if (isShaking) {
        x += Random::Float() - 0.5f;
        y += Random::Float() - 0.5f;
    }

    newPos = glm::vec3(x , 0/*y*/, 0.0f);
    setPosition((1-0.2f) * this->position + 0.2f * newPos);
    //setPosition(glm::vec3(target->getTransform().getPosition().x, 0.0f, 0.0f));
}


void Camera::setTarget(DrawableObject* target) {
    this->target = target;
    if (this->target == nullptr) {
        //cout << "o" << endl;
    }
    else {
        //cout << "k" << endl;
    }
}

void Camera::setOffset(glm::vec3 offset) {
    this->offset = offset;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

DrawableObject* Camera::getTarget() const {
    return target;
}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 a;
    if (target == nullptr) {
        a = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else {
        a = target->getTransform().getPosition();
    }
    return glm::lookAt(position, a, up);
}

void Camera::moveLeft(float distance) {
    position.x -= distance;
    //target->getPosition().x -= distance;
}

void Camera::moveRight(float distance) {
    position.x += distance;
    //target.x += distance;
}

void Camera::moveUp(float distance) {
    position.y += distance;
    //target.y += distance;
}

void Camera::moveDown(float distance) {
    position.y -= distance;
    //target.y -= distance;
}

void Camera::startShake(float duration) {
    isShaking = true;
    shakeDurationRemaining = duration;
}

void Camera::updateCamera() {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTimeRealTime();
    
    if (shakeDurationRemaining > 0.0f && isShaking) {
        shakeDurationRemaining -= dt;

        if (shakeDurationRemaining <= 0.0f) {
            isShaking = false;
        }
    }
}