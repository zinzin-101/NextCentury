#pragma once
#include "ColliderObject.h"
#include "EnemyObject.h"
#include "GameEngine.h"
#include "LivingEntity.h"
#include "PlayerObject.h"
#include "SquareBorderMesh.h"

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

		void trigger(glm::vec3 pos);
		void trigger(glm::vec3 pos, glm::vec3 offset);

		void setLifeSpan(float lifespan);
		void setDamage(int damage);
		void setOwner(LivingEntity* owner);
		void setFollowOwner(bool value);
		void setFollowOffset(glm::vec3 offset);
		void setCanDecreaseTime(bool value);

		virtual void render(glm::mat4 globalModelTransform);
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
		entity->takeDamage(damage);
		std::cout << entity->getName() << " took " << damage << " damage" << std::endl;
	}
}

template <class TargetEntityType>
void DamageCollider<TargetEntityType>::trigger(glm::vec3 pos) {
	timeRemaining = lifespan;
	this->transform.setPosition(pos);
	this->setActive(true);
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
		squareBorderMesh->render();
	}
}