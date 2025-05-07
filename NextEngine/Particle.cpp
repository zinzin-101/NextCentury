#include "Particle.h"

Particle::Particle() {
	lifespan = 1.0f;
	lifeRemaining = 0.0f;
	initSize = 1.0f;
	endSize = 1.0f;
}

void Particle::setInitSize(float size) {
	this->initSize = size;
}
void Particle::setEndSize(float size) {
	this->endSize = size;
}
void Particle::setLifespan(float lifespan) {
	this->lifespan = lifespan;
}
void Particle::setLifeRemaining(float lifeRemaining) {
	this->lifeRemaining = lifeRemaining;
}
void Particle::decrementLife(float amount) {
	this->lifeRemaining -= amount;
}

float Particle::getLifeRemaining() const {
	return this->lifeRemaining;
}