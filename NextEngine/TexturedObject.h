#pragma once

#include "Animation.h"
#include "DrawableObject.h"
#include "Physics.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <string>

class TexturedObject : virtual public DrawableObject
{
	private:
		unsigned int texture;
		Animation* animation;

	protected:
		glm::vec4 colorOverlay;

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