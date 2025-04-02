#pragma once
#include "ColliderObject.h"
#include "EnemyObject.h"
#include "GameEngine.h"
#include "LivingEntity.h"
#include "ParticleProperties.h"
#include "PlayerObject.h"
#include "SquareBorderMesh.h"

/// testing ///
#include "random.h"

template <class TargetEntityType>
class DamageCollider : public ColliderObject {
	private:
		float lifespan;
		int damage;
		LivingEntity* owner;

		float timeRemaining;
		bool canDecreaseTimeRemaining;

		bool followOwner;
		glm::vec3 followOffset;


	public:
		DamageCollider(LivingEntity* owner, int damage, float lifespan);
		virtual void update(std::list<DrawableObject*>& objectsList);
		virtual void onCollisionEnter(Collider* collider);
		virtual void onTriggerEnter(Collider* collider);

		void trigger(glm::vec3 pos);
		void trigger(glm::vec3 pos, glm::vec3 offset);

		void setLifeSpan(float lifespan);
		void setDamage(int damage);
		void setOwner(LivingEntity* owner);
		void setFollowOwner(bool value);
		void setFollowOffset(glm::vec3 offset);
		void setCanDecreaseTime(bool value);

		LivingEntity* getOwner() const;

		virtual void render(glm::mat4 globalModelTransform);
		virtual void drawCollider();
};

template <class TargetEntityType>
DamageCollider<TargetEntityType>::DamageCollider(LivingEntity* owner, int damage, float lifespan) : owner(owner), damage(damage), lifespan(lifespan) {
	this->getColliderComponent()->setTrigger(true);
	this->setName(owner->getName() + "DamageCollider");
	this->timeRemaining = 0.0f;
	this->setActive(false);
	this->followOwner = false;
	this->setDrawCollider(true); // for debug
	this->canDecreaseTimeRemaining = true;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::update(std::list<DrawableObject*>& objectsList) {
	processCollider();

	if (followOwner) {
		glm::vec3 pos = followOffset;
		pos.x *= (owner->getIsFacingRight() ? 1.0f : -1.0f);
		pos += owner->getTransform().getPosition();
		this->transform.setPosition(pos);
	}

	if (lifespan <= 0.0f) {
		return;
	}

	if (timeRemaining > 0.0f && canDecreaseTimeRemaining) {
		//std::cout << "hitbox active" << std::endl;
		timeRemaining -= GameEngine::getInstance()->getTime()->getDeltaTime();
		if (timeRemaining <= 0.0f) {
			//std::cout << "hitbox deactivated" << std::endl;
			this->setActive(false);
			timeRemaining = 0.0f;
		}
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::onCollisionEnter(Collider* collider) {
	DrawableObject* obj = collider->getObject();
	//EnemyObject* entity = dynamic_cast<EnemyObject*>(obj);
	TargetEntityType* entity = dynamic_cast<TargetEntityType*>(obj);

	if (entity != NULL) {
		EnemyObject* enemy = dynamic_cast<EnemyObject*>(obj);
		if (enemy != NULL) {
			PlayerObject* playerAsOwner = dynamic_cast<PlayerObject*>(this->owner);

			if (playerAsOwner != NULL && playerAsOwner->getIsParrying()) {
				return;
			}
		}
		
		entity->takeDamage(damage);
		//std::cout << entity->getName() << " took " << damage << " damage" << std::endl;
		PlayerObject* player = dynamic_cast<PlayerObject*>(obj);
		if (player != NULL && !player->getIsParrying() && player->getCanTakeDamage()) {
			player->flinch(0.5f);
			return;
		}
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::onTriggerEnter(Collider* collider) { // for handling parrying from player
	DrawableObject* obj = collider->getObject();
	DamageCollider<EnemyObject>* damageCollider = dynamic_cast<DamageCollider<EnemyObject>*>(obj);

	if (damageCollider != NULL) {
		PlayerObject* player = dynamic_cast<PlayerObject*>(damageCollider->getOwner());
		if (player != NULL) {
			if (player->getIsParrying()) {
				// implement stun later
				std::cout << owner->getName() << " got parried" << std::endl;
				GameEngine::getInstance()->getRenderer()->getCamera()->startShake(0.3f);
				EnemyObject* enemyObj = dynamic_cast<EnemyObject*>(this->getOwner());

				if (enemyObj != NULL) {
					enemyObj->setCurrentState(enemyObj->STUNNED);
					float parryDirection = (player->getTransform().getPosition().x < enemyObj->getTransform().getPosition().x) ? 1.0f : -1.0f;
					for (int i = 0; i < 5; i++) {
						ParticleProperties particleProps = ParticleProperties(
							enemyObj->getTransform().getPosition(),
							20.0f * glm::vec2(parryDirection * Random::Float(), Random::Float()),
							glm::vec2(-0.1f, 0.1f), 
							glm::vec3(0.8f, 0, 0),
							0.2f, 0.1f, 0.05f, 1.0f
						);
						enemyObj->getEmitter()->emit(particleProps);
					}

				}
			}
		}
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::trigger(glm::vec3 pos) {
	timeRemaining = lifespan;
	this->transform.setPosition(pos);
	this->setActive(true);

	// debug //
	//std::cout << "damage: " << damage << std::endl;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::trigger(glm::vec3 pos, glm::vec3 offset) {
	timeRemaining = lifespan;
	this->transform.setPosition(pos + offset);
	this->setActive(true);
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setLifeSpan(float lifespan) {
	this->lifespan = lifespan;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setDamage(int damage) {
	this->damage = damage;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setOwner(LivingEntity* owner) {
	this->owner = owner;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setFollowOwner(bool value) {
	followOwner = value;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setFollowOffset(glm::vec3 offset) {
	followOffset = offset;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::setCanDecreaseTime(bool value) {
	canDecreaseTimeRemaining = value;
}

template <class TargetEntityType>
LivingEntity* DamageCollider<TargetEntityType>::getOwner() const {
	return owner;
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::render(glm::mat4 globalModelTransform) {
	if (collider == nullptr) {
		return;
	}

	if (!canDrawCollider || !this->getIsActive()) {
		return;
	}

	SquareBorderMesh* squareBorderMesh = dynamic_cast<SquareBorderMesh*> (GameEngine::getInstance()->getRenderer()->getMesh(SquareBorderMesh::MESH_NAME));

	GLuint modelMatixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();
	GLuint colorId = GameEngine::getInstance()->getRenderer()->getColorUniformId();
	GLuint brightnessId = GameEngine::getInstance()->getRenderer()->getBrightnessUniformId();

	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (renderModeId == -1) {
		cout << "Error: Can't set renderMode in DamageCollider " << endl;
		return;
	}
	if (colorId == -1) {
		cout << "Error: Can't set color " << endl;
		return;
	}

	vector <glm::mat4> matrixStack;

	//glm::mat4 currentMatrix = this->getTransformMat4();
	Transform colTransform = collider->getGlobalTransform(this->transform);
	colTransform.scales(collider->getWidth(), collider->getHeight());
	glm::mat4 currentMatrix = colTransform.getTransformMat4();
	//glm::mat4 currentMatrix = collider->getGlobalTransform(this->transform).getTransformMat4();

	if (squareBorderMesh != nullptr) {

		//currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, 1, 0, 0);
		glUniform1i(renderModeId, 0);
		glUniform1f(brightnessId, 1.0f);
		squareBorderMesh->render();
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::drawCollider() {
	this->render(glm::mat4());
}