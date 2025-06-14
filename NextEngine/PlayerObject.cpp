// PlayerObject.cpp
#include "DamageCollider.h"
#include "HitScanDamage.h"
#include "PlayerObject.h"
#include "GameEngine.h"

//constexpr float ATTACK_DURATION = 0.2f; // Duration for the attack collider to be active
//constexpr float ATTACK_COOLDOWN = 0.5f; // Cooldown between attacks

PlayerObject::PlayerObject() : LivingEntity("Player", PlayerStat::MAX_HEALTH) {
    //this->damage = playerInfo.damage;

    noWeaponSprite = setTexture("../Resource/Texture/MACAOWALK.png");
    normalSprite = setTexture("../Resource/Texture/MC.png");
    initAnimation(18, 9);
    this->getAnimationComponent()->setTexture(normalSprite);

    getAnimationComponent()->addState("Idle", 0, 0, 6, true);
    getAnimationComponent()->addState("Walking", 1, 0, 6, true);
    getAnimationComponent()->addState("Jumping", 2, 0, 4, false, 0.2f);

    getAnimationComponent()->addState("Dodging", 3, 0, 5, false, PlayerStat::DODGE_DURATION / 6.0f);

    getAnimationComponent()->addState("Combo1", 4, 0, 5, false, PlayerStat::ATTACK_ANIMATION_TIME_PER_FRAME);
    getAnimationComponent()->addState("Combo2", 5, 0, 5, false, PlayerStat::ATTACK_ANIMATION_TIME_PER_FRAME);
    getAnimationComponent()->addState("Combo3", 6, 0, 5, false, PlayerStat::ATTACK_ANIMATION_TIME_PER_FRAME);

    getAnimationComponent()->addState("Charging", 7, 0, 8, false, PlayerStat::CHARGE_ANIMATION_TIME_PER_FRAME);
    getAnimationComponent()->addState("MaxCharging", 8, 0, 3, true, PlayerStat::CHARGE_ANIMATION_TIME_PER_FRAME);
    getAnimationComponent()->addState("Charge1", 9, 0, 4, false, PlayerStat::ATTACK_ANIMATION_TIME_PER_FRAME);
    getAnimationComponent()->addState("Charge2", 10, 0, 4, false, PlayerStat::ATTACK_ANIMATION_TIME_PER_FRAME);

    getAnimationComponent()->addState("Parrying", 15, 0, 8, false, PlayerStat::PARRY_ANIMATION_TIME_PER_FRAME);

    getAnimationComponent()->addState("GunCharge1", 12, 0, 6, true, PlayerStat::GUN_CHARGE_ANIMATION_TIME_PER_FRAME);
    getAnimationComponent()->addState("GunCharge2", 13, 0, 6, true, PlayerStat::GUN_CHARGE_ANIMATION_TIME_PER_FRAME);
    getAnimationComponent()->addState("GunCharge3", 14, 0, 6, true, PlayerStat::GUN_CHARGE_ANIMATION_TIME_PER_FRAME);
    getAnimationComponent()->addState("GunShoot", 11, 1, 3, false, PlayerStat::GUN_SHOT_ANIMATION_TIME_PER_FRAME);

    getAnimationComponent()->addState("Healing", 16, 0, 7, false);

    getAnimationComponent()->addState("Dead", 17, 0, 6, false);

    //getTransform().setScale(1, 1);
    addColliderComponent();
    addPhysicsComponent();

    moveDirection = glm::vec3();
    isFacingRight = true;
    isCurrentAttackFacingRight = isFacingRight;

    dodgeTimeElapsed = 0.0f;
    dodgeCooldownLeft = 0.0f;
    canDodge = true;
    isDodging = false;
    isJumping = false;
    midAirFrameNum = 3;
    groundedFrameNum = 4;
    canMove = true;
    canChangeFacingDirection = true;

    isDead = false;

    baseDamage[PlayerCombo::FIRST] = PlayerStat::COMBO_DAMAGE_1;
    baseDamage[PlayerCombo::SECOND] = PlayerStat::COMBO_DAMAGE_2;
    baseDamage[PlayerCombo::THIRD] = PlayerStat::COMBO_DAMAGE_3;

    comboFrame[PlayerCombo::FIRST] = AttackFrame(1, 2);
    comboFrame[PlayerCombo::SECOND] = AttackFrame(1, 2);
    comboFrame[PlayerCombo::THIRD] = AttackFrame(1, 2);

    damageMultiplier[PlayerHeavyCharge::LEVEL_1] = PlayerStat::CHARGE_DAMAGE_MULTIPLIER_1;
    damageMultiplier[PlayerHeavyCharge::LEVEL_2] = PlayerStat::CHARGE_DAMAGE_MULTIPLIER_2;

    heavyAttackCooldown[PlayerHeavyCharge::LEVEL_1] = PlayerStat::HEAVY_ATTACK_COOLDOWN_1;
    heavyAttackCooldown[PlayerHeavyCharge::LEVEL_2] = PlayerStat::HEAVY_ATTACK_COOLDOWN_2;

    heavyAttackFrame[PlayerHeavyCharge::LEVEL_1] = AttackFrame(0, 1);
    heavyAttackFrame[PlayerHeavyCharge::LEVEL_2] = AttackFrame(0, 1);
    currentHeavyCharge = PlayerHeavyCharge::LEVEL_0;

    baseRangeDamage = PlayerStat::RANGE_DAMAGE;
    rangeAttackCooldown[PlayerRangeCharge::CHARGE_1] = PlayerStat::RANGE_ATTACK_COOLDOWN_1;
    rangeAttackCooldown[PlayerRangeCharge::CHARGE_2] = PlayerStat::RANGE_ATTACK_COOLDOWN_2;
    rangeAttackCooldown[PlayerRangeCharge::CHARGE_3] = PlayerStat::RANGE_ATTACK_COOLDOWN_3;

    rangeDamageMultiplier[PlayerRangeCharge::CHARGE_1] = PlayerStat::NUM_OF_BULLET_PER_SHOT_1;
    rangeDamageMultiplier[PlayerRangeCharge::CHARGE_2] = PlayerStat::NUM_OF_BULLET_PER_SHOT_2;
    rangeDamageMultiplier[PlayerRangeCharge::CHARGE_3] = PlayerStat::NUM_OF_BULLET_PER_SHOT_3;

    rangeChargeDuration[PlayerRangeCharge::CHARGE_1] = PlayerStat::RANGE_CHARGE_DURATION_1;
    rangeChargeDuration[PlayerRangeCharge::CHARGE_2] = PlayerStat::RANGE_CHARGE_DURATION_2;
    rangeChargeDuration[PlayerRangeCharge::CHARGE_3] = PlayerStat::RANGE_CHARGE_DURATION_3;

    parryFrame = AttackFrame(1, 2);

    resetAttack();

    attackHitbox = nullptr;

    iFrameTimeRemaining = 0.0f;

    stamina = PlayerStat::MAX_STAMINA;
    staminaRechargeDelayTimer = 0.0f;
    staminaRechargeTimer = 0.0f;

    setMaxNumOfPotion(PlayerStat::MAX_HEALTH_POTION);

    resetNumOfBullet();
    resetHealing();
    healFrame = 4;

    emitter = new ParticleSystem();
}

PlayerObject::~PlayerObject() {
    if (attackHitbox != nullptr) {
        destroyObject(attackHitbox);
    }
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
    if (isDodging || !canMove || isJumping) {
        return;
    }

    if (stamina < PlayerStat::JUMP_STAMINA_CONSUMPTION) {
        return;
    }

    stamina -= PlayerStat::JUMP_STAMINA_CONSUMPTION;
    resetStaminaRechargeDelay();

    bool grounded = collider->getCollisionFlag() & COLLISION_DOWN;
    if (!grounded) {
        return;
    }

    glm::vec2 vel = this->physics->getVelocity();
    vel.y = PlayerStat::JUMP_VELOCITY;

    if (moveDirection.x != 0.0f) {
        vel.x = moveDirection.x * PlayerStat::MOVE_SPEED;
    }

    this->physics->setVelocity(vel);
    isJumping = true;
    this->getAnimationComponent()->setState("Jumping");
}

void PlayerObject::dodge() {
    if (!canDodge || !canMove || isJumping || isDodging) {
        return;
    }

    if (stamina < PlayerStat::DODGE_STAMINA_CONSUMPTION) {
        return;
    }

    stamina -= PlayerStat::DODGE_STAMINA_CONSUMPTION;
    resetStaminaRechargeDelay();

	GameEngine::getInstance()->getAudioEngine().playSoundEffectByName("Sound_Rolling.wav");
    isDodging = true;
    canChangeFacingDirection = false;

    dodgeTimeElapsed = 0.0f;
}

void PlayerObject::dodge(float xDirection) {
    if (!isDodging) {
        isFacingRight = xDirection > 0.0f;

    }
    dodge();
}

void PlayerObject::useHealthPotion() {
    if (isHealing || isAttacking || isParrying || isDodging || isJumping || isInAttackState || isInHeavyAttack || isInRangeAttack) {
        return;
    }

    if (currentNumOfPotion <= 0) {
        return;
    }

    if (this->getHealth() == this->getMaxHealth()) {
        return;
    }

    isHealing = true;
    healed = false;

    this->getAnimationComponent()->setState("Healing");
	GameEngine::getInstance()->getAudioEngine().playSoundEffectByName("Sound_Heal.wav");
}

void PlayerObject::start(list<DrawableObject*>& objectsList) {
    attackHitbox = new DamageCollider<EnemyObject>(this, 0, -1);
    //attackHitbox = new DamageCollider<EnemyObject>(this, damage, 2.5f);
    attackHitbox->setActive(false);
    attackHitbox->setFollowOwner(true);
    attackHitbox->setFollowOffset(glm::vec3(0.1f, 0.0f, 0));
    attackHitbox->getColliderComponent()->setWidth(1.8f);
    attackHitbox->addEmitter(objectsList);
    objectsList.emplace_back(attackHitbox);

    this->getTransform().setScale(3.1868f, 2.5f);
    this->getColliderComponent()->getTransform().translate(0.0f, -0.44f);
    this->getColliderComponent()->setDimension(0.25f, 0.65f);

    this->getAnimationComponent()->setState("Idle");
}

void PlayerObject::updateStat() {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    if (bulletRechargeTimer > 0.0f && currentNumOfBullets < PlayerStat::MAX_BULLET) {
        bulletRechargeTimer -= dt;
    }

    if (currentNumOfBullets < PlayerStat::MAX_BULLET && bulletRechargeTimer <= 0.0f) {
        bulletRechargeTimer = PlayerStat::BULLET_RECHARGE_TIMER;
        currentNumOfBullets++;
    }

    if (staminaRechargeDelayTimer > 0.0f) {
        staminaRechargeDelayTimer -= dt;
    }

    if (staminaRechargeTimer > 0.0f) {
        staminaRechargeTimer -= dt;
    }

    if (staminaRechargeDelayTimer <= 0.0f) {
        if (staminaRechargeTimer <= 0.0f && stamina < PlayerStat::MAX_STAMINA) {
            staminaRechargeTimer = PlayerStat::STAMINA_RECHARGE_TIMER;

            stamina += PlayerStat::STAMINA_RECHARGE_AMOUNT;
            if (stamina > PlayerStat::MAX_STAMINA) {
                stamina = PlayerStat::MAX_STAMINA;
            }
        }
    }

    //if (potionRechargeTimer > 0.0f && currentNumOfPotion < PlayerStat::MAX_HEALTH_POTION) {
    //    potionRechargeTimer -= dt;
    //}

    //if (currentNumOfPotion < PlayerStat::MAX_HEALTH_POTION && potionRechargeTimer <= 0.0f) {
    //    potionRechargeTimer = PlayerStat::POTION_RECHARGE_TIMER;
    //    currentNumOfPotion++;
    //}
    //cout << "potion left: " << currentNumOfPotion << endl;
}

void PlayerObject::updateBehavior(list<DrawableObject*>& objectsList) {
    /// Using particle emitter ///
    emitter->update(objectsList);
    ///

    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    glm::vec2 vel = this->physics->getVelocity();

    updateStat();

    if (isDead) {
        handleDead();
        return;
    }

    if (iFrameTimeRemaining > 0.0f) {
        this->setCanTakeDamage(false);
        iFrameTimeRemaining -= dt;
    }
    else {
        this->setCanTakeDamage(true);
    }

    if (this->isInKnockback) {
        //+ add knockback animation
        moveDirection.x = 0.0f;
        return;
    }

    if (flinchTimeRemaining > 0.0f) {
        handleFlinch();
        return;
    }

    if (attackCooldownRemaining > 0.0f) {
        attackCooldownRemaining -= dt;
    }

    if (rangeAttackCooldownRemaining > 0.0f) {
        rangeAttackCooldownRemaining -= dt;
    }

    if (isHealing) {
        handleHealing();
        return;
    }

    if (isParrying) {
        handleParryAttack();
        return;
    }

    if (isInRangeAttack) {
        handleRangeAttack();
        return;
    }

    if (isInHeavyAttack) {
        handleHeavyAttack();
        return;
    }

    if (isInAttackState) {
        handleNormalAttack();
        return;
    }

    currentCombo = PlayerCombo::NONE;
    canChangeFacingDirection = true;

    //this->getAnimationComponent()->setState("Idle");

    if (isJumping) {
        handleJumpMovement();
        return;
    }

    if (isDodging) {
        handleDodging();
        return;
    }
    else {
        dodgeCooldownLeft -= dt;

        if (dodgeCooldownLeft <= 0) {
            dodgeCooldownLeft = 0.0f;
            canDodge = true;
        }
    }

    if (canMove) {
        handleMovement();
    }

    moveDirection.x = 0.0f; // Reset move direction for next frame

}

void PlayerObject::normalAttack() {
    if (isAttacking || isParrying || isDodging || isJumping || isHealing || isInRangeAttack) {
        return;
    }

    if (attackCooldownRemaining > 0.0f) {
        return;
    }

    if (stamina < PlayerStat::MELEE_STAMINA_CONSUMPTION) {
        return;
    }

    stamina -= PlayerStat::MELEE_STAMINA_CONSUMPTION;
    resetStaminaRechargeDelay();

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
        attackHitbox->setDamageTag("FinalNormalAttack");
        break;

    case PlayerCombo::THIRD:
        currentCombo = PlayerCombo::FIRST;
        this->getAnimationComponent()->setState("Combo1");
        break;
    }

    attackHitbox->setDamage(baseDamage[currentCombo]);
}

void PlayerObject::heavyAttack() {
    if (stamina < PlayerStat::HEAVY1_STAMINA_CONSUMPTION) {
        return;
    }

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
        attackHitbox->setDamageTag("HeavyAttack1");
        stamina -= PlayerStat::HEAVY1_STAMINA_CONSUMPTION;
        //cout << "lvl 1" << endl;
        break;

    case PlayerHeavyCharge::LEVEL_2:
        this->getAnimationComponent()->setState("Charge2");
        attackHitbox->setDamageTag("HeavyAttack2");
        stamina -= PlayerStat::HEAVY2_STAMINA_CONSUMPTION;
        //cout << "lvl 2" << endl;
        break;
    }
    resetStaminaRechargeDelay();

    attackHitbox->setDamage(baseDamage[currentCombo] * damageMultiplier[currentHeavyCharge]);

}

void PlayerObject::parryAttack() {
    if (isParrying || isDodging || isAttacking || isInHeavyAttack || isJumping || isInRangeAttack || isHealing) {
        return;
    }

    if (attackCooldownRemaining > 0.0f) {
        return;
    }

    if (stamina < PlayerStat::PARRY_STAMINA_CONSUMPTION) {
        return;
    }

    stamina -= PlayerStat::PARRY_STAMINA_CONSUMPTION;
    resetStaminaRechargeDelay();

    isAttacking = false;
    isInAttackState = false;
    currentCombo = PlayerCombo::NONE;

    isFacingRight = (moveDirection.x > 0.0f) ? true : ((moveDirection.x < 0.0f) ? false : isFacingRight);

    canChangeFacingDirection = false;

    if (moveDirection.x != 0.0f) {
        isFacingRight = moveDirection.x >= 0.0f ? true : false;
    }

    this->getAnimationComponent()->setState("Parrying");
    isParrying = true;
    successfulParry = false;

    attackHitbox->setDamage(0);
}

void PlayerObject::rangeAttack(std::list<DrawableObject*>& objectsList) {
    if (currentRangeCharge == PlayerRangeCharge::CHARGE_0) {
        return;
    }

    isAttacking = true;
    canChangeFacingDirection = false;
    GameEngine::getInstance()->playSoundEffect("Sound_Gun_Shoot.wav");

    if (moveDirection.x != 0.0f) {
        isFacingRight = moveDirection.x >= 0.0f ? true : false;
        moveDirection.x = 0.0f;
    }

    timeBetweenLastAttack = 0.0f;

    glm::vec3 direction = isFacingRight ? glm::vec3(1, 0, 0) : glm::vec3(-1, 0, 0);

    HitScanDamage<EnemyObject>* hitscan = new HitScanDamage<EnemyObject>(
        glm::vec3(),
        direction,
        PlayerStat::RANGE_ATTACK_DISTANCE,
        baseRangeDamage,
        rangeDamageMultiplier[currentRangeCharge],
        PlayerStat::RANGE_ATTACK_LIFESPAN
    );
    ///temporary pos, new adjustment later
    glm::vec3 currentPos = this->getTransform().getPosition();
    currentPos.y -= 0.25f;
    hitscan->getTransform().setPosition(currentPos);

    this->getAnimationComponent()->setState("GunShoot");

    objectsList.emplace_back(hitscan);
    rangeAttackCooldownRemaining = rangeAttackCooldown[currentRangeCharge];
    currentNumOfBullets -= rangeDamageMultiplier[currentRangeCharge];
    currentRangeCharge = PlayerRangeCharge::CHARGE_0;
    rangeHeldDuration = 0.0f;

    stamina -= PlayerStat::RANGE_STAMINA_CONSUMPTION;
    resetStaminaRechargeDelay();
}

void PlayerObject::resetAttack() {
    isAttacking = false;
    isInAttackState = false;
    attackCooldownRemaining = 0.0f;
    currentCombo = PlayerCombo::NONE;
    timeBetweenLastAttack = 0.0f;
    timeToResetComboRemaining = 0.0f;

    currentHeavyCharge = PlayerHeavyCharge::LEVEL_0;
    isInHeavyAttack = false;

    currentRangeCharge = PlayerRangeCharge::CHARGE_0;
    rangeAttackCooldownRemaining = 0.0f;
    rangeHeldDuration = 0.0f;

    isParrying = false;
    successfulParry = false;
    isInRangeAttack = false;
}

void PlayerObject::flinch(float duration) {
    if (isJumping || isDodging) {
        return;
    }

    resetHealing();

    this->flinchTimeRemaining = duration;
    resetAttack();
    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    vel.x = 0.0f;
    this->getPhysicsComponent()->setVelocity(vel);
    moveDirection.x = 0.0f;
    endMeleeAttack();

    canChangeFacingDirection = false;
    canMove = false;

    //+ start flinch animation
    this->getAnimationComponent()->setState("Idle");
}

void PlayerObject::startMeleeAttack() {
    attackHitbox->trigger(transform.getPosition());
}

void PlayerObject::endMeleeAttack() {
    attackHitbox->setActive(false);
    attackHitbox->setDamageTag("");
}

void PlayerObject::startHeavyAttack() {
    if (isAttacking || isParrying || isDodging || isJumping || isHealing || isInRangeAttack) {
        return;
    }

    if (attackCooldownRemaining > 0.0f) {
        return;
    }

    if (stamina < PlayerStat::HEAVY1_STAMINA_CONSUMPTION) {
        return;
    }

    isInHeavyAttack = true;

    if (currentCombo == PlayerCombo::NONE) {
        currentCombo = PlayerCombo::FIRST;
    }

    Animation::State currentState = this->getAnimationComponent()->getCurrentAnimationState();
    if (currentState.name != "MaxCharging") {
        this->getAnimationComponent()->setState("Charging");
        currentHeavyCharge = PlayerHeavyCharge::LEVEL_1;
    }

    if (!currentState.isPlaying && currentState.name == "Charging") {
        currentHeavyCharge = PlayerHeavyCharge::LEVEL_2;
        this->getAnimationComponent()->setState("MaxCharging");
        return;
    }
}

void PlayerObject::startRangeAttack(float dt) {
    if (isAttacking || isParrying || isDodging || isJumping || isInAttackState || isInHeavyAttack || isHealing) {
        return;
    }

    if (rangeAttackCooldownRemaining > 0.0f) {
        return;
    }

    if (stamina < PlayerStat::RANGE_STAMINA_CONSUMPTION || currentNumOfBullets < PlayerStat::NUM_OF_BULLET_PER_SHOT_1) {
        return;
    }

    isInRangeAttack = true;

    rangeHeldDuration += dt;

    if (rangeHeldDuration > rangeChargeDuration[PlayerRangeCharge::CHARGE_3] && currentNumOfBullets >= rangeChargeDuration[PlayerRangeCharge::CHARGE_3]) {
        currentRangeCharge = PlayerRangeCharge::CHARGE_3;
        this->getAnimationComponent()->setState("GunCharge3");
		GameEngine::getInstance()->getAudioEngine().playSoundEffectByName("Sound_Gun_Charge.wav");
        return;
    }

    if (rangeHeldDuration > rangeChargeDuration[PlayerRangeCharge::CHARGE_2] && currentNumOfBullets >= rangeChargeDuration[PlayerRangeCharge::CHARGE_2]) {
        currentRangeCharge = PlayerRangeCharge::CHARGE_2;
        this->getAnimationComponent()->setState("GunCharge2");
        return;
    }

    currentRangeCharge = PlayerRangeCharge::CHARGE_1;
    this->getAnimationComponent()->setState("GunCharge1");
}

void PlayerObject::handleDodging() {
    this->getAnimationComponent()->setState("Dodging");

    canChangeFacingDirection = false;

    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    canDodge = false;
    setCanTakeDamage(false);

    float xDirection = isFacingRight ? 1.0f : -1.0f;

    glm::vec2 dodgeVelocity = glm::vec2(abs(PlayerStat::DODGE_VELOCITY) * xDirection, vel.y);
    this->physics->setVelocity(dodgeVelocity);

    dodgeTimeElapsed += dt;

    if (dodgeTimeElapsed >= PlayerStat::DODGE_DURATION) {
        isDodging = false;
        canChangeFacingDirection = true;
        dodgeCooldownLeft = PlayerStat::DODGE_COOLDOWN;
        setCanTakeDamage(true);
        iFrameTimeRemaining = 0.0f;
    }
}

void PlayerObject::handleMovement() {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();

    this->physics->addVelocity(glm::vec2(moveDirection.x * PlayerStat::ACCEL_SPEED * dt, 0.0f));
    vel = this->physics->getVelocity();
    if (abs(vel.x) > PlayerStat::MOVE_SPEED) {
        vel.x = PlayerStat::MOVE_SPEED * moveDirection.x;
        this->physics->setVelocity(vel);
    }

    if (vel.x == 0.0f) {
        Animation::State currentState = this->getAnimationComponent()->getCurrentAnimationState();
        if (currentState.name != "Parrying") {
            this->getAnimationComponent()->setState("Idle");
        }
        else if (currentState.name == "Parrying" && !currentState.isPlaying) {
            this->getAnimationComponent()->setState("Idle");
        }
    }
    else {
        this->getAnimationComponent()->setState("Walking");
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

void PlayerObject::handleJumpMovement() {
    bool grounded = this->getColliderComponent()->getCollisionFlag() & COLLISION_DOWN;
    Animation::State& currentState = this->getAnimationComponent()->getCurrentAnimationStateRef();

    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    vel.x += moveDirection.x * PlayerStat::AIR_ACCEL * dt;

    if (abs(vel.x) > PlayerStat::MOVE_SPEED) {
        vel.x = (abs(vel.x) / vel.x) * PlayerStat::MOVE_SPEED;
    }

    this->getPhysicsComponent()->setVelocity(vel);

    moveDirection.x = 0.0f;

    if (currentState.currentFrame < midAirFrameNum && !grounded) {
        return;
    }

    if (grounded) {
        if (currentState.currentFrame < groundedFrameNum) {
            currentState.currentFrame = groundedFrameNum;
        }

        if (!currentState.isPlaying) {
            isJumping = false;
        }

        return;
    }

    currentState.currentFrame = midAirFrameNum;
}

void PlayerObject::handleHealing() {
    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    vel.x = 0.0f;
    this->getPhysicsComponent()->setVelocity(vel);

    Animation::State animState = this->getAnimationComponent()->getCurrentAnimationState();
    if (animState.currentFrame == healFrame && !healed) {
        this->heal(PlayerStat::HEAL_AMOUNT);
        currentNumOfPotion--;
        resetAttack();
        endMeleeAttack();
        healed = true;
        return;
    }

    if (!animState.isPlaying) {
        isHealing = false;
        this->getAnimationComponent()->setState("Idle");
        return;
    }
}

void PlayerObject::handleNormalAttack() {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
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
    if (currentFrame == 0) {
        switch (currentCombo)
        {
		case PlayerCombo::FIRST:
			GameEngine::getInstance()->playSoundEffect("Sound_Attack_Light_1.wav");
			break;
		case PlayerCombo::SECOND:
			GameEngine::getInstance()->playSoundEffect("Sound_Attack_Light_2.wav");
			break;
		case PlayerCombo::THIRD:
			GameEngine::getInstance()->playSoundEffect("Sound_Attack_Light_3.wav");
			break;
        }
    }
    if (currentFrame < comboFrame[currentCombo].startAttackFrame + 1) {
        return;
    }

    if (currentFrame == comboFrame[currentCombo].startAttackFrame + 1) {
        startMeleeAttack();

        

        return;
    }

    if (currentFrame == comboFrame[currentCombo].allowNextComboFrame + 1) {
        endMeleeAttack();
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
}
void PlayerObject::handleHeavyAttack() {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
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
            startMeleeAttack();
            return;
        }

        if (currentFrame == heavyAttackFrame[currentHeavyCharge].allowNextComboFrame + 1) {
            endMeleeAttack();
            vel = this->getPhysicsComponent()->getVelocity();
            this->getPhysicsComponent()->setVelocity(glm::vec2(0.0f, vel.y));
            attackCooldownRemaining = heavyAttackCooldown[currentHeavyCharge];
			GameEngine::getInstance()->playSoundEffect("Sound_Attack_Heavy.wav");
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
}

void PlayerObject::handleRangeAttack() {
    canMove = false;

    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    this->getPhysicsComponent()->setVelocity(glm::vec2(0.0f, vel.y));
    if (isAttacking) {
        if (!getAnimationComponent()->getCurrentAnimationState().isPlaying) {
            isInRangeAttack = false;
            isAttacking = false;
            canMove = true;
            getAnimationComponent()->setState("Idle");
        }
        return;
    }
}

void PlayerObject::handleParryAttack() {
    canMove = false;
    currentCombo = PlayerCombo::NONE;

    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    this->getPhysicsComponent()->setVelocity(glm::vec2(0.0f, vel.y));

    Animation::State currentState = this->getAnimationComponent()->getCurrentAnimationState();
    int currentFrame = currentState.currentFrame;

    if (currentFrame == parryFrame.startAttackFrame) {
        return;
    }

    if (currentFrame < parryFrame.startAttackFrame + 1) {
        return;
    }

    if (currentFrame == parryFrame.startAttackFrame + 1) {
        startMeleeAttack();

        //this->setCanTakeDamage(false);

        return;
    }

    if (currentFrame == parryFrame.allowNextComboFrame + 1 || (successfulParry)) {
        endMeleeAttack();
        
        //this->setCanTakeDamage(true);
        attackCooldownRemaining = successfulParry ? 0.0f : PlayerStat::ATTACK_COOLDOWN;

        if (successfulParry) {
            successfulParry = false;

            canMove = true;
            isParrying = false;
            moveDirection.x = 0.0f;
        }

        return;
    }

    if (!currentState.isPlaying) {
        canMove = true;
        isParrying = false;
        moveDirection.x = 0.0f;
    }
}

void PlayerObject::handleFlinch() {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    flinchTimeRemaining -= dt;
    canMove = false;

    resetAttack();
    endMeleeAttack();

    if (flinchTimeRemaining <= 0.0f) {
        //+ reset to idle animation
        this->getAnimationComponent()->setState("Idle");
        canMove = true;
        canChangeFacingDirection = true;
        return;
    }
}

void PlayerObject::handleDead() {
    this->getAnimationComponent()->setState("Dead");

    glm::vec2 vel = this->getPhysicsComponent()->getVelocity();
    vel.x = 0.0f;
    this->getPhysicsComponent()->setVelocity(vel);
    moveDirection.x = 0.0f;

    if (!deathSoundPlayed) {
        deathSoundPlayed = true;
        GameEngine::getInstance()->playSoundEffect("Sound_Died.wav");
    }
}

bool PlayerObject::getCanMove() const {
    return canMove;
}

bool PlayerObject::getIsParrying() const {
    return isParrying;
}

DamageCollider<EnemyObject>* PlayerObject::getDamageCollider() const {
    return attackHitbox;
}

void PlayerObject::signalSuccessfulParry() {
	GameEngine::getInstance()->playSoundEffect("Enemy_Getting Parry.wav");
    successfulParry = true;
    stamina += PlayerStat::STAMINA_GAIN_FROM_PARRY;
    if (stamina > PlayerStat::MAX_STAMINA) {
        stamina = PlayerStat::MAX_STAMINA;
    }
}

void PlayerObject::takeDamage(int damage) {
    if (!this->getCanTakeDamage()) {
        return;
    }

    if (this->getHealth() - damage <= 0 && !isDead) {
        this->LivingEntity::takeDamage(damage);
        this->setHealth(0);
        this->setCanTakeDamage(false);
        isDead = true;

        this->getAnimationComponent()->setState("Dead");

        return;
    }

    this->LivingEntity::takeDamage(damage);
    iFrameTimeRemaining = PlayerStat::INVINCIBLE_DURATION_AFTER_TAKING_DAMAGE;
	GameEngine::getInstance()->playSoundEffect("Sound_GettingHit.wav");
}

void PlayerObject::takeDamage(int damage, bool ignoreCanTakeDamage) {
    if (this->getHealth() - damage <= 0 && !isDead) {
        this->LivingEntity::takeDamage(damage);
        this->setHealth(0);
        this->setCanTakeDamage(false);
        isDead = true;

        this->getAnimationComponent()->setState("Dead");

        return;
    }

    this->LivingEntity::takeDamage(damage, true);
    iFrameTimeRemaining = PlayerStat::INVINCIBLE_DURATION_AFTER_TAKING_DAMAGE;
    GameEngine::getInstance()->playSoundEffect("Sound_GettingHit.wav");
}

void PlayerObject::resetStaminaRechargeDelay() {
    staminaRechargeDelayTimer = PlayerStat::STAMINA_RECHARGE_DELAY;
}

void PlayerObject::resetNumOfBullet() {
    currentNumOfBullets = PlayerStat::MAX_BULLET;
    bulletRechargeTimer = PlayerStat::BULLET_RECHARGE_TIMER;
}

void PlayerObject::resetHealing() {
    isHealing = false;
    healed = false;
}

void PlayerObject::setWieldWeaponSprite(bool value) {
    value ? this->getAnimationComponent()->setTexture(normalSprite) : this->getAnimationComponent()->setTexture(noWeaponSprite);
}

void PlayerObject::setMaxNumOfPotion(int n) {
    this->maxNumOfPotion = n;
    this->currentNumOfPotion = this->maxNumOfPotion;
}

int PlayerObject::getStamina() const {
    return stamina;
}

int PlayerObject::getCurrentNumOfBullet() const {
    return currentNumOfBullets;
}

float PlayerObject::getRangeAttackCooldownRemaining() const {
    return rangeAttackCooldownRemaining;
}

int PlayerObject::getCurrentNumOfPotion() const {
    return currentNumOfPotion;
}

float PlayerObject::getPotionRechargeTimer() const {
    return potionRechargeTimer;
}

bool PlayerObject::getIsDead() const {
    return this->isDead;
}