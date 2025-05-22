#include "GameEngine.h"
#include "LightSource.h"
#include "LivingEntity.h"
#include "Random.h"
#include <iostream>

LivingEntity::Status::Status(StatusType type, float cooldown) {
    this->type = type;
    this->cooldown = cooldown;
    remainingTime = this->cooldown;
}

LivingEntity::LivingEntity() : health(100), isStun(false), canTakeDamage(true), isFacingRight(false),
isDamageOverlayed(false), damageOverlayTimeRemaining(0.0f), isInKnockback(false), knockbackDurationRemaining(0.0f),
isAffectedByLighting(false), forceIgnoreLighting(false) {
    emitter = new ParticleSystem();
    maxHealth = health;
}

LivingEntity::LivingEntity(int hp) : health(hp), isStun(false), canTakeDamage(true), isFacingRight(false),
isDamageOverlayed(false), damageOverlayTimeRemaining(0.0f), isInKnockback(false), knockbackDurationRemaining(0.0f),
isAffectedByLighting(false), forceIgnoreLighting(false) {
    emitter = new ParticleSystem();
    maxHealth = health;
}

LivingEntity::LivingEntity(std::string name) : TexturedObject(name), health(100), isStun(false), canTakeDamage(true),
isFacingRight(false), isDamageOverlayed(false), damageOverlayTimeRemaining(0.0f), isInKnockback(false), knockbackDurationRemaining(0.0f), 
isAffectedByLighting(false), forceIgnoreLighting(false) {
    emitter = new ParticleSystem();
    maxHealth = health;
}

LivingEntity::LivingEntity(std::string name, int hp) : TexturedObject(name), health(hp), isStun(false), canTakeDamage(true),
isFacingRight(false), isDamageOverlayed(false), damageOverlayTimeRemaining(0.0f), isInKnockback(false), knockbackDurationRemaining(0.0f), 
isAffectedByLighting(false), forceIgnoreLighting(false) {
    emitter = new ParticleSystem();
    maxHealth = health;
}

LivingEntity::~LivingEntity() {
    if (emitter != nullptr) {
        delete emitter;
    }
}

void LivingEntity::setHealth(int hp) {
    this->health = hp;
}

void LivingEntity::setMaxHealth(int hp) {
    this->maxHealth = hp;
}

void LivingEntity::setCanTakeDamage(bool value) {
    canTakeDamage = value;
}

void LivingEntity::setIsFacingRight(bool value) {
    isFacingRight = value;
}

int LivingEntity::getHealth() const {
    return health;
}

int LivingEntity::getMaxHealth() const {
    return maxHealth;
}

bool LivingEntity::getCanTakeDamage() const {
    return canTakeDamage;
}

bool LivingEntity::getIsDead() const {
    return health <= 0;
}

bool LivingEntity::getIsAlive() const {
    return health > 0;
}

void LivingEntity::handleHealth(list<DrawableObject*>& objectsList) {
    if (this->getIsDead()) {
        this->onDeath(objectsList);
    }
}

void LivingEntity::onDeath(list<DrawableObject*>& objectsList) {}

void LivingEntity::addStatus(Status newStatus) { 
    std::list<Status>::iterator itr = std::find(statusList.begin(), statusList.end(), newStatus);
    if (itr != statusList.end()) {
        Status& status = *itr;
        status.remainingTime = status.cooldown;
        return;
    }

    statusList.emplace_back(newStatus);
}

void LivingEntity::deleteStatus(int index) {
    
    std::list<Status>::iterator itr;
    itr = statusList.begin();
    advance(itr, index);
    statusList.erase(itr);
    
}

std::list<LivingEntity::Status>::iterator LivingEntity::deleteStatus(std::list<Status>::iterator itr) {
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

        Status& status = *itr;
        switch (status.type) {
            case STUN:
                
                isStun = true;
                
                break;

            case POISON:
                //takeDamage(1);
                break;

            case BURNING:
                handleBurning();
                break;
        }
        if (status.remainingTime <= 0.0f) {
            if (status.type == STUN) {
                isStun = false;
            }
            else if (status.type == BURNING) {
                forceIgnoreLighting = false;
            }
            

            itr = deleteStatus(itr);
            if (itr != statusList.begin()) --itr;
            if (itr == statusList.end()) break;
        }
        else {
            status.remainingTime -= dt;
        }
    }
}

void LivingEntity::handleBurning() {
    static float emitTimer = 0.0f;
    static const float timeBetweenEmit = 0.0833f;

    static float damageTimer = 0.0f;
    static const float timeBetweenFireDamage = 0.4f;

    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    emitTimer += dt;
    damageTimer += dt;

    if (damageTimer > timeBetweenFireDamage) {
        damageTimer = 0.0f;
        LivingEntity::takeDamage(LivingEntityStat::BURNING_DAMAGE, true); // fire damage per damage cooldown
    }

    if (emitTimer > timeBetweenEmit) {
        emitTimer = 0.0f;
        int randEmitNum = Random::Int() % 3 + 3;
        for (int i = 0; i < randEmitNum; i++) {
            ParticleProperties particleProps = ParticleProperties(
                this->getTransform().getPosition() + glm::vec3(0.4f * Random::Float() - 0.2f, 0.15f * Random::Float() - 0.2f, 0),
                glm::vec2(1.0f * Random::Float() - 0.5f, 2.5f * Random::Float() + 0.25f),
                glm::vec2(-0.25f, 0.25f),
                glm::vec3(1.0f, 0.47f, 0),
                glm::vec3(0.2f, 0.2f, 0.2f),
                0.2f, 0.1f, 0.02f, 1.0f
            );
            this->emitter->emit(particleProps);
        }
    }

    forceIgnoreLighting = true;
    renderBrightness = 1.0f;
}

void LivingEntity::takeDamage(int damage) {
    if (!canTakeDamage) {
        return;
    }

    health -= damage;

    isDamageOverlayed = true;
    damageOverlayTimeRemaining = LivingEntityStat::DAMAGE_OVERLAY_DURATION;
}

void LivingEntity::takeDamage(int damage, bool ignoreCanTakeDamage) {
    health -= damage;

    isDamageOverlayed = true;
    damageOverlayTimeRemaining = LivingEntityStat::DAMAGE_OVERLAY_DURATION;
}

void LivingEntity::heal(int amount) {
    health += amount;

    if (health > maxHealth) {
        health = maxHealth;
    }
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
            vel.x = knockbackDurationRemaining / vel.x;
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

void LivingEntity::postUpdateBehavior() {};

void LivingEntity::update(list<DrawableObject*>& objectsList) {
    applyStatus(GameEngine::getInstance()->getTime()->getDeltaTime());

    handleHealth(objectsList);
    
    updateBehavior(objectsList);
    postUpdateBehavior();
    

    handleKnockback();

    DrawableObject::update(objectsList);

    if (isAffectedByLighting && !forceIgnoreLighting) {
        handleLighting(objectsList);
    }

    handleDamageOverlay();

    glm::vec3 currentScale = this->transform.getScale();
    currentScale.x = abs(currentScale.x);
    isFacingRight ? this->transform.setScale(currentScale.x, currentScale.y) : this->transform.setScale(-currentScale.x, currentScale.y);

}

bool operator==(LivingEntity::Status s1, LivingEntity::Status s2) {
    return s1.type == s2.type;
}