#pragma once
#include "EnemyObject.h"
#include "SonicWave.h"
#include <set>

class Zealot;

namespace WailerStat {
	constexpr float SONIC_BLAST_COOLDOWN = 3.0f;
	constexpr float SUMMON_COOLDOWN = 15.0f;
	constexpr float DISTANCE_FROM_PLAYER_TO_REPOSITION = 10.0f;
	constexpr float TIME_UNTIL_REPOSITION = 1.25f;
	constexpr float REPOSITIONING_TIME = 2.5f;

	constexpr float DISTANCE_TO_SPAWN_ZEALOT = 17.0f;
	constexpr int MAX_ZEALOT_PER_WAILER = 1;

	constexpr float SONIC_WAVE_TIME_PER_FRAME = 0.1667f;
	constexpr float SUMMONING_TIME_PER_FRAME = 0.1667f;
}

class Wailer : public EnemyObject {
	public:
		enum State {
			IDLE,
			AGGRO,
			REPOSITIONING,
			ATTACKING,
			STUNNED,
			FLINCH
		};
	protected:
		enum AttackState {
			SONICBLAST,
			SUMMONING,
			NONE
		};

		float repositionTimer;
		float repositioningTimer;
		
		int zealotCounter;

		Wailer::State currentState;
		SonicWave* sonicAttack;
		bool isInSonicAttack;
		AttackState currentAttackState;
		void handleAttackState(std::list<DrawableObject*>& objectlist);
		void handleSonicBlastState();
		void handleSummoningState(std::list<DrawableObject*>& objectlist);

		std::set<Zealot*> summonedZealots;

		void handleReposition(float dt);

		void spawnZealot(glm::vec3 playerPos, std::list<DrawableObject*>& objectlist);
		void moveTowardsPosition(float xPosition);

	public:
		Wailer(const EnemyInfo& enemyinfo);
		~Wailer();

		void removeSummonedZealot(Zealot* zealot);
		void resetAttack();

		void start(list<DrawableObject*>& objectsList);
		virtual void setCurrentState(Wailer::State state);
		virtual void updateState();
		virtual void updateBehavior(std::list<DrawableObject*>& objectlist);
		void render(glm::mat4 globalModelTransform) { TexturedObject::render(glm::mat4()); emitter->render(glm::mat4()); };
};