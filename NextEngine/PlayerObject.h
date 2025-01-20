#pragma once
#include "DamageCollider.h"
#include "LivingEntity.h"
#include "MovementInfo.h"
#include "PlayerInfo.h"
#include <list>

constexpr float DODGE_VELOCITY = 50.0f;
constexpr float DODGE_COOLDOWN = 1.0f;
constexpr float DODGE_DURATION = 0.05f;
constexpr float HITBOX_ACTIVE_TIME = 0.5f;

class EnemyObject;

class PlayerObject : public LivingEntity {
    private:
        DamageCollider<EnemyObject>* attackHitbox;
        float attackCooldown;  
        int damage;

        MovementInfo movementInfo;
    
        bool isDodging;
        bool canDodge;
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

        void setMovementInfo(MovementInfo movementInfo);

        void move(glm::vec2 direction);
        void jump();
        void dodge();

        /// debugging ///
        virtual void onTriggerEnter(Collider* collider);
};