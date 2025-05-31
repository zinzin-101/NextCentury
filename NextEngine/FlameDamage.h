#pragma once
#include "ColliderObject.h"
#include "EnemyObject.h"
#include "GameEngine.h"
#include "LivingEntity.h"
#include "ParticleProperties.h"
#include "PlayerObject.h"
#include "SquareBorderMesh.h"

template <class TargetEntityType>
class FlameDamage : public ColliderObject, public TexturedObject {
	private:
		LivingEntity* owner;
		int damage;

		bool followOwner;
		glm::vec3 followOffset;

		bool flameStart;
		bool isWaitingToDeactivate;

	public:
		FlameDamage(LivingEntity* owner, int damage, float delayBetweenDamage);
		virtual void update(std::list<DrawableObject*>& objectsList);
		virtual void onCollisionStay(Collider* collider);

		void trigger(glm::vec3 pos, bool facingRight);
		void trigger(glm::vec3 pos, glm::vec3 offset, bool facingRight);

		void setDamage(int damage);
		void setOwner(LivingEntity* owner);
		void setFollowOwner(bool value);
		void setFollowOffset(glm::vec3 offset);
		void setFacingDirection(bool isFacingRight);
		virtual void setActive(bool value);

		LivingEntity* getOwner() const;

		virtual void render(glm::mat4 globalModelTransform);
		virtual void drawCollider();

		void reset();
};

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::onCollisionStay(Collider* col) {
	DrawableObject* obj = col->getObject();
	TargetEntityType* entity = dynamic_cast<TargetEntityType*>(obj);
	if (entity != NULL) {
		//entity->takeDamage(damage);
		entity->addStatus(LivingEntity::Status(LivingEntity::StatusType::BURNING, 3));
	}
}

template <class TargetEntityType>
FlameDamage<TargetEntityType>::FlameDamage(LivingEntity* owner, int damage, float delayBetweenDamage) : TexturedObject(),
	owner(owner), damage(damage) {
	this->getColliderComponent()->setTrigger(true);
	this->getColliderComponent()->setEnableCollision(false);
	this->setName(owner->getName() + "FlameDamage");
	this->DrawableObject::setActive(false);
	this->followOwner = false;

	this->setDrawCollider(true); // debug

	this->setTexture("../Resource/Texture/BlightFlameFixed3.png");
	this->initAnimation(9, 11);
	this->getAnimationComponent()->addState("Start", 3, 1, 4, false);
	this->getAnimationComponent()->addState("Blast", 3, 8, 3, true);
	this->getAnimationComponent()->addState("End", 4, 0, 9, false);

	this->getTransform().setScale(5.0f, 2.5f);
	this->getColliderComponent()->setDimension(0.4f, 0.7f);

	reset();
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::update(std::list<DrawableObject*>& objectsList) {
	processCollider();
	this->getAnimationComponent()->updateCurrentState();

	if (flameStart) {


		Animation::State currentState = this->getAnimationComponent()->getCurrentAnimationState();
		if (currentState.name == "Start") {
			if (!currentState.isPlaying) {
				this->getAnimationComponent()->setState("Blast");
			}
		}
		else if (currentState.name == "Blast") {
			this->getColliderComponent()->setEnableCollision(true);
			if (isWaitingToDeactivate) {
				this->getAnimationComponent()->setState("End");
				this->getColliderComponent()->setEnableCollision(false);
			}
		}
		else if (currentState.name == "End") {
			if (!currentState.isPlaying) {
				reset();
			}
		}
	}

	if (followOwner) {
		glm::vec3 pos = followOffset;
		pos.x *= (owner->getIsFacingRight() ? 1.0f : -1.0f);
		pos += owner->getTransform().getPosition();
		this->transform.setPosition(pos);
	}
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::setActive(bool value) {
	if (!value) {
		if (isWaitingToDeactivate) {
			return;
		}

		isWaitingToDeactivate = true;
		return;
	}

	DrawableObject::setActive(true);
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::trigger(glm::vec3 pos, bool facingRight) {
	//if (flameStart) {
	//	return;
	//}

	reset();

	this->transform.setPosition(pos);

	glm::vec3 currentScale = this->transform.getScale();
	currentScale.x = abs(currentScale.x) * (facingRight ? 1.0f : -1.0f);
	this->transform.setScale(currentScale);

	this->DrawableObject::setActive(true);

	this->getAnimationComponent()->setState("Start");
	flameStart = true;
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::trigger(glm::vec3 pos, glm::vec3 offset, bool facingRight) {
	//if (flameStart) {
	//	return;
	//}

	reset();

	this->transform.setPosition(pos + offset);
	
	glm::vec3 currentScale = this->transform.getScale();
	currentScale.x = abs(currentScale.x) * (facingRight ? 1.0f : -1.0f);
	this->transform.setScale(currentScale);
	
	this->DrawableObject::setActive(true);

	this->getAnimationComponent()->setState("Start");
	flameStart = true;
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::setDamage(int damage){
	this->damage = damage;
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::setOwner(LivingEntity* owner){
	this->owner = owner;
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::setFollowOwner(bool value){
	this->followOwner = value;
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::setFollowOffset(glm::vec3 offset) {
	this->followOffset = offset;
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::setFacingDirection(bool isFacingRight) {
	glm::vec3 currentScale = this->transform.getScale();
	currentScale.x = abs(currentScale.x) * (isFacingRight ? 1.0f : -1.0f);
	this->transform.setScale(currentScale);
}

template <class TargetEntityType>
LivingEntity* FlameDamage<TargetEntityType>::getOwner() const {
	return owner;
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::render(glm::mat4 globalModelTransform){
	this->TexturedObject::render(glm::mat4());

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
void FlameDamage<TargetEntityType>::drawCollider() {
	this->render(glm::mat4());
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::reset() {
	this->getColliderComponent()->setEnableCollision(false);
	flameStart = false;
	isWaitingToDeactivate = false;
	this->DrawableObject::setActive(false);
}