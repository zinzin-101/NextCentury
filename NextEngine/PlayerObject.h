#pragma once
#include "DamageCollider.h"
#include "LivingEntity.h"
#include "PlayerInfo.h"
#include <list>

namespace PlayerStat {
    constexpr float DODGE_VELOCITY = 20.0f;
    constexpr float DODGE_COOLDOWN = 1.0f;
    constexpr float DODGE_DURATION = 0.1f;

    //constexpr float ATTACK_HITBOX_ACTIVE_TIME = 0.5f;
    constexpr float TIME_TO_RESET_COMBO = 2.0f;
    constexpr float ATTACK_COOLDOWN = 0.2f;
    constexpr float LAST_COMBO_COOLDOWN = 1.0f;
    constexpr float ATTACK_DASH_VELOCITY = 2.0f;
    constexpr float AFTER_ATTACK_MOVE_DELAY_TIME = 0.8f;
    constexpr int COMBO_DAMAGE_1 = 1;
    constexpr int COMBO_DAMAGE_2 = 2;
    constexpr int COMBO_DAMAGE_3 = 3;

    constexpr float MOVE_SPEED = 5.0f;
    constexpr float ACCEL_SPEED = 2000.0f;
    constexpr float DECEL_SPEED = 10.0f;

    constexpr float JUMP_VELOCITY = 25.0f;
}

class EnemyObject;

class PlayerObject : public LivingEntity {
    private:
        enum PlayerCombo {
            FIRST,
            SECOND,
            THIRD,
            NONE
        };

        struct Combo {
            int startAttackFrame;
            int allowNextComboFrame;
            Combo(): startAttackFrame(0), allowNextComboFrame(0) {}
            Combo(int startAttackFrame, int allowNextComboFrame) : startAttackFrame(startAttackFrame), allowNextComboFrame(allowNextComboFrame) {}
        };

        Combo comboFrame[3];
        PlayerCombo currentCombo;
        bool isCurrentAttackFacingRight;
        float timeToResetComboRemaining;
        void startAttack();
        void endAttack();

        DamageCollider<EnemyObject>* attackHitbox;
        int damage;
        float attackCooldownRemaining;
        bool isAttacking;
        bool isInAttackState;
        float timeBetweenLastAttack;
    
        bool isDodging;
        bool canDodge;
        bool canMove;
        bool canChangeFacingDirection;
        float dodgeTimeElapsed;
        float dodgeCooldownLeft;
    
        glm::vec3 moveDirection;

    public:
        PlayerObject(PlayerInfo& playerInfo);
        ~PlayerObject();

        void attack();

        int getDamage() const;
        void setDamage(int damage);

        virtual void start(list<DrawableObject*>& objectsList);
        virtual void updateBehavior(list<DrawableObject*>& objectsList);

        void move(glm::vec2 direction);
        void jump();
        void dodge();

        /// debugging ///
        virtual void onTriggerEnter(Collider* collider);
};