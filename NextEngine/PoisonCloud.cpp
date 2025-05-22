#include "PoisonCloud.h"
#include "GameEngine.h"
#include "PlayerObject.h"
#include "SquareBorderMesh.h"

PoisonCloudCollider::PoisonCloudCollider(LivingEntity* owner) : owner(owner) {
	this->getColliderComponent()->setTrigger(true);
	this->setName(owner->getName() + "PoisonCloud");
	this->DrawableObject::setActive(false);
	this->followOwner = false;

	this->setDrawCollider(true); // debug

	this->getTransform().setScale(2.0f, 1.0f);
}

void PoisonCloudCollider::onCollisionStay(Collider* col) {
	DrawableObject* obj = col->getObject();
	PlayerObject* entity = dynamic_cast<PlayerObject*>(obj);
	if (entity != NULL) {
		//entity->takeDamage(damage);
		entity->addStatus(LivingEntity::Status(LivingEntity::StatusType::POISON, 3));
	}
}

void PoisonCloudCollider::update(std::list<DrawableObject*>& objectsList) {
	processCollider();

	if (followOwner) {
		glm::vec3 pos = followOffset;
		pos.x *= (owner->getIsFacingRight() ? 1.0f : -1.0f);
		pos += owner->getTransform().getPosition();
		this->transform.setPosition(pos);
	}
}

void PoisonCloudCollider::setOwner(LivingEntity* owner) {
	this->owner = owner;
}

void PoisonCloudCollider::setFollowOwner(bool value) {
	this->followOwner = value;
}

void PoisonCloudCollider::setFollowOffset(glm::vec3 offset) {
	this->followOffset = offset;
}

LivingEntity* PoisonCloudCollider::getOwner() const {
	return owner;
}

void PoisonCloudCollider::render(glm::mat4 globalModelTransform) {
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

void PoisonCloudCollider::drawCollider() {
	this->render(glm::mat4());
}