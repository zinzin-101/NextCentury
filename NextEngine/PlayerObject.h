#pragma once
#include "LivingEntity.h"
#include "PlayerInfo.h"
#include <list>

namespace PlayerStat {
    constexpr float DODGE_VELOCITY = 6.0f;
    constexpr float DODGE_COOLDOWN = 0.125f;
    constexpr float DODGE_DURATION = 0.45f;

    constexpr float TIME_TO_RESET_COMBO = 0.8f;
    constexpr float ATTACK_COOLDOWN = 0.05f;
    constexpr float LAST_COMBO_COOLDOWN = 0.4f;
    constexpr float ATTACK_DASH_VELOCITY = 0.2f;
    constexpr float AFTER_ATTACK_MOVE_DELAY_TIME = 0.3f;
    constexpr float DURATION_TO_START_HEAVY_ATTACK = 0.3f;
    constexpr float DURATION_TO_START_CHARGED_BULLET = 0.5f;

    constexpr float ATTACK_ANIMATION_TIME_PER_FRAME = 0.1f;
    constexpr float PARRY_ANIMATION_TIME_PER_FRAME = 0.1f;
    constexpr float CHARGE_ANIMATION_TIME_PER_FRAME = 0.1f;
    constexpr float GUN_CHARGE_ANIMATION_TIME_PER_FRAME = 0.1f;

    constexpr int COMBO_DAMAGE_1 = 10;
    constexpr int COMBO_DAMAGE_2 = 15;
    constexpr int COMBO_DAMAGE_3 = 15;
    
    constexpr float CHARGE_DAMAGE_MULTIPLIER_1 = 1.0f;
    constexpr float CHARGE_DAMAGE_MULTIPLIER_2 = 2.0f;
    constexpr float HEAVY_ATTACK_COOLDOWN_1 = 0.5f;
    constexpr float HEAVY_ATTACK_COOLDOWN_2 = 0.8f;
    
    constexpr int RANGE_DAMAGE = 8;
    constexpr int NUM_OF_BULLET_PER_SHOT_1 = 1;
    constexpr int NUM_OF_BULLET_PER_SHOT_2 = 2;
    constexpr int NUM_OF_BULLET_PER_SHOT_3 = 3;
    constexpr float RANGE_CHARGE_DURATION_1 = 0.0f;
    constexpr float RANGE_CHARGE_DURATION_2 = 0.5f;
    constexpr float RANGE_CHARGE_DURATION_3 = 1.0f;
    constexpr float RANGE_ATTACK_COOLDOWN_1 = 0.5f;
    constexpr float RANGE_ATTACK_COOLDOWN_2 = 0.75f;
    constexpr float RANGE_ATTACK_COOLDOWN_3 = 1.25f;
    constexpr float RANGE_ATTACK_DISTANCE = 5.0f;
    constexpr float RANGE_ATTACK_LIFESPAN = 0.5f;

    constexpr float MOVE_SPEED = 3.0f;
    constexpr float ACCEL_SPEED = 2000.0f;
    constexpr float DECEL_SPEED = 10.0f;
    constexpr float AIR_ACCEL = 6.0f;

    constexpr float JUMP_VELOCITY = 20.0f;

    constexpr float INVINCIBLE_DURATION_AFTER_TAKING_DAMAGE = 0.4f;

    constexpr float FLINCH_TIME = 0.2f;

    //+ placeholder player health
    constexpr int MAX_HEALTH = 100;
    constexpr int MAX_STAMINA = 100;
    constexpr int MAX_BULLET = 3;

    constexpr float STAMINA_RECHARGE_DELAY = 1.25f;
    constexpr float STAMINA_RECHARGE_TIMER = 0.1f;
    constexpr int STAMINA_RECHARGE_AMOUNT = 5.0f;
    constexpr int STAMINA_GAIN_FROM_PARRY = MAX_STAMINA / 2;

    constexpr float BULLET_RECHARGE_TIMER = 2.0f;

    constexpr int JUMP_STAMINA_CONSUMPTION = 10;
    constexpr int DODGE_STAMINA_CONSUMPTION = 15;
    constexpr int MELEE_STAMINA_CONSUMPTION = 10;
    constexpr int HEAVY1_STAMINA_CONSUMPTION = 35;
    constexpr int HEAVY2_STAMINA_CONSUMPTION = 50;
    constexpr int PARRY_STAMINA_CONSUMPTION = 20;
    constexpr int RANGE_STAMINA_CONSUMPTION = 15;

    constexpr int MAX_HEALTH_POTION = 3;
    constexpr int HEAL_AMOUNT = MAX_HEALTH / 2;
    constexpr float POTION_RECHARGE_TIMER = 5.0f;
}

class EnemyObject;

template <class T>
class DamageCollider;

class PlayerObject : public LivingEntity {
    private:
        enum PlayerCombo {
            FIRST,
            SECOND,
            THIRD,
            NONE
        };

        enum PlayerHeavyCharge {
            LEVEL_1,
            LEVEL_2,
            LEVEL_0
        };

        enum PlayerRangeCharge {
            CHARGE_1,
            CHARGE_2,
            CHARGE_3,
            CHARGE_0
        };

        struct AttackFrame {
            int startAttackFrame;
            int allowNextComboFrame;
            AttackFrame(): startAttackFrame(0), allowNextComboFrame(0) {}
            AttackFrame(int startAttackFrame, int allowNextComboFrame) : startAttackFrame(startAttackFrame), allowNextComboFrame(allowNextComboFrame) {}
        };

        int baseDamage[3];
        AttackFrame comboFrame[3];
        PlayerCombo currentCombo;
        bool isCurrentAttackFacingRight;
        float timeToResetComboRemaining;
        void startMeleeAttack();
        void endMeleeAttack();

        float damageMultiplier[2];
        float heavyAttackCooldown[2];
        AttackFrame heavyAttackFrame[2];
        PlayerHeavyCharge currentHeavyCharge;
        bool isInHeavyAttack;

        AttackFrame parryFrame;
        bool isParrying;
        bool successfulParry;

        int rangeDamageMultiplier[3];

        int baseRangeDamage;
        float rangeAttackCooldown[3];
        float rangeChargeDuration[3];
        PlayerRangeCharge currentRangeCharge;
        bool isInRangeAttack;
        float rangeHeldDuration;
        float rangeAttackCooldownRemaining;
        
        DamageCollider<EnemyObject>* attackHitbox;
        //int damage;
        float attackCooldownRemaining;
        bool isAttacking;
        bool isInAttackState;
        float timeBetweenLastAttack;

        void handleNormalAttack();
        void handleHeavyAttack();
        void handleRangeAttack();
        void handleParryAttack();
        
        bool isDodging;
        bool canDodge;
        float dodgeTimeElapsed;
        float dodgeCooldownLeft;
        
        void handleDodging();

        bool isJumping;
        int midAirFrameNum;
        int groundedFrameNum;
        bool canMove;
        bool canChangeFacingDirection;

        void handleMovement();
        void handleJumpMovement();
    
        void resetAttack();

        void handleFlinch();
        float flinchTimeRemaining;

        float iFrameTimeRemaining;

        glm::vec3 moveDirection;

        int stamina;
        float staminaRechargeTimer;
        float staminaRechargeDelayTimer;
        void resetStaminaRechargeDelay();

        int currentNumOfBullets;
        float bulletRechargeTimer;

        int currentNumOfPotion;
        float potionRechargeTimer;
        bool isHealing;
        bool healed;
        int healFrame;
        void resetHealing();
        void handleHealing();

    public:
        PlayerObject();
        ~PlayerObject();

        void normalAttack();
        void heavyAttack();
        void parryAttack();
        void rangeAttack(list<DrawableObject*>& objectsList);

        void startHeavyAttack();
        void startRangeAttack(float dt);

        //int getDamage() const;
        //void setDamage(int damage);

        virtual void start(list<DrawableObject*>& objectsList);
        virtual void updateBehavior(list<DrawableObject*>& objectsList);
        void updateStat();

        void move(glm::vec2 direction);
        void jump();
        void dodge();
        void dodge(float xDirection);

        void useHealthPotion();
        void resetNumOfPotion();

        bool getCanMove() const;
        bool getIsParrying() const;
        DamageCollider<EnemyObject>* getDamageCollider() const;

        void signalSuccessfulParry();

        virtual void takeDamage(int damage);
        void flinch(float duration);

        int getStamina() const;   
};