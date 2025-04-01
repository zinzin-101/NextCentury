#pragma once
#include "EnemyObject.h"

namespace ZealotStat {
	constexpr float STUN_DURATION = 2.0f;
}

class Zealot : public EnemyObject {
protected:
	enum AttackVariation {
		Variation1,
		Variation2
	};
	AttackVariation currentAttack = Variation1;

	void moveTowardsTarget();
	void startAttack();
	void endAttack();

public:
	Zealot(const EnemyInfo& enemyinfo);
	void start(list<DrawableObject*>& objectsList);
	void updateState();
	void updateBehavior(list<DrawableObject*>& objectlist);
};