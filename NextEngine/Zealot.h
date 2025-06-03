#pragma once
#include "EnemyObject.h"
#include "Wailer.h"

namespace ZealotStat {
	constexpr float STUN_DURATION = 2.0f;
	constexpr float ATTACK_ANIMATION_TIME_PER_FRAME = 0.12f;
}

class Zealot : public EnemyObject {
	private:
		Wailer* wailerSummonner;

	protected:
		enum AttackVariation {
			Variation1,
			Variation2
		};
		AttackVariation currentAttack = Variation1;

		virtual void startAttack();
		virtual void attack();
		virtual void endAttack();

	public:
		Zealot(const EnemyInfo& enemyinfo);
		~Zealot();
		void start(list<DrawableObject*>& objectsList);
		void updateState();
		void updateBehavior(list<DrawableObject*>& objectlist);

		void setWailerSummoner(Wailer* wailer);
};