#pragma once
#include "EnemyObject.h"

namespace ElivaStat {
	constexpr float COOLDOWN_DURATION = 1.0f;
	constexpr float DAMAGE_REDUCTION_ARMOR_BUFF = 0.5f;

	constexpr int MAX_HEALTH = 100;
	constexpr int HEALTH_TO_BEGIN_SERUM_INJECT = MAX_HEALTH * 0.6f;
	constexpr int HEALTH_TO_BEGIN_FURY = MAX_HEALTH * 0.25f;

	constexpr float BAYONET_SLASH_RANGE = 5.0f;

	constexpr float IDLE_TIME_PER_FRAME = 0.1667;
	constexpr float BLINKING_TIME_PER_FRAME = 0.1667;
	constexpr float RIFLE_SHOT_TIME_PER_FRAME = 0.1667;
	constexpr float BAYONET_SLASH_TIME_PER_FRAME = 0.1667;
	constexpr float POISON_CLOUD_TIME_PER_FRAME = 0.1667;
	constexpr float SERUM_INJECT_TIME_PER_FRAME = 0.1667;
}

class ElivaBoss;

typedef bool (*transitionCheckFunction)(ElivaBoss* boss);

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
			Fury
		};

		struct State {
			State(BossState currentState): currentState(currentState) {}
			BossState currentState;
			std::map<State*, transitionCheckFunction> nextStateAndTransitionCheck;
		};

		virtual void start(list<DrawableObject*>& objectsList);
		virtual void postUpdateBehavior();
		virtual void updateBehavior(list<DrawableObject*>& objectsList);
		virtual void onCollisionStay(Collider* collider);

		float getCoolDownTimer() const;
		float getDistanceFromPlayer() const;
		bool isShieldActivated() const;
		bool hasFuryBeenActivated() const;

	private:
		State states[10];
		BossState currentState;

		ElivaBoss();
		~ElivaBoss();

		void processState();

		bool isFuryUsed;
		float cooldownTimer;

		bool hasShield;
};