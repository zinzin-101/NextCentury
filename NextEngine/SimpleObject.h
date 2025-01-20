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
	void setColor(float r, float g, float b);
	void setColor(glm::vec3 color);
	virtual void render(glm::mat4 globalModelTransform);
};

