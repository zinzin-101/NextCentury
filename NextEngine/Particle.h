#pragma once
#include "DrawableObject.h"
#include "SimpleObject.h"
#include <list>

class Particle : public SimpleObject {
	private:
		float initSize;
		float endSize;
		glm::vec3 initColor;
		glm::vec3 endColor;
		float lifespan = 1.0f;
		float lifeRemaining = 0.0f;

	public:
		Particle();
		virtual void update(std::list<DrawableObject*>& objectsList);
		void setInitSize(float size);
		void setEndSize(float size);
		void setInitColor(glm::vec3 color);
		void setEndColor(glm::vec3 color);
		void setLifespan(float lifespan);
		void setLifeRemaining(float lifeRemaining);
		void decrementLife(float amount);
		float getLifeRemaining() const;
};