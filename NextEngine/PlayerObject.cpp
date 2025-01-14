// PlayerObject.cpp
#include "PlayerObject.h"
#include "GameEngine.h"

constexpr float ATTACK_DURATION = 0.2f; // Duration for the attack collider to be active
constexpr float ATTACK_COOLDOWN = 0.5f; // Cooldown between attacks

PlayerObject::PlayerObject(PlayerInfo& playerInfo) : LivingEntity(playerInfo.name, playerInfo.health), movementInfo(playerInfo.movementInfo) {
    this->damage = playerInfo.damage;

    setTexture("../Resource/Texture/SIZENextCentury_Player_Idle-Sheet.png");
    initAnimation(6, 1);
    getTransform().setScale(1, 1);
    addColliderComponent();
    addPhysicsComponent();

    moveDirection = glm::vec3();
    isFacingRight = true;

    dodgeTimeElapsed = 0.0f;
    dodgeCooldownLeft = 0.0f;
    canDodge = true;
    isDodging = false;

    attackCooldown = ATTACK_COOLDOWN;
    attackHitbox = nullptr;
}

PlayerObject::~PlayerObject() {
    if (attackHitbox != nullptr) {
        destroyObject(attackHitbox);
    }
}

int PlayerObject::getDamage() const { 
    return damage; 
}

void PlayerObject::setDamage(int damage) {
    this->damage = damage;
    attackHitbox->setDamage(this->damage);
}

void PlayerObject::setMovementInfo(MovementInfo movementInfo) {
    this->movementInfo = movementInfo;
}

void PlayerObject::move(glm::vec2 direction) {
    if (isDodging) {
        return;
    }

    isFacingRight = direction.x > 0.0f ? true : false;

    this->moveDirection.x += direction.x;
}

void PlayerObject::jump() {
    if (isDodging) {
        return;
    }

    bool grounded = collider->getCollisionFlag() && COLLISION_DOWN;
    if (grounded) {
        glm::vec2 vel = physics->getVelocity();
        vel.y = movementInfo.jumpVelocity;
        physics->setVelocity(vel);
    }
}

void PlayerObject::dodge() {
    if (!canDodge) {
        return;
    }

    isDodging = true;
    dodgeTimeElapsed = 0.0f;
}

void PlayerObject::start(list<DrawableObject*>& objectsList) {
    attackHitbox = new DamageCollider<EnemyObject>(this, damage, HITBOX_ACTIVE_TIME);
    //attackHitbox = new DamageCollider<EnemyObject>(this, damage, 2.5f);
    attackHitbox->setActive(false);
    attackHitbox->setFollowOwner(true);
    attackHitbox->setFollowOffset(glm::vec3(0.5f, 0, 0));
    attackHitbox->getColliderComponent()->setWidth(1.5f);
    objectsList.emplace_back(attackHitbox);
}

void PlayerObject::updateBehavior(list<DrawableObject*>& objectsList) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    // Handle attack cooldown
    if (attackCooldown > 0.0f) {
        attackCooldown -= dt;
        if (attackCooldown < 0.0f) {
            attackCooldown = 0.0f;
        }
    }

    glm::vec2 vel = this->physics->getVelocity();

    if (isDodging) {
        canDodge = false;
        setCanTakeDamage(false);

        glm::vec2 dodgeVelocity = isFacingRight ? glm::vec2(DODGE_VELOCITY, vel.y) : glm::vec2(-DODGE_VELOCITY, vel.y);
        this->physics->setVelocity(dodgeVelocity);

        dodgeTimeElapsed += dt;

        if (dodgeTimeElapsed >= DODGE_DURATION) {
            isDodging = false;
            dodgeCooldownLeft = DODGE_COOLDOWN;
        }

        return;
    }
    else {
        dodgeCooldownLeft -= dt;

        if (dodgeCooldownLeft <= 0) {
            dodgeCooldownLeft = 0.0f;
            canDodge = true;
        }

        setCanTakeDamage(true);
    }

    // Handle movement
    vel.x = moveDirection.x * movementInfo.speed;
    this->physics->setVelocity(vel);
    moveDirection.x = 0.0f; // Reset move direction for the next frame
}


void PlayerObject::attack() {
    if (attackCooldown > 0.0f) {
        return; 
    }

    attackHitbox->trigger(transform.getPosition());

    attackCooldown = 1.0f; 
    std::cout << "Player attacked!" << std::endl;
}