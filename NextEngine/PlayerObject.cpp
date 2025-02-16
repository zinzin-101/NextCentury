// PlayerObject.cpp
#include "PlayerObject.h"
#include "GameEngine.h"

//constexpr float ATTACK_DURATION = 0.2f; // Duration for the attack collider to be active
//constexpr float ATTACK_COOLDOWN = 0.5f; // Cooldown between attacks

PlayerObject::PlayerObject(PlayerInfo& playerInfo) : LivingEntity(playerInfo.name, playerInfo.health) {
    this->damage = playerInfo.damage;

    setTexture("../Resource/Texture/playertest.png");
    initAnimation(4, 6);

    getAnimationComponent()->addState("Idle", 0, 0, 6, true);

    getAnimationComponent()->addState("Combo1", 1, 0, 6, false);
    getAnimationComponent()->addState("Combo2", 2, 0, 6, false);
    getAnimationComponent()->addState("Combo3", 3, 0, 6, false);

    getTransform().setScale(1, 1);
    addColliderComponent();
    addPhysicsComponent();

    moveDirection = glm::vec3();
    isFacingRight = true;

    dodgeTimeElapsed = 0.0f;
    dodgeCooldownLeft = 0.0f;
    canDodge = true;
    isDodging = false;
    canMove = true;

    isAttacking = false;
    isInAttackState = false;
    attackCooldownRemaining = 0.0f;
    comboFrame[PlayerCombo::FIRST] = Combo(2, 3);
    comboFrame[PlayerCombo::SECOND] = Combo(2, 3);
    comboFrame[PlayerCombo::THIRD] = Combo(2, 3);
    currentCombo = PlayerCombo::NONE;

    timeToResetComboRemaining = 0.0f;
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
    if (isDodging || !canMove || isAttacking) {
        return;
    }

    isFacingRight = direction.x > 0.0f ? true : false;

    this->moveDirection.x += direction.x;
}

void PlayerObject::jump() {
    if (isDodging || !canMove) {
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
    if (!canDodge || !canMove) {
        return;
    }

    isDodging = true;
    dodgeTimeElapsed = 0.0f;
}

void PlayerObject::start(list<DrawableObject*>& objectsList) {
    attackHitbox = new DamageCollider<EnemyObject>(this, damage, -1);
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
    glm::vec2 vel = this->physics->getVelocity();

    if (attackCooldownRemaining > 0.0f) {
        attackCooldownRemaining -= dt;
    }

    if (isInAttackState) {
        canMove = false;

        float decreaseSpeed = PlayerStat::ATTACK_DASH_VELOCITY / static_cast<float>(comboFrame[currentCombo].allowNextComboFrame + 1);
        decreaseSpeed *= dt;

        if (vel.x != 0.0f) {
            if (isFacingRight) {
                this->getPhysicsComponent()->addVelocity(glm::vec2(-decreaseSpeed, 0.0f));
            }
            else {
                this->getPhysicsComponent()->addVelocity(glm::vec2(decreaseSpeed, 0.0f));
            }
        }


        Animation::State currentState = this->getAnimationComponent()->getCurrentAnimationState();
        int currentFrame = currentState.currentFrame;

        if (currentFrame < comboFrame[currentCombo].startAttackFrame + 1) {
            return;
        }

        if (currentFrame == comboFrame[currentCombo].startAttackFrame + 1) {
            startAttack();
            return;
        }

        if (currentFrame == comboFrame[currentCombo].allowNextComboFrame + 1) {
            endAttack();
            vel = this->getPhysicsComponent()->getVelocity();
            this->getPhysicsComponent()->setVelocity(glm::vec2(0.0f, vel.y));

            attackCooldownRemaining = currentCombo == PlayerCombo::THIRD ? PlayerStat::LAST_COMBO_COOLDOWN : PlayerStat::ATTACK_COOLDOWN;
            timeToResetComboRemaining = currentCombo == PlayerCombo::THIRD ? PlayerStat::LAST_COMBO_COOLDOWN : PlayerStat::TIME_TO_RESET_COMBO;
            isAttacking = false;
            return;
        }

        timeToResetComboRemaining -= dt;

        if (moveDirection.x != 0.0f) {
            //moveDirection.x = 0.0f;
            canMove = true;
            isInAttackState = false;
            return;
        }

        if (timeToResetComboRemaining <= 0.0f) {
            currentCombo = PlayerCombo::NONE;
            canMove = true;
            isInAttackState = false;
        }

        return;
    }

    currentCombo = PlayerCombo::NONE;
    this->getAnimationComponent()->setState("Idle");

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
    if (isAttacking || isDodging) {
        return;
    }

    if (attackCooldownRemaining > 0.0f) {
        return;
    }

    isInAttackState = true;
    isAttacking = true;

    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    vel.x = PlayerStat::ATTACK_DASH_VELOCITY;
    vel.x *= isFacingRight ? 1.0f : -1.0f;
    this->getPhysicsComponent()->setVelocity(glm::vec2(vel));

    switch (currentCombo) {
        case PlayerCombo::NONE:
            currentCombo = PlayerCombo::FIRST;
            this->getAnimationComponent()->setState("Combo1");
            attackHitbox->setDamage(PlayerStat::COMBO_DAMAGE_1);
            break;

        case PlayerCombo::FIRST:
            currentCombo = PlayerCombo::SECOND;
            this->getAnimationComponent()->setState("Combo2");
            attackHitbox->setDamage(PlayerStat::COMBO_DAMAGE_2);
            break;

        case PlayerCombo::SECOND:
            currentCombo = PlayerCombo::THIRD;
            this->getAnimationComponent()->setState("Combo3");
            attackHitbox->setDamage(PlayerStat::COMBO_DAMAGE_3);
            break;

        case PlayerCombo::THIRD:
            currentCombo = PlayerCombo::FIRST;
            this->getAnimationComponent()->setState("Combo1");
            attackHitbox->setDamage(PlayerStat::COMBO_DAMAGE_1);
            break;
    }
}

void PlayerObject::startAttack() {
    attackHitbox->trigger(transform.getPosition());
}

void PlayerObject::endAttack() {
    attackHitbox->setActive(false);
}

void PlayerObject::onTriggerEnter(Collider* collider) {

}