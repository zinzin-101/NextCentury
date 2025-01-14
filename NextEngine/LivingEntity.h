#pragma once
#include "TexturedObject.h"
#include <list>

using namespace std;

class LivingEntity : public TexturedObject {
    public:
        struct Status;

    private:
        int health;
        bool canTakeDamage;
        bool isDead;
        bool isStun; // for when any status that can cause a stun
        list<Status> statusList;

    protected:
        bool isFacingRight;

    public:
        enum StatusType {
            STUN,
            POISON,
            MOODENG
        };

        struct Status {
            Status(StatusType type, float cooldown);
            StatusType type;
            float cooldown;
            float remainingTime;
        };

        LivingEntity();
        LivingEntity(int hp);
        LivingEntity(string name);
        LivingEntity(string name, int hp);

        void setHealth(int hp);
        void setCanTakeDamage(bool value);
        int getHealth();
        bool getIsDead();
        bool getIsStun();
        bool getIsFacingRight();
        void addStatus(Status newStatus);
        bool findStatus(Status target);
        void deleteStatus(int index);
        std::list<Status>::iterator deleteStatus(list<Status>::iterator itr);
        list<Status>& getStatusList();
        void applyStatus(float dt);
        void takeDamage(int damage);

        virtual void update(list<DrawableObject*>& objectsList);
        virtual void updateBehavior(list<DrawableObject*>& objectsList);
 };

bool operator==(LivingEntity::Status s1, LivingEntity::Status s2);