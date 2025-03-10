#pragma once
#include "EnemyObject.h"

class Zealot : public EnemyObject {
protected:
	void moveTowardsTarget();
public:
	Zealot(EnemyInfo& enemyinfo);
	void start(list<DrawableObject*>& objectsList);
	void updateBehavior(list<DrawableObject*>& objectlist);
	void render(glm::mat4 globalModelTransform) { TexturedObject::render(glm::mat4()); emitter->render(glm::mat4()); };
};