#include "GameEngine.h"
#include "LightSource.h"
#include "LivingEntity.h"
#include <iostream>

LivingEntity::Status::Status(StatusType type, float cooldown) {
    this->type = type;
    this->cooldown = cooldown;
    remainingTime = this->cooldown;
}

LivingEntity::LivingEntity() : health(100), isDead(false), isStun(false), canTakeDamage(true), isFacingRight(false),
isDamageOverlayed(false), damageOverlayTimeRemaining(0.0f), knockbackDurationRemaining(0.0f), isAffectedByLighting(false) {}

LivingEntity::LivingEntity(int hp) : health(hp), isDead(false), isStun(false), canTakeDamage(true), isFacingRight(false),
isDamageOverlayed(false), damageOverlayTimeRemaining(0.0f), isInKnockback(false), knockbackDurationRemaining(0.0f), isAffectedByLighting(false) {}

LivingEntity::LivingEntity(std::string name) : TexturedObject(name), health(100), isDead(false), isStun(false), canTakeDamage(true),
isFacingRight(false), isDamageOverlayed(false), damageOverlayTimeRemaining(0.0f), isInKnockback(false), knockbackDurationRemaining(0.0f), 
isAffectedByLighting(false) {}

LivingEntity::LivingEntity(std::string name, int hp) : TexturedObject(name), health(hp), isDead(false), isStun(false), canTakeDamage(true),
isFacingRight(false), isDamageOverlayed(false), damageOverlayTimeRemaining(0.0f), isInKnockback(false), knockbackDurationRemaining(0.0f), 
isAffectedByLighting(false) {}


void LivingEntity::setHealth(int hp) {
    this->health = hp;
}

void LivingEntity::setCanTakeDamage(bool value) {
    canTakeDamage = value;
}

int LivingEntity::getHealth() const {
    return health;
}

bool LivingEntity::getCanTakeDamage() const {
    return canTakeDamage;
}

bool LivingEntity::getIsDead() const {
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
    
    for (std::list<Status>::iterator itr = statusList.begin(); itr != statusList.end(); ++itr) {

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
            --itr;
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

    isDamageOverlayed = true;
    damageOverlayTimeRemaining = LivingEntityStat::DAMAGE_OVERLAY_DURATION;
}

void LivingEntity::handleDamageOverlay() {
    if (!isDamageOverlayed) {
        return;
    }

    colorOverlay = glm::vec4(1.0f, 0.0f, 0.0f, 0.75f);

    damageOverlayTimeRemaining -= GameEngine::getInstance()->getTime()->getDeltaTime();
    if (damageOverlayTimeRemaining <= 0.0f) {
        damageOverlayTimeRemaining = 0.0f;
        isDamageOverlayed = false;
        colorOverlay = glm::vec4();
    }
}

void LivingEntity::handleLighting(std::list<DrawableObject*>& objectsList) {
    unsigned int numOfLight = 0;
    float totalBrightness = 0.0f;

    for (DrawableObject* obj : objectsList) {
        LightSource* l = dynamic_cast<LightSource*>(obj);

        if (l != NULL) {
            totalBrightness += l->getTextureBrightness(this);
            numOfLight++;
        }
    }

    if (numOfLight == 0) {
        this->renderBrightness = LightSourceData::MINIMUM_BRIGHTNESS;
        return;
    }

    this->renderBrightness = LightSource::normalizeBrightness(totalBrightness / static_cast<float>(numOfLight));
}

void LivingEntity::setAffectedByLighting(bool value) {
    this->isAffectedByLighting = value;
}

void LivingEntity::knockback(glm::vec2 velocityDirection, float duration) {
    if (isInKnockback) {
        return;
    }
    
    isInKnockback = true;
    this->getPhysicsComponent()->setVelocity(velocityDirection);
    this->knockbackDurationRemaining = duration;
}

void LivingEntity::handleKnockback() {
    if (isInKnockback) {
        float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
        this->knockbackDurationRemaining -= dt;

        if (knockbackDurationRemaining <= 0.0f) {
            isInKnockback = false;
            return;
        }

        glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
        if (abs(vel.x) > 0.1f) {
            float dragDir = vel.x > 0.0f ? -1.0f : 1.0f;
            vel.x *= dragDir;
            vel.x /= knockbackDurationRemaining;
        }

        this->physics->addVelocity(glm::vec2(vel.x * dt, 0.0f));
    }
}

bool LivingEntity::getIsStun() const {
    return isStun;
}

bool LivingEntity::getIsFacingRight() const {
    return isFacingRight;
}

void LivingEntity::update(list<DrawableObject*>& objectsList) {
    updateBehavior(objectsList);

    handleKnockback();

    DrawableObject::update(objectsList);

    if (isAffectedByLighting) {
        handleLighting(objectsList);
    }

    handleDamageOverlay();

    glm::vec3 currentScale = this->transform.getScale();
    currentScale.x = abs(currentScale.x);
    isFacingRight ? this->transform.setScale(currentScale.x, currentScale.y) : this->transform.setScale(-currentScale.x, currentScale.y);


    applyStatus(GameEngine::getInstance()->getTime()->getDeltaTime());
}

bool operator==(LivingEntity::Status s1, LivingEntity::Status s2) {
    return s1.type == s2.type;
}