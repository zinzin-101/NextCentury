#include "GameEngine.h"
#include "LivingEntity.h"
#include <iostream>

LivingEntity::Status::Status(StatusType type, float cooldown) {
    this->type = type;
    this->cooldown = cooldown;
    remainingTime = this->cooldown;
}

LivingEntity::LivingEntity() : health(100), isDead(false), isStun(false), canTakeDamage(true), isFacingRight(false) {}

LivingEntity::LivingEntity(int hp) : health(hp), isDead(false), isStun(false), canTakeDamage(true), isFacingRight(false) {}

LivingEntity::LivingEntity(std::string name) : TexturedObject(name), health(100), isDead(false), isStun(false), canTakeDamage(true), isFacingRight(false) {}

LivingEntity::LivingEntity(std::string name, int hp) : TexturedObject(name), health(hp), isDead(false), isStun(false), canTakeDamage(true), isFacingRight(false) {}


void LivingEntity::setHealth(int hp) {
    this->health = hp;
}

void LivingEntity::setCanTakeDamage(bool value) {
    canTakeDamage = value;
}

int LivingEntity::getHealth() {
    return health;
}

bool LivingEntity::getIsDead() {
    return isDead;
}

void LivingEntity::addStatus(Status newStatus) { 
    std::cout << "Adding";
    std::list<Status>::iterator itr = std::find(statusList.begin(), statusList.end(), newStatus);
    if (itr != statusList.end()) {
        Status& status = *itr;
        status.remainingTime = status.cooldown;
        return;
    }

    statusList.emplace_back(newStatus);
    std::cout << newStatus.type;
    cout << "added" << endl;
}

void LivingEntity::deleteStatus(int index) {
    
    std::list<Status>::iterator itr;
    itr = statusList.begin();
    advance(itr, index);
    statusList.erase(itr);
    
}

std::list<LivingEntity::Status>::iterator LivingEntity::deleteStatus(std::list<Status>::iterator itr) {
    std::cout << "deleting status" << endl;
    return statusList.erase(itr);
}

bool LivingEntity::findStatus(Status target) {
    for (Status& status : statusList) {
        if (status.type == target.type) {
            return true;
        }
    }
    return false;
}

std::list<LivingEntity::Status>& LivingEntity::getStatusList() {
    return statusList;
}

void LivingEntity::applyStatus(float dt) { // NOt required???
    // Placeholder for status application logic std::list<Status>::iterator itr = statusList.begin(); itr != statusList.end(); itr++
    if (statusList.empty()) {
        return;
    }

    //cout << "applying" << endl;
    
    for (std::list<Status>::iterator itr = statusList.begin(); itr != statusList.end(); itr++) {

        cout << "enter" << endl;
        Status& status = *itr;
        switch (status.type) {
            case STUN:
                
                isStun = true;
                
                break;

            case POISON:
                takeDamage(1);
                break;

            case MOODENG:
                isStun = true;
                break;
        }
        if (status.remainingTime <= 0.0f) {
            if (status.type == STUN || status.type == MOODENG) {
                isStun = false;
                cout << "falsey";
            }
            else {
                cout << "What";
            }
            itr = deleteStatus(itr);
            itr--;
            //itr = statusList.erase(itr);
            cout << "removed" << endl;
        }
        else {
            status.remainingTime -= dt;
            cout << "current time " << status.remainingTime << endl;
        }

        if (itr == statusList.end()){
            break;
        }
    }
}

void LivingEntity::takeDamage(int damage) {
    if (!canTakeDamage) {
        return;
    }

    health -= damage;
    if (health <= 0) {
        health = 0;
        isDead = true;
    }
}

bool LivingEntity::getIsStun() {
    return isStun;
}

bool LivingEntity::getIsFacingRight() {
    return isFacingRight;
}

void LivingEntity::update(list<DrawableObject*>& objectsList) {
    updateBehavior(objectsList);
    DrawableObject::update(objectsList);
    applyStatus(GameEngine::getInstance()->getTime()->getDeltaTime());
}

void LivingEntity::updateBehavior(list<DrawableObject*>& objectsList) {}

bool operator==(LivingEntity::Status s1, LivingEntity::Status s2) {
    return s1.type == s2.type;
}