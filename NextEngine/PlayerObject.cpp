// PlayerObject.cpp
#include "PlayerObject.h"
#include "GameEngine.h"

constexpr float ATTACK_DURATION = 0.2f; // Duration for the attack collider to be active
constexpr float ATTACK_COOLDOWN = 0.5f; // Cooldown between attacks

PlayerObject::PlayerObject(PlayerInfo& playerInfo) : LivingEntity(playerInfo.name, playerInfo.health) {
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
        vel.y = PlayerStat::JUMP_HEIGHT;
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
    attackHitbox = new DamageCollider<EnemyObject>(this, damage, PlayerStat::HITBOX_ACTIVE_TIME);
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
    vel.x = moveDirection.x * PlayerStat::MOVE_SPEED;
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

/// debugging ///
void PlayerObject::onTriggerEnter(Collider* collider) {
    //std::cout << "player enters: " << collider->getObject()->getName() << std::endl;
}