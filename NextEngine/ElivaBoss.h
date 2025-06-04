#pragma once
#include "EnemyObject.h"
#include "ProjectileObject.h"
#include "PoisonCloud.h"

namespace ElivaStat {
	constexpr float COOLDOWN_DURATION = 1.0f;
	constexpr float FURY_COOLDOWN_DURATION = 0.5f;
	constexpr float DAMAGE_REDUCTION_ARMOR_BUFF = 0.5f;

	constexpr int MAX_HEALTH = 250;
	constexpr int HEALTH_TO_BEGIN_SERUM_INJECT = MAX_HEALTH * 0.6f;
	constexpr int HEALTH_TO_BEGIN_FURY = MAX_HEALTH * 0.25f;

	constexpr int BAYONET_DAMAGE = 15;

	constexpr int RIFLE_SHOT_DAMAGE = 18;
	constexpr float RIFLE_SHOT_SPEED = 10.0f;
	constexpr float RIFLE_SHOT_LIFESPAN = 10.0f;

	constexpr float BAYONET_SLASH_RANGE = 2.5f;
	constexpr float MAX_BLINK_DISTANCE_FROM_PLAYER = 6.0f;
	constexpr float MIN_BLINK_DISTANCE_FOR_RIFLE_SHOT_FROM_PLAYER = 4.0f;
	constexpr float MIN_BLINK_DISTANCE_FOR_BAYONET_FROM_PLAYER = 1.0f;

	constexpr float STUNNED_DURATION = 1.5f;
	constexpr float STAGGERED_DURATION = 0.5f;

	constexpr float IDLE_TIME_PER_FRAME = 0.1667f;
	constexpr float BLINKING_TIME_PER_FRAME = 0.1667f;
	constexpr float RIFLE_SHOT_TIME_PER_FRAME = 0.1667f;
	constexpr float BAYONET_SLASH_TIME_PER_FRAME = 0.1667f;
	constexpr float PARRIED_TIME_PER_FRAME = 0.1667f;
	constexpr float POISON_CLOUD_TIME_PER_FRAME = 0.0833f;
	constexpr float SERUM_INJECT_TIME_PER_FRAME = 0.1667f;
	constexpr float RAPID_BURST_TIME_PER_FRAME = 0.1667f;
	constexpr float DEAD_TIME_PER_FRAME = 0.3333f;

	constexpr float FURY_BAYONET_SLASH_TIME_PER_FRAME = 0.0833f;
	constexpr float FURY_BLINKING_TIME_PER_FRAME = 0.0833f;

	constexpr float DEATH_DIALOGUE_TIMER = 13.3f;
}

class ElivaBoss;

typedef bool (*TransitionCheckFunction)(ElivaBoss* boss);

class ElivaBoss : public EnemyObject {
	public:
		enum BossState {
			Cooldown = 0,
			Blink,
			FuryBlink,
			CloseBlink,
			RifleShot,
			BayonetSlash,
			PoisonCloud,
			RapidBurst,
			SerumInject,
			Fury,
			Stunned,
			FuryBayonetSlash,
			FuryCooldown,
			Dead,
			Intro
		};

		struct State {
			State(): currentState(BossState::Cooldown){}
			State(BossState currentState): currentState(currentState) {}
			BossState currentState;
			std::map<State*, TransitionCheckFunction> nextStateAndTransitionCheck;
		};

		enum Phase {
			First,
			Second,
			Third
		};

		virtual void start(list<DrawableObject*>& objectsList);
		virtual void postUpdateBehavior();
		virtual void updateBehavior(list<DrawableObject*>& objectsList);
		virtual void onCollisionStay(Collider* collider);

		virtual void takeDamage(int damage);
		virtual void onDeath(list<DrawableObject*>& objectsList);

		void signalCanStart();
		void breakShield();
		void signalStun();
		void signalStagger();
		void setBlinkOrigin(float x);

		bool getCanStart() const;
		Phase getCurrentPhase() const;
		float getCoolDownTimer() const;
		float getDistanceFromPlayer() const;
		bool isShieldActivated() const;
		bool hasFuryBeenActivated() const;
		bool hasSerumBeenInjected() const;
		float getStunnedTimer() const;
		bool getCanUsePoisonCloud() const;

		ElivaBoss();
		~ElivaBoss();

	private:
		unsigned int normalSprite;
		unsigned int shieldedSprite;

		State states[15];
		State* currentState;
		Phase currentPhase;

		ProjectileObject<PlayerObject>* rifleProjectiles[3];
		unsigned int currentProjectileIndex;
		bool hasRifleBeenFired;

		DamageCollider<PlayerObject>* bayonetCollider;
		PoisonCloudCollider* poisonCollider;

		bool canStart;

		bool canBlink;

		bool isFuryUsed;
		bool hasInjectedSerum;
		float cooldownTimer;

		float stunnedTimer;

		bool hasShield;

		bool isDead;

		bool canUsePoisonCloud;

		bool hasSignalDeath;
		float deathTimer;
		bool isDyingDialoguePlaying;

		glm::vec3 blinkOrigin;

		bool deathSoundPlayed = false;

		void processState();
		void (ElivaBoss::*statesHandler[15])();

		void handleCooldown();
		void handleBlink();
		void handleFuryBlink();
		void handleCloseBlink();
		void handleRifleShot();
		void handleBayonetSlash();
		void handlePoisonCloud();
		void handleRapidBurst();
		void handleSerumInject();
		void handleFury();
		void handleStunned();
		void handleFuryBayonetSlash();
		void handleFuryCooldown();
		void handleDead();
		void handleIntro();
};