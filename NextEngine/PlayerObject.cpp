// PlayerObject.cpp
#include "PlayerObject.h"
#include "GameEngine.h"

constexpr float ATTACK_DURATION = 0.2f; // Duration for the attack collider to be active
constexpr float ATTACK_COOLDOWN = 0.5f; // Cooldown between attacks

PlayerObject::PlayerObject(PlayerInfo& playerInfo) : LivingEntity(playerInfo.name, playerInfo.health) {
    this->damage = playerInfo.damage;

    setTexture("../Resource/Texture/SIZENextCentury_Player_Idle-Sheet.png");
    initAnimation(1, 6);

    getAnimationComponent()->addState("Idle", 0, 6, true);

    //getTransform().setScale(50.0f, 50.0f);
    addColliderComponent();
    addPhysicsComponent();

    moveDirection = glm::vec3();
    isFacingRight = true;

    dodgeTimeElapsed = 0.0f;
    dodgeCooldownLeft = 0.0f;
    canDodge = true;
    isDodging = false;
    canMove = true;

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

void PlayerObject::move(glm::vec2 direction) {
    if (isDodging) {
        return;
    }

    if (!canMove) {
        return;
    }

    isFacingRight = direction.x > 0.0f ? true : false;

    this->moveDirection.x += direction.x;
}

void PlayerObject::jump() {
    if (isDodging) {
        return;
    }

    bool grounded = collider->getCollisionFlag() & COLLISION_DOWN;
    if (grounded) {
        glm::vec2 vel = this->physics->getVelocity();
        vel.y = PlayerStat::JUMP_VELOCITY;
        this->physics->setVelocity(vel);
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
    attackHitbox = new DamageCollider<EnemyObject>(this, damage, PlayerStat::ATTACK_HITBOX_ACTIVE_TIME);
    //attackHitbox = new DamageCollider<EnemyObject>(this, damage, 2.5f);
    attackHitbox->setActive(false);
    attackHitbox->setFollowOwner(true);
    attackHitbox->setFollowOffset(glm::vec3(0.5f, 0, 0));
    attackHitbox->getColliderComponent()->setWidth(1.5f);
    objectsList.emplace_back(attackHitbox);

    this->getAnimationComponent()->setState("Idle");
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

        glm::vec2 dodgeVelocity = isFacingRight ? glm::vec2(PlayerStat::DODGE_VELOCITY, vel.y) : glm::vec2(-PlayerStat::DODGE_VELOCITY, vel.y);
        this->physics->setVelocity(dodgeVelocity);

        dodgeTimeElapsed += dt;

        if (dodgeTimeElapsed >= PlayerStat::DODGE_DURATION) {
            isDodging = false;
            dodgeCooldownLeft = PlayerStat::DODGE_COOLDOWN;
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
    this->physics->addVelocity(glm::vec2(moveDirection.x * PlayerStat::ACCEL_SPEED * dt, 0.0f));
    vel = this->physics->getVelocity();
    if (abs(vel.x) > PlayerStat::MOVE_SPEED) {
        vel.x = PlayerStat::MOVE_SPEED * moveDirection.x;
        this->physics->setVelocity(vel);
    }

    vel = this->physics->getVelocity();
    if (vel.x != 0.0f && moveDirection.x == 0.0f) {
        float dragDir = vel.x > 0.0f ? -1 : 1;
        this->physics->addVelocity(glm::vec2(dragDir * PlayerStat::DECEL_SPEED * dt, 0.0f));
        float newVel = this->physics->getVelocity().x;

        if ((vel.x < 0) != (newVel < 0)) {
            this->physics->setVelocity(glm::vec2(0.0f, vel.y));
        }
    }
    
    moveDirection.x = 0.0f; // Reset move direction for next frame
}


void PlayerObject::attack() {
    if (attackCooldown > 0.0f) {
        return; 
    }

    attackHitbox->trigger(transform.getPosition());

    attackCooldown = 1.0f; 
    std::cout << "Player attacked!" << std::endl;
}

void PlayerObject::onTriggerEnter(Collider* collider) {

}