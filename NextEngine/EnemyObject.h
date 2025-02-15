#pragma once
#include "EnemyInfo.h"
#include "LivingEntity.h"
#include "MovementInfo.h"
#include <list>

class PlayerObject;

template <class T>
class DamageCollider;

class EnemyObject : public LivingEntity {
    public:
        enum State {
            IDLE,
            AGGRO,
            ATTACKING,
            STUNNED
        };

    protected:
        DamageCollider<PlayerObject>* attackHitbox;
        float hitboxActiveTime;
        //float hitboxTimer;

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

        float getDistanceFromTarget() const;
        virtual void moveTowardsTarget();
        void startAttack();
        void endAttack();

    public:
        EnemyObject(EnemyInfo& enemyInfo);
        void setCurrentState(State state);
        State getCurrentState();

        /*SimpleObject* getHitbox() const;

        void activateHitbox();
        void deactivateHitbox();
        bool isHitboxActive() const;*/

        void findTarget(std::string name, std::list<DrawableObject*>& objectList);
        void setTarget(LivingEntity* target);

        void setAttackCooldown(float cooldown);
        float getAttackCooldown() const;

        void setMovementInfo(MovementInfo movementInfo);
        void setAggroRange(float range);
        void setAttackRange(float range);
        void setDamage(int damage);
        float getAggroRange() const;
        float getAttackRange() const;

        void setCanAttack(bool value);

        virtual void start(list<DrawableObject*>& objectsList);
        void updateState();
        virtual void updateBehavior(list<DrawableObject*>& objectsList);


        ~EnemyObject();
};