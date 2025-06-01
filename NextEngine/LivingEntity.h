#pragma once
#include "ParticleSystem.h"
#include "TexturedObject.h"
#include <list>

using namespace std;

namespace LivingEntityStat {
    constexpr float DAMAGE_OVERLAY_DURATION = 0.2f;
    constexpr int BURNING_DAMAGE = 5;
    constexpr int POISON_DAMAGE = 5;
}

class LivingEntity : public TexturedObject {
    public:
        struct Status;

    private:
        int maxHealth;
        int health;
        bool canTakeDamage;
        bool isStun; // for when any status that can cause a stun
        list<Status> statusList;
        bool isAffectedByLighting;
        bool forceIgnoreLighting;

        void handleKnockback();
        void handleDamageOverlay();
        void handleLighting(list<DrawableObject*>& objectsList);

        void handleBurning();
        void handlePoison();

        void handleHealth(list<DrawableObject*>& objectsList);

        virtual void onDeath(list<DrawableObject*>& objectsList);

    protected:
        bool isFacingRight;

        float damageOverlayTimeRemaining;
        bool isDamageOverlayed;

        float isInKnockback;
        float knockbackDurationRemaining;

        ParticleSystem* emitter;

    public:
        enum StatusType {
            STUN,
            POISON,
            BURNING
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
        ~LivingEntity();

        void setHealth(int hp);
        void setMaxHealth(int hp);
        void setCanTakeDamage(bool value);

        int getHealth() const;
        int getMaxHealth() const;
        bool getCanTakeDamage() const;
        virtual bool getIsDead() const;
        bool getIsAlive() const;
        bool getIsStun() const;
        bool getIsFacingRight() const;

        void addStatus(Status newStatus);
        bool findStatus(Status target);
        void deleteStatus(int index);
        std::list<Status>::iterator deleteStatus(list<Status>::iterator itr);
        list<Status>& getStatusList();
        void applyStatus(float dt);

        virtual void takeDamage(int damage);
        virtual void takeDamage(int damage, bool ignoreCanTakeDamage);

        void heal(int amount);

        void knockback(glm::vec2 velocityDirection, float duration);

        void setAffectedByLighting(bool value);

        virtual void postUpdateBehavior();
        virtual void update(list<DrawableObject*>& objectsList);
        virtual void updateBehavior(list<DrawableObject*>& objectsList) = 0;
        virtual void lateUpdateBehavior();

        /// Particle System ///
        ParticleSystem* getEmitter() { return emitter; };
        virtual void render(glm::mat4 globalModelTransform) { TexturedObject::render(glm::mat4()); emitter->render(glm::mat4()); };
 };

bool operator==(LivingEntity::Status s1, LivingEntity::Status s2);