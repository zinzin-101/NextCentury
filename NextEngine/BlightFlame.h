#pragma once
#include "EnemyObject.h"

template <class T>
class FlameDamage;

namespace BlightFlameStat {
	constexpr int FLAME_DAMAGE = 1;
	constexpr int MELEE_DAMAGE = 5;

	constexpr float MELEE_ATTACK_DISTANCE = 1.1f;
	constexpr float FLAME_ATTACK_DURATION = 10.0f;

	constexpr float MELEE_TIME_PER_FRAME = 0.1667f;

	constexpr float STUN_DURATION = 2.0f;
}

class BlightFlame : public EnemyObject {
	private:
		FlameDamage<PlayerObject>* flameHitbox;

		enum AttackType {
			FLAME,
			MELEE
		};

		AttackType currentAttackType;
		void handleFlameAttack();
		void handleMeleeAttack();

protected:
	float flameTime = BlightFlameStat::FLAME_ATTACK_DURATION;
	float flameTimeKeep = 0;

	virtual void startAttack();
	virtual void endAttack();

public:
	BlightFlame(const EnemyInfo& enemyinfo);
	virtual ~BlightFlame();

	virtual void setDamage(int damage);

	virtual void start(list<DrawableObject*>& objectsList);
	virtual void updateState();
	virtual void updateBehavior(list<DrawableObject*>& objectlist);
	virtual void render(glm::mat4 globalModelTransform) { TexturedObject::render(glm::mat4()); emitter->render(glm::mat4()); };

	FlameDamage<PlayerObject>* getFlameCollider() const;
};