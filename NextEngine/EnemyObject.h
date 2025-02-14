#pragma once
#include "EnemyInfo.h"
#include "LivingEntity.h"
#include "MovementInfo.h"
#include <list>

class PlayerObject;

template <class T>
class DamageCollider;

class EnemyObject : public LivingEntity {
protected:
    enum State {
        IDLE,
        AGGRO,
        ATTACKING,
        STUNNED
    };
    DamageCollider<PlayerObject>* attackHitbox;
    float hitboxActiveTime;
    //float hitboxTimer;

    State currentState;
    float aggroRange;
    float attackRange;
    float attackCooldown;
    float attackingCooldownTimer;
    bool canAttack;
    int damage;

    MovementInfo movementInfo;
    LivingEntity* targetEntity;

    bool isAttacking;
    int attackFrameStart;
    int attackFrameEnd;

public:
    EnemyObject(EnemyInfo& enemyInfo);
    void setCurrentState(State state);
    State getCurrentState();

    /*SimpleObject* getHitbox() const;

    void activateHitbox();
    void deactivateHitbox();
    bool isHitboxActive() const;*/

    virtual void moveTowards(glm::vec3 targetPos);

    void findTarget(std::string name, std::list<DrawableObject*>& objectList);
    void setTarget(LivingEntity* target);

    void setAttackCooldown(float cooldown);
    float getAttackCooldown() const;

    bool isPlayerInAttackRange(glm::vec3 targetPos);
    bool isPlayerInAggroRange(glm::vec3 targetPos);

    void setMovementInfo(MovementInfo movementInfo);
    void setAggroRange(float range);
    void setAttackRange(float range);
    void setDamage(int damage);
    float getAggroRange() const;
    float getAttackRange() const;

    void setCanAttack(bool value);
    void attack();

    virtual void start(list<DrawableObject*>& objectsList);
    void updateState(glm::vec3 targetPos);
    virtual void updateBehavior(list<DrawableObject*>& objectsList);


    ~EnemyObject();
};