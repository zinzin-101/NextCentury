#pragma once
#include "EnemyObject.h"

class Wailer : public EnemyObject {
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
	Wailer(EnemyInfo& enemyinfo);
	void start(list<DrawableObject*>& objectsList);
	void updateState();
	void updateBehavior(list<DrawableObject*>& objectlist);
	void render(glm::mat4 globalModelTransform) { TexturedObject::render(glm::mat4()); emitter->render(glm::mat4()); };
};