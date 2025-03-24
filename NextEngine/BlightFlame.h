#pragma once
#include "EnemyObject.h"

class BlightFlame : public EnemyObject {
protected:

	float flameTime = 1.5f;
	float flameTimeKeep = 0;

	void moveTowardsTarget();
	void startAttack();
	void endAttack();

public:
	BlightFlame(EnemyInfo& enemyinfo);
	void start(list<DrawableObject*>& objectsList);
	void updateState();
	void updateBehavior(list<DrawableObject*>& objectlist);
	void render(glm::mat4 globalModelTransform) { TexturedObject::render(glm::mat4()); emitter->render(glm::mat4()); };
};