#pragma once

#include "DrawableObject.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
class SimpleObject : virtual public DrawableObject {
	glm::vec3 color;

public:
	SimpleObject();
	SimpleObject(string name);
	~SimpleObject();
	///////////////////////////////////////////////
	virtual void onCollisionStay(Collider* collider);
	virtual void onCollisionExit(Collider* collider);
	virtual void onTriggerStay(Collider* collider);
	virtual void onTriggerExit(Collider* collider);
	///////////////////////////////////////////////
	void setColor(float r, float g, float b);
	void render(glm::mat4 globalModelTransform);
};

