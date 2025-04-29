#pragma once
#include "EnemyObject.h"
#include "SonicWave.h"

namespace WailerStat {
	constexpr float SONIC_BLAST_COOLDOWN = 3.0f;
	constexpr float SUMMON_COOLDOWN = 15.0f;
	constexpr float DISTANCE_FROM_PLAYER_TO_REPOSITION = 3.0f;
	constexpr float TIME_UNTIL_REPOSITION = 3.0f;
}

class Wailer : public EnemyObject {
protected:
	enum State {
		IDLE,
		AGGRO,
		REPOSITIONING,
		ATTACKING,
		STUNNED,
		FLINCH
	};

	enum AttackState {
		SONICBLAST,
		SUMMONING,
		NONE
	};

	float repositionTimer;

	State currentState;
	SonicWave* sonicAttack;
	bool isInSonicAttack;
	AttackState currentAttackState;
	void handleAttackState(std::list<DrawableObject*>& objectlist);
	void handleSonicBlastState();
	void handleSummoningState(std::list<DrawableObject*>& objectlist);

	void handleReposition(float dt);

	void moveTowardsPosition(float xPosition);

public:
	Wailer(const EnemyInfo& enemyinfo);
	~Wailer();

	void resetAttack();

	void start(list<DrawableObject*>& objectsList);
	virtual void setCurrentState(State state);
	virtual void updateState();
	virtual void updateBehavior(std::list<DrawableObject*>& objectlist);
	void render(glm::mat4 globalModelTransform) { TexturedObject::render(glm::mat4()); emitter->render(glm::mat4()); };
};