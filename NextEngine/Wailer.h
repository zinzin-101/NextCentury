#pragma once
#include "EnemyObject.h"
#include "SonicWave.h"

namespace WailerStat {
	constexpr float SONIC_BLAST_COOLDOWN = 3.0f;
	constexpr float SUMMON_COOLDOWN = 15.0f;

}

class Wailer : public EnemyObject {
protected:
	enum AttackStates {
		Preparing,
		SonicBlast,
		Summoning,
		None
	};

	SonicWave* sonicAttack;
	AttackStates currentAttackState = None;
	void handleAttackState();
	void handlePrepareState();
	void handleSonicBlastState();
	void handleSummoningState();

	void startAttack();
	void endAttack();

public:
	Wailer(EnemyInfo& enemyinfo);
	void start(list<DrawableObject*>& objectsList);
	void updateState();
	void updateBehavior(list<DrawableObject*>& objectlist);
	void render(glm::mat4 globalModelTransform) { TexturedObject::render(glm::mat4()); emitter->render(glm::mat4()); };
};