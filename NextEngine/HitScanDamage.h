#pragma once
#include "EnemyObject.h"
#include "GameEngine.h"
#include "PlayerObject.h"
#include "RayObject.h"

template <class TargetEntity>
class HitScanDamage : public RayObject {
	private:
		int damage;
		float lifespan;
		bool used;
		LivingEntity* closestEntity;

	public:
		HitScanDamage(glm::vec3 pos, glm::vec3 dir, float length, int damage, float lifespan);
		virtual void onCollisionEnter(Collider* collider);
		virtual void update(std::list<DrawableObject*>& objectsList);
		void setDamage(int damage);
};

template <class TargetEntity>
HitScanDamage<TargetEntity>::HitScanDamage(glm::vec3 pos, glm::vec3 dir, float length, int damage, float lifespan) :
	RayObject(pos, dir, length), damage(damage), lifespan(lifespan), used(false), closestEntity(nullptr) {
	this->setDrawCollider(true);
	std::cout << "spawn hitscan" << std::endl;
}

template <class TargetEntity>
void HitScanDamage<TargetEntity>::onCollisionEnter(Collider* collider) {

	cout << "hitscan collided with " << collider->getObject()->getName() << endl;

	TargetEntity* entity = dynamic_cast<TargetEntity*>(collider->getObject());
	if (entity != NULL) {
		if (closestEntity == nullptr) {
			closestEntity = entity;
			return;
		}

		glm::vec3 entityPos = entity->getTransform().getPosition();
		float newDistance = glm::length(entityPos - origin);

		glm::vec3 lastClosestPos = closestEntity->getTransform().getPosition();
		float oldDistance = glm::length(lastClosestPos - origin);

		if (newDistance < oldDistance) {
			closestEntity = entity;
		}
	}
}

template <class TargetEntity>
void HitScanDamage<TargetEntity>::update(std::list<DrawableObject*>& objectsList) {
	DrawableObject::update(objectsList);

	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	lifespan -= dt;

	if (lifespan <= 0.0f) {
		DrawableObject::destroyObject(this);
		return;
	}
	
	if (used || closestEntity == nullptr) {
		return;
	}

	closestEntity->takeDamage(damage);
	used = true;
}

template <class TargetEntity>
void HitScanDamage<TargetEntity>::setDamage(int damage) {
	this->damage = damage;
}