#pragma once
#include "Collider.h"
#include "DrawableObject.h"

class ColliderObject : public virtual DrawableObject {
	public:
		ColliderObject();
		ColliderObject(std::string name);
		virtual void drawCollider();
		virtual void render(glm::mat4 globalModelTransform);
};