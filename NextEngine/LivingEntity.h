#pragma once
#include "TexturedObject.h"
#include <list>

using namespace std;

namespace LivingEntityStat {
    constexpr float DAMAGE_OVERLAY_DURATION = 0.2f;
}

class LivingEntity : public TexturedObject {
    public:
        struct Status;

    private:
        int health;
        bool canTakeDamage;
        bool isStun; // for when any status that can cause a stun
        list<Status> statusList;
        bool isAffectedByLighting;

        void handleKnockback();
        void handleDamageOverlay();
        void handleLighting(list<DrawableObject*>& objectsList);

    protected:
        bool isFacingRight;

        float damageOverlayTimeRemaining;
        bool isDamageOverlayed;

        float isInKnockback;
        float knockbackDurationRemaining;

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

        int getHealth() const;
        bool getCanTakeDamage() const;
        bool getIsDead() const;
        bool getIsStun() const;
        bool getIsFacingRight() const;

        void addStatus(Status newStatus);
        bool findStatus(Status target);
        void deleteStatus(int index);
        std::list<Status>::iterator deleteStatus(list<Status>::iterator itr);
        list<Status>& getStatusList();
        void applyStatus(float dt);

        virtual void takeDamage(int damage);

        void knockback(glm::vec2 velocityDirection, float duration);

        void setAffectedByLighting(bool value);

        virtual void update(list<DrawableObject*>& objectsList);
        virtual void updateBehavior(list<DrawableObject*>& objectsList) = 0;
 };

bool operator==(LivingEntity::Status s1, LivingEntity::Status s2);