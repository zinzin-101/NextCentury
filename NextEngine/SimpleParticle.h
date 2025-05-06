#pragma once
#include "Particle.h"
#include "SimpleObject.h"
#include <list>

class SimpleParticle : public Particle, public SimpleObject {
	private:
		glm::vec3 initColor;
		glm::vec3 endColor;

	public:
		SimpleParticle();
		virtual void update(std::list<DrawableObject*>& objectsList);
		void setInitColor(glm::vec3 color);
		void setEndColor(glm::vec3 color);
};