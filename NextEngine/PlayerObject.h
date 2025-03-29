#pragma once
#include "LivingEntity.h"
#include "PlayerInfo.h"
#include <list>

namespace PlayerStat {
    constexpr float DODGE_VELOCITY = 8.0f;
    constexpr float DODGE_COOLDOWN = 1.0f;
    constexpr float DODGE_DURATION = 0.8f;

    constexpr float TIME_TO_RESET_COMBO = 2.0f;
    constexpr float ATTACK_COOLDOWN = 0.2f;
    constexpr float LAST_COMBO_COOLDOWN = 1.0f;
    constexpr float ATTACK_DASH_VELOCITY = 2.0f;
    constexpr float AFTER_ATTACK_MOVE_DELAY_TIME = 0.8f;
    constexpr float DURATION_TO_START_HEAVY_ATTACK = 0.5f;
    constexpr float DURATION_TO_START_CHARGED_BULLET = 0.5f;

    constexpr int COMBO_DAMAGE_1 = 10;
    constexpr int COMBO_DAMAGE_2 = 12;
    constexpr int COMBO_DAMAGE_3 = 15;
    
    constexpr float CHARGE_DAMAGE_MULTIPLIER_1 = 1.2f;
    constexpr float CHARGE_DAMAGE_MULTIPLIER_2 = 1.5f;
    constexpr float HEAVY_ATTACK_COOLDOWN_1 = 0.5f;
    constexpr float HEAVY_ATTACK_COOLDOWN_2 = 0.8f;
    
    constexpr int RANGE_DAMAGE = 8;
    constexpr int NUM_OF_BULLET_PER_SHOT_1 = 1;
    constexpr int NUM_OF_BULLET_PER_SHOT_2 = 2;
    constexpr int NUM_OF_BULLET_PER_SHOT_3 = 3;
    constexpr float RANGE_CHARGE_DURATION_1 = 1.0f;
    constexpr float RANGE_CHARGE_DURATION_2 = 2.0f;
    constexpr float RANGE_CHARGE_DURATION_3 = 3.0f;
    constexpr float RANGE_ATTACK_COOLDOWN_1 = 2.0f;
    constexpr float RANGE_ATTACK_COOLDOWN_2 = 3.0f;
    constexpr float RANGE_ATTACK_COOLDOWN_3 = 4.0f;
    constexpr float RANGE_ATTACK_DISTANCE = 5.0f;
    constexpr float RANGE_ATTACK_LIFESPAN = 0.5f;

    constexpr float MOVE_SPEED = 5.0f;
    constexpr float ACCEL_SPEED = 2000.0f;
    constexpr float DECEL_SPEED = 10.0f;
    constexpr float AIR_ACCEL = 10.0f;

    constexpr float JUMP_VELOCITY = 25.0f;

    constexpr float INVINCIBLE_DURATION_AFTER_TAKING_DAMAGE = 0.5f;
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
        void flinch(float duration);
        void handleFlinch();
        float flinchTimeRemaining;

        float iFrameTimeRemaining;

        glm::vec3 moveDirection;

    public:
        PlayerObject(PlayerInfo& playerInfo);
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

        void move(glm::vec2 direction);
        void jump();
        void dodge();
        void dodge(float xDirection);

        bool getCanMove() const;
        bool getIsParrying() const;
        DamageCollider<EnemyObject>* getDamageCollider() const;

        virtual void takeDamage(int damage);

        /// debugging ///
        virtual void onTriggerEnter(Collider* collider);
};