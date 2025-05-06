#include "TexturedParticle.h"

TexturedParticle::TexturedParticle(): Particle() {
	this->addPhysicsComponent();
	this->getPhysicsComponent()->setEnableGravity(false);
}

void TexturedParticle::update(std::list<DrawableObject*>& objectsList) {
	float interval = lifeRemaining / lifespan;
	float size = initSize * (1.0f - interval) + endSize * interval;
	this->getTransform().setScale(size);
	this->setRenderOpacity(glm::clamp(interval / 0.2f, 0.0f, 1.0f));

	TexturedObject::update(objectsList);
}