#pragma once
#include "ColliderObject.h"
#include "GameEngine.h"
#include "LivingEntity.h"
#include "SimpleObject.h"
#include "TexturedObject.h"

template <class TargetEntity>
class ProjectileObject : public SimpleObject { // change later to TexturedObject
	private:
		int damage;
		float lifespan;
		LivingEntity* owner;

		bool canDespawn;

	public:
		ProjectileObject(LivingEntity* owner, int damage, glm::vec3 position, glm::vec2 velocity, float lifespan);
		virtual void update(std::list<DrawableObject*>& objectsList);
		virtual void onCollisionEnter(Collider* collider);
};

template <class TargetEntity>
ProjectileObject<TargetEntity>::ProjectileObject(LivingEntity* owner, int damage, glm::vec3 position, glm::vec2 velocity, float lifespan): 
	owner(owner), damage(damage), lifespan(lifespan) {
	this->getTransform().setPosition(position);
	this->addPhysicsComponent();
	this->getPhysicsComponent()->setEnableGravity(false);
	this->getPhysicsComponent()->setVelocity(velocity);
	this->addColliderComponent();
	this->getColliderComponent()->setTrigger(true);
	(lifespan <= 0.0f) ? canDespawn = false : canDespawn = true;
}

template <class TargetEntity>
void ProjectileObject<TargetEntity>::update(std::list<DrawableObject*>& objectsList) {
	DrawableObject::update(objectsList);

	if (canDespawn) {
		lifespan -= GameEngine::getInstance()->getTime()->getDeltaTime();

		if (lifespan <= 0.0f) {
			destroyObject(this);
		}
	}
}

template <class TargetEntity>
void ProjectileObject<TargetEntity>::onCollisionEnter(Collider* collider) {
	DrawableObject* obj = collider->getObject();
	TargetEntity* entity = dynamic_cast<TargetEntity*>(obj);

	if (entity != NULL) {
		entity->takeDamage(damage);
		destroyObject(this);
		return;
	}

	ColliderObject* colObj = dynamic_cast<ColliderObject*>(obj);

	if (colObj != NULL) {
		destroyObject(this);
		return;
	}
}
