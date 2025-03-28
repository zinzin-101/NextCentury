#pragma once
#include "ColliderObject.h"
#include "EnemyObject.h"
#include "GameEngine.h"
#include "LivingEntity.h"
#include "ParticleProperties.h"
#include "PlayerObject.h"
#include "SquareBorderMesh.h"

template <class TargetEntityType>
class FlameDamage : public ColliderObject {
	private:
		LivingEntity* owner;
		int damage;

		float delayBetweenDamage;
		float delayTimer;

		bool followOwner;
		glm::vec3 followOffset;


public:
	FlameDamage(LivingEntity* owner, int damage, float delayBetweenDamage);
	virtual void update(std::list<DrawableObject*>& objectsList);
	virtual void onCollisionStay(Collider* collider);

	void trigger(glm::vec3 pos);
	void trigger(glm::vec3 pos, glm::vec3 offset);

	void setDamage(int damage);
	void setOwner(LivingEntity* owner);
	void setFollowOwner(bool value);
	void setFollowOffset(glm::vec3 offset);
	virtual void setActive(bool value);

	LivingEntity* getOwner() const;

	virtual void render(glm::mat4 globalModelTransform);
	virtual void drawCollider();
};

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::onCollisionStay(Collider* col) {
	DrawableObject* obj = col->getObject();
	TargetEntityType* entity = dynamic_cast<TargetEntityType*>(obj);
	if (entity != NULL) {
		entity->takeDamage(damage);
	}
}

template <class TargetEntityType>
FlameDamage<TargetEntityType>::FlameDamage(LivingEntity* owner, int damage, float delayBetweenDamage) :
	owner(owner), damage(damage), delayBetweenDamage(delayBetweenDamage), delayTimer(0.0f) {
	this->getColliderComponent()->setTrigger(true);
	this->setName(owner->getName() + "FlameDamage");
	this->setActive(false);
	this->followOwner = false;
	this->setDrawCollider(true);
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::update(std::list<DrawableObject*>& objectsList) {
	processCollider();

	if (followOwner) {
		glm::vec3 pos = followOffset;
		pos.x *= (owner->getIsFacingRight() ? 1.0f : -1.0f);
		pos += owner->getTransform().getPosition();
		this->transform.setPosition(pos);
	}

	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	if (delayTimer > 0.0f) {
		delayTimer -= dt;
	}

	//std::cout << "delay timer " << delayTimer << std::endl;
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::setActive(bool value) {
	DrawableObject::setActive(value);
	if (!DrawableObject::getIsActive()) {
		delayTimer = 0.0f;
	}
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::trigger(glm::vec3 pos) {
	this->transform.setPosition(pos);
	this->setActive(true);
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::trigger(glm::vec3 pos, glm::vec3 offset) {
	this->transform.setPosition(pos + offset);
	this->setActive(true);
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
LivingEntity* FlameDamage<TargetEntityType>::getOwner() const {
	return owner;
}

template <class TargetEntityType>
void FlameDamage<TargetEntityType>::render(glm::mat4 globalModelTransform){
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