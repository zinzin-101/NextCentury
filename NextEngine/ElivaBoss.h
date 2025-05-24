#pragma once
#include "EnemyObject.h"
#include "ProjectileObject.h"
#include "PoisonCloud.h"

namespace ElivaStat {
	constexpr float COOLDOWN_DURATION = 2.0f;
	constexpr float DAMAGE_REDUCTION_ARMOR_BUFF = 0.5f;

	constexpr int MAX_HEALTH = 100;
	constexpr int HEALTH_TO_BEGIN_SERUM_INJECT = MAX_HEALTH * 0.6f;
	constexpr int HEALTH_TO_BEGIN_FURY = MAX_HEALTH * 0.25f;

	constexpr int BAYONET_DAMAGE = 15;

	constexpr int RIFLE_SHOT_DAMAGE = 18;
	constexpr float RIFLE_SHOT_SPEED = 8.0f;
	constexpr float RIFLE_SHOT_LIFESPAN = 10.0f;

	constexpr float BAYONET_SLASH_RANGE = 2.0f;
	constexpr float MAX_BLINK_DISTANCE_FROM_PLAYER = 6.0;

	constexpr float STUNNED_DURATION = 1.5f;

	constexpr float IDLE_TIME_PER_FRAME = 0.1667;
	constexpr float BLINKING_TIME_PER_FRAME = 0.1667;
	constexpr float RIFLE_SHOT_TIME_PER_FRAME = 0.1667;
	constexpr float BAYONET_SLASH_TIME_PER_FRAME = 0.1667;
	constexpr float PARRIED_TIME_PER_FRAME = 0.1667;
	constexpr float POISON_CLOUD_TIME_PER_FRAME = 0.1667;
	constexpr float SERUM_INJECT_TIME_PER_FRAME = 0.1667;
	constexpr float RAPID_BURST_TIME_PER_FRAME = 0.1667;
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
			Stunned
		};

		struct State {
			State(): currentState(BossState::Cooldown){}
			State(BossState currentState): currentState(currentState) {}
			BossState currentState;
			std::map<State*, TransitionCheckFunction> nextStateAndTransitionCheck;
		};

		virtual void start(list<DrawableObject*>& objectsList);
		virtual void postUpdateBehavior();
		virtual void updateBehavior(list<DrawableObject*>& objectsList);
		virtual void onCollisionStay(Collider* collider);

		void breakShield();
		void signalStun();

		float getCoolDownTimer() const;
		float getDistanceFromPlayer() const;
		bool isShieldActivated() const;
		bool hasFuryBeenActivated() const;
		float getStunnedTimer() const;

		ElivaBoss();
		~ElivaBoss();

	private:
		State states[11];
		State* currentState;

		ProjectileObject<PlayerObject>* rifleProjectile;
		DamageCollider<PlayerObject>* bayonetCollider;
		PoisonCloudCollider* poisonCollider;

		bool canBlink;

		bool isFuryUsed;
		float cooldownTimer;

		float stunnedTimer;

		bool hasShield;

		void processState();
		void (ElivaBoss::*statesHandler[11])();

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
};