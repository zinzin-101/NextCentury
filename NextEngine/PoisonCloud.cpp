#include "PoisonCloud.h"
#include "PlayerObject.h"

PoisonCloudCollider::PoisonCloudCollider(LivingEntity* owner) : owner(owner) {
	this->getColliderComponent()->setTrigger(true);
	this->getColliderComponent()->setEnableCollision(false);
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