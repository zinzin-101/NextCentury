#include "SimpleParticle.h"

SimpleParticle::SimpleParticle(): Particle() {
	this->addPhysicsComponent();
	this->getPhysicsComponent()->setEnableGravity(false);
	initColor = glm::vec3();
	endColor = glm::vec3();
}

void SimpleParticle::update(std::list<DrawableObject*>& objectsList) {
	float interval = lifeRemaining / lifespan;
	float size = initSize * (1.0f - interval) + endSize * interval;
	this->getTransform().setScale(size);
	glm::vec3 color = endColor * (1.0f - interval) + initColor * interval; // had to switch init and end values for the color to be correct for some reason
	this->setColor(color);
	this->setRenderOpacity(glm::clamp(interval / 0.2f, 0.0f, 1.0f));

	SimpleObject::update(objectsList);
}

void SimpleParticle::setInitColor(glm::vec3 color) {
	this->initColor = color;
}
void SimpleParticle::setEndColor(glm::vec3 color) {
	this->endColor = color;
}