#pragma once
#include "ColliderObject.h"
#include "GameEngine.h"
#include "LivingEntity.h"
#include "ParticleSystem.h"
#include "SimpleObject.h"
#include "TexturedObject.h"

template <class TargetEntity>
class ProjectileObject : public SimpleObject { // change later to TexturedObject
	private:
		int damage;
		float lifespan;
		LivingEntity* owner;

		bool canDespawn;

		/// test ///
		ParticleSystem* emitter;
		ParticleProperties particleProps;

	public:
		ProjectileObject(LivingEntity* owner, int damage, glm::vec3 position, glm::vec2 velocity, float lifespan);
		virtual void update(std::list<DrawableObject*>& objectsList);
		virtual void onCollisionEnter(Collider* collider);

		/// test ///
		virtual void render(glm::mat4 globalModelTransform);
		~ProjectileObject();
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

	/// test ///
	particleProps = ParticleProperties(this->transform.getPosition(), glm::normalize(this->physics->getVelocity()), glm::vec2(1.0f, 1.0f), glm::vec3(),
		0.2f, 0.1f, 0.05f, 1.0f);
	emitter = new ParticleSystem();
}

template <class TargetEntity>
ProjectileObject<TargetEntity>::~ProjectileObject() {
	if (emitter != nullptr) {
		delete emitter;
	}
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

	/// test ///
	unsigned int ticks = GameEngine::getInstance()->getTime()->getTicks();
	if (emitter != nullptr) {
		if (ticks % 5 == 0) {
			particleProps.position = this->getTransform().getPosition();
			emitter->emit(particleProps);
		}
		emitter->update(objectsList);
	}
}

template <class TargetEntity>
void ProjectileObject<TargetEntity>::render(glm::mat4 globalModelTransform) {
	SimpleObject::render(globalModelTransform);

	if (emitter != nullptr) {
		emitter->render(globalModelTransform);
	}
}

template <class TargetEntity>
void ProjectileObject<TargetEntity>::onCollisionEnter(Collider* collider) {
	DrawableObject* obj = collider->getObject();
	TargetEntity* entity = dynamic_cast<TargetEntity*>(obj);

	if (entity != NULL) {

		if (!entity->getCanTakeDamage()) {
			return;
		}

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
