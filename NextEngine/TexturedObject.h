#pragma once

#include "Animation.h"
#include "DrawableObject.h"
#include "Physics.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <string>

class TexturedObject : virtual public DrawableObject
{
	unsigned int texture;
	Animation* animation; // Might use vector in the future to support multiple animation state
	/*bool animated = false;
	int rowCount;
	int colCount;
	float offsetX;
	float offsetY;*/

public:
	TexturedObject();
	TexturedObject(std::string name);
	~TexturedObject();
	void setTexture(std::string path);
	void initAnimation(int row, int column);
	void render(glm::mat4 globalModelTransform);
	Animation* getAnimationComponent();
	bool isAnimated();
	//void setFrame(float row, float column);
};