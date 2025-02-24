// PlayerObject.cpp
#include "DamageCollider.h"
#include "PlayerObject.h"
#include "GameEngine.h"

//constexpr float ATTACK_DURATION = 0.2f; // Duration for the attack collider to be active
//constexpr float ATTACK_COOLDOWN = 0.5f; // Cooldown between attacks

PlayerObject::PlayerObject(PlayerInfo& playerInfo) : LivingEntity(playerInfo.name, playerInfo.health) {
    this->damage = playerInfo.damage;

    setTexture("../Resource/Texture/playertest3.png");
    initAnimation(8, 6);

    getAnimationComponent()->addState("Idle", 0, 0, 6, true);

    getAnimationComponent()->addState("Combo1", 1, 0, 6, false);
    getAnimationComponent()->addState("Combo2", 2, 0, 6, false);
    getAnimationComponent()->addState("Combo3", 3, 0, 6, false);

    getAnimationComponent()->addState("Charging", 4, 0, 6, false);
    getAnimationComponent()->addState("Charge1", 5, 0, 6, false);
    getAnimationComponent()->addState("Charge2", 6, 0, 6, false);

    getAnimationComponent()->addState("Parrying", 7, 0, 6, false);

    getTransform().setScale(1, 1);
    addColliderComponent();
    addPhysicsComponent();

    moveDirection = glm::vec3();
    isFacingRight = true;
    isCurrentAttackFacingRight = isFacingRight;

    dodgeTimeElapsed = 0.0f;
    dodgeCooldownLeft = 0.0f;
    canDodge = true;
    isDodging = false;
    canMove = true;
    canChangeFacingDirection = true;

    isAttacking = false;
    isInAttackState = false;
    attackCooldownRemaining = 0.0f;

    baseDamage[PlayerCombo::FIRST] = PlayerStat::COMBO_DAMAGE_1;
    baseDamage[PlayerCombo::SECOND] = PlayerStat::COMBO_DAMAGE_2;
    baseDamage[PlayerCombo::THIRD] = PlayerStat::COMBO_DAMAGE_3;

    comboFrame[PlayerCombo::FIRST] = AttackFrame(2, 3);
    comboFrame[PlayerCombo::SECOND] = AttackFrame(2, 3);
    comboFrame[PlayerCombo::THIRD] = AttackFrame(2, 3);

    currentCombo = PlayerCombo::NONE;
    timeBetweenLastAttack = 0.0f;

    damageMultiplier[PlayerHeavyCharge::LEVEL_1] = PlayerStat::CHARGE_DAMAGE_MULTIPLIER_1;
    damageMultiplier[PlayerHeavyCharge::LEVEL_2] = PlayerStat::CHARGE_DAMAGE_MULTIPLIER_2;

    heavyAttackCooldown[PlayerHeavyCharge::LEVEL_1] = PlayerStat::HEAVY_ATTACK_COOLDOWN_1;
    heavyAttackCooldown[PlayerHeavyCharge::LEVEL_2] = PlayerStat::HEAVY_ATTACK_COOLDOWN_2;

    heavyAttackFrame[PlayerHeavyCharge::LEVEL_1] = AttackFrame(2, 3);
    heavyAttackFrame[PlayerHeavyCharge::LEVEL_2] = AttackFrame(2, 3);
    currentHeavyCharge = PlayerHeavyCharge::LEVEL_0;
    isInHeavyAttack = false;

    parryFrame = AttackFrame(2, 3);
    isParrying = false;

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
    this->moveDirection.x += direction.x;

    if (this->moveDirection.x != 0.0f) {
        this->moveDirection.x /= abs(this->moveDirection.x);
    
        if (canChangeFacingDirection) {
            isFacingRight = moveDirection.x >= 0.0f ? true : false;
        }
    }
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

    if (isParrying) {
        canMove = false;

        vel = this->getPhysicsComponent()->getVelocity();
        this->getPhysicsComponent()->setVelocity(glm::vec2(0.0f, vel.y));

        Animation::State currentState = this->getAnimationComponent()->getCurrentAnimationState();
        int currentFrame = currentState.currentFrame;

        if (currentFrame < parryFrame.startAttackFrame + 1) {
            return;
        }

        if (currentFrame == parryFrame.startAttackFrame + 1) {
            startAttack();
            return;
        }

        if (currentFrame == parryFrame.allowNextComboFrame + 1) {
            endAttack();
            attackCooldownRemaining = PlayerStat::ATTACK_COOLDOWN;
            return;
        }

        if (!currentState.isPlaying) {
            canMove = true;
            isParrying = false;
            moveDirection.x = 0.0f;
        }
        return;
    }

    if (isInHeavyAttack) {
        canMove = false;
        Animation::State currentState = this->getAnimationComponent()->getCurrentAnimationState();

        if (isAttacking) {
            float decreaseSpeed = PlayerStat::ATTACK_DASH_VELOCITY / static_cast<float>(comboFrame[currentCombo].allowNextComboFrame + 1);
            decreaseSpeed *= dt;

            int currentFrame = currentState.currentFrame;

            if (currentFrame < heavyAttackFrame[currentHeavyCharge].startAttackFrame + 1) {
                return;
            }

            if (currentFrame == heavyAttackFrame[currentHeavyCharge].startAttackFrame + 1) {
                startAttack();
                return;
            }

            if (currentFrame == heavyAttackFrame[currentHeavyCharge].allowNextComboFrame + 1) {
                endAttack();
                vel = this->getPhysicsComponent()->getVelocity();
                this->getPhysicsComponent()->setVelocity(glm::vec2(0.0f, vel.y));
                attackCooldownRemaining = heavyAttackCooldown[currentHeavyCharge];
                return;
            }

            if (attackCooldownRemaining <= 0.0f) {
                isAttacking = false;
                isInHeavyAttack = false;
                isInAttackState = false;
                canMove = true;
                currentCombo = PlayerCombo::NONE;
                this->getAnimationComponent()->setState("Idle");
                return;
            }

            return;
        }

        this->getPhysicsComponent()->setVelocity(glm::vec2(0.0f, vel.y));

        if (!currentState.isPlaying) {
            currentHeavyCharge = PlayerHeavyCharge::LEVEL_2;
        }

        return;
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
        timeBetweenLastAttack += dt;

        if (moveDirection.x != 0.0f && timeBetweenLastAttack >= PlayerStat::AFTER_ATTACK_MOVE_DELAY_TIME) {
            moveDirection.x = 0.0f;
            this->getAnimationComponent()->setState("Idle");
            canMove = true;
            isInAttackState = false;

            return;
        }

        if (timeToResetComboRemaining <= 0.0f) {
            this->getAnimationComponent()->setState("Idle");
            canMove = true;
            currentCombo = PlayerCombo::NONE;
            isInAttackState = false;
        }

        return;
    }

    currentCombo = PlayerCombo::NONE;
    canChangeFacingDirection = true;
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
    if (canMove) {
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
    }
    
    moveDirection.x = 0.0f; // Reset move direction for next frame
}

void PlayerObject::normalAttack() {
    if (isAttacking || isDodging) {
        return;
    }

    if (attackCooldownRemaining > 0.0f) {
        return;
    }

    isInAttackState = true;
    isAttacking = true;
    canChangeFacingDirection = false;

    if (moveDirection.x != 0.0f) {
        isFacingRight = moveDirection.x >= 0.0f ? true : false;
    }

    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    vel.x = PlayerStat::ATTACK_DASH_VELOCITY;
    vel.x *= isFacingRight ? 1.0f : -1.0f;
    this->getPhysicsComponent()->setVelocity(glm::vec2(vel));

    timeBetweenLastAttack = 0.0f;

    switch (currentCombo) {
        case PlayerCombo::NONE:
            currentCombo = PlayerCombo::FIRST;
            this->getAnimationComponent()->setState("Combo1");
            break;

        case PlayerCombo::FIRST:
            currentCombo = PlayerCombo::SECOND;
            this->getAnimationComponent()->setState("Combo2");
            break;

        case PlayerCombo::SECOND:
            currentCombo = PlayerCombo::THIRD;
            this->getAnimationComponent()->setState("Combo3");
            break;

        case PlayerCombo::THIRD:
            currentCombo = PlayerCombo::FIRST;
            this->getAnimationComponent()->setState("Combo1");
            break;
    }

    attackHitbox->setDamage(baseDamage[currentCombo]);
}

void PlayerObject::heavyAttack(float duration) {
    isAttacking = true;
    canChangeFacingDirection = false;

    if (moveDirection.x != 0.0f) {
        isFacingRight = moveDirection.x >= 0.0f ? true : false;
    }

    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    vel.x = PlayerStat::ATTACK_DASH_VELOCITY;
    vel.x *= isFacingRight ? 1.0f : -1.0f;
    this->getPhysicsComponent()->setVelocity(glm::vec2(vel));

    timeBetweenLastAttack = 0.0f;
    
    switch (currentHeavyCharge) {
        case PlayerHeavyCharge::LEVEL_1:
            this->getAnimationComponent()->setState("Charge1");
            break;

        case PlayerHeavyCharge::LEVEL_2:
            this->getAnimationComponent()->setState("Charge2");
            break;
    }

    attackHitbox->setDamage(baseDamage[currentCombo] * damageMultiplier[currentHeavyCharge]);
}

void PlayerObject::parryAttack() {
    if (isParrying || isDodging) {
        return;
    }

    if (attackCooldownRemaining > 0.0f) {
        return;
    }

    canChangeFacingDirection = false;

    if (moveDirection.x != 0.0f) {
        isFacingRight = moveDirection.x >= 0.0f ? true : false;
    }

    this->getAnimationComponent()->setState("Parrying");
    isParrying = true;

    attackHitbox->setDamage(0);
}

void PlayerObject::startAttack() {
    attackHitbox->trigger(transform.getPosition());
}

void PlayerObject::endAttack() {
    attackHitbox->setActive(false);
}

void PlayerObject::startHeavyAttack() {
    if (isAttacking || isDodging) {
        return;
    }

    if (attackCooldownRemaining > 0.0f) {
        return;
    }

    isInHeavyAttack = true;
    this->getAnimationComponent()->setState("Charging");
    currentHeavyCharge = PlayerHeavyCharge::LEVEL_1;

    if (currentCombo == PlayerCombo::NONE) {
        currentCombo = PlayerCombo::FIRST;
    }
}

bool PlayerObject::getCanMove() const {
    return canMove;
}

bool PlayerObject::getIsParrying() const {
    return isParrying;
}

void PlayerObject::onTriggerEnter(Collider* collider) {

}