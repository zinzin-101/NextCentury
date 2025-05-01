#include "Particle.h"

Particle::Particle() {
	this->addPhysicsComponent();
	this->getPhysicsComponent()->setEnableGravity(false);
	lifespan = 1.0f;
	lifeRemaining = 0.0f;
	initSize = 1.0f;
	endSize = 1.0f;
	initColor = glm::vec3();
	endColor = glm::vec3();
}

void Particle::update(std::list<DrawableObject*>& objectsList) {
	float interval = lifeRemaining / lifespan;
	float size = initSize * (1.0f - interval) + endSize * interval;
	this->getTransform().setScale(size);
	glm::vec3 color = endColor * (1.0f - interval) + initColor * interval; // had to switch init and end values for the color to be correct for some reason
	this->setColor(color);
	this->setRenderOpacity(glm::clamp(interval / 0.2f, 0.0f, 1.0f));

	SimpleObject::update(objectsList);
}

void Particle::setInitSize(float size) {
	this->initSize = size;
}
void Particle::setEndSize(float size){
	this->endSize = size;
}
void Particle::setInitColor(glm::vec3 color) {
	this->initColor = color;
}
void Particle::setEndColor(glm::vec3 color) {
	this->endColor = color;
}
void Particle::setLifespan(float lifespan){
	this->lifespan = lifespan;
}
void Particle::setLifeRemaining(float lifeRemaining) {
	this->lifeRemaining = lifeRemaining;
}
void Particle::decrementLife(float amount){
	this->lifeRemaining -= amount;
}

float Particle::getLifeRemaining() const {
	return this->lifeRemaining;
}