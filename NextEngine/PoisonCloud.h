#pragma once
#include "ColliderObject.h"
#include "LivingEntity.h"

class PoisonCloudCollider : public ColliderObject {
private:
	LivingEntity* owner;

	bool followOwner;
	glm::vec3 followOffset;

public:
	PoisonCloudCollider(LivingEntity* owner);
	virtual void update(std::list<DrawableObject*>& objectsList);
	virtual void onCollisionStay(Collider* collider);

	void setDamage(int damage);
	void setOwner(LivingEntity* owner);
	void setFollowOwner(bool value);
	void setFollowOffset(glm::vec3 offset);

	LivingEntity* getOwner() const;

	virtual void render(glm::mat4 globalModelTransform);
	virtual void drawCollider();
};