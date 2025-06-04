#pragma once
#include "EnemyObject.h"
#include "GameEngine.h"
#include "PlayerObject.h"
#include "RayObject.h"
#include "Wailer.h"

template <class TargetEntity>
class HitScanDamage : public RayObject {
	private:
		int damage;
		int multiplier;
		float lifespan;
		bool used;
		LivingEntity* closestEntity;

	public:
		HitScanDamage(glm::vec3 pos, glm::vec3 dir, float length, int damage, float lifespan);
		HitScanDamage(glm::vec3 pos, glm::vec3 dir, float length, int damage, int multiplier, float lifespan);
		virtual void onCollisionEnter(Collider* collider);
		virtual void update(std::list<DrawableObject*>& objectsList);
		void setDamage(int damage);
};

template <class TargetEntity>
HitScanDamage<TargetEntity>::HitScanDamage(glm::vec3 pos, glm::vec3 dir, float length, int damage, float lifespan) :
	RayObject(pos, dir, length), damage(damage), multiplier(1), lifespan(lifespan), used(false), closestEntity(nullptr) {
	//this->setDrawCollider(true);
	std::cout << "spawn hitscan, damage = " << damage << std::endl;
}

template <class TargetEntity>
HitScanDamage<TargetEntity>::HitScanDamage(glm::vec3 pos, glm::vec3 dir, float length, int damage, int multiplier, float lifespan) :
	RayObject(pos, dir, length), damage(damage), multiplier(multiplier), lifespan(lifespan), used(false), closestEntity(nullptr) {
	//this->setDrawCollider(true);
	std::cout << "spawn hitscan, damage = " << damage * multiplier << std::endl;
}

template <class TargetEntity>
void HitScanDamage<TargetEntity>::onCollisionEnter(Collider* collider) {
	if (used) {
		return;
	}

	TargetEntity* entity = dynamic_cast<TargetEntity*>(collider->getObject());
	if (entity != NULL) {
		if (closestEntity == nullptr) {
			closestEntity = entity;
			return;
		}

		glm::vec3 globalOrigin = this->getTransform().getPosition() + origin;

		glm::vec3 entityPos = entity->getTransform().getPosition();
		float newDistance = glm::length(entityPos - globalOrigin);

		glm::vec3 lastClosestPos = closestEntity->getTransform().getPosition();
		float oldDistance = glm::length(lastClosestPos - globalOrigin);

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

	closestEntity->takeDamage(damage * multiplier);
	EnemyObject* enemy = dynamic_cast<EnemyObject*>(closestEntity);
	if (enemy != NULL) {
		Wailer* wailer = dynamic_cast<Wailer*>(enemy);
		if (wailer != NULL) {
			wailer->setCurrentState(Wailer::State::FLINCH);
		}
		else {
			enemy->setCurrentState(EnemyObject::State::FLINCH);
		}
		enemy->signalBulletHit(multiplier);
	}

	used = true;
}

template <class TargetEntity>
void HitScanDamage<TargetEntity>::setDamage(int damage) {
	this->damage = damage;
}