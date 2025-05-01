#pragma once
#include "EnemyInfo.h"
#include "LivingEntity.h"
#include "MovementInfo.h"
#include <list>
#include "GameEngine.h"
#include "ProjectileObject.h"

/// test ///
#include "ParticleSystem.h"

class PlayerObject;

template <class T>
class DamageCollider;

namespace EnemyStat {
    constexpr float FLINCH_TIME = 0.2f;
    constexpr int NUM_OF_BULLET_TO_STUN = 3;
}

class EnemyObject : public LivingEntity {
    public:
        enum State {
            IDLE,
            AGGRO,
            ATTACKING,
            STUNNED,
            FLINCH
        };

    protected:
        DamageCollider<PlayerObject>* attackHitbox;
        //float hitboxActiveTime;
        //float hitboxTimer;

        bool canMoveTowardTarget;

        State currentState;
        float aggroRange;
        float attackRange;
        float attackCooldown;
        float attackCooldownTimer;
        bool canAttack;
        int damage;

        MovementInfo movementInfo;
        LivingEntity* targetEntity;

        bool isAttacking;
        int attackFrameStart;
        int attackFrameEnd;

        float stunnedTime;
        float currentStunnedTime;

        float flinchTimer;

        int bulletHitCounter;

        float getDistanceFromTarget() const;
        virtual void moveTowardsTarget();
        virtual void startAttack();
        virtual void endAttack();

        virtual void onDeath(std::list<DrawableObject*>& objectsList);

    public:
        EnemyObject(const EnemyInfo& enemyInfo);
        virtual void setCurrentState(State state);
        State getCurrentState();

        /*SimpleObject* getHitbox() const;

        void activateHitbox();
        void deactivateHitbox();
        bool isHitboxActive() const;*/

        void findTarget(std::string name, std::list<DrawableObject*>& objectList);
        void setTarget(LivingEntity* target);
        
        static PlayerObject* findPlayer(std::list<DrawableObject*>& objectList);

        void setAttackCooldown(float cooldown);
        float getAttackCooldown() const;

        void setMovementInfo(MovementInfo movementInfo);
        void setAggroRange(float range);
        void setAttackRange(float range);
        virtual void setDamage(int damage);
        float getAggroRange() const;
        float getAttackRange() const;

        void setCanAttack(bool value);

        MovementInfo getMovementInfo() const;

        void signalBulletHit(int numOfBullet);

        virtual void start(list<DrawableObject*>& objectsList);
        virtual void updateState();
        virtual void postUpdateBehavior();
        virtual void updateBehavior(list<DrawableObject*>& objectsList);

        DamageCollider<PlayerObject>* getDamageCollider() const;

        virtual ~EnemyObject();

        virtual void onCollisionStay(Collider* collider);
};