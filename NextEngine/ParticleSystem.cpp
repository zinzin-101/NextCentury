#include "GameEngine.h"
#include "ParticleSystem.h"
#include "Random.h"

ParticleSystem::ParticleSystem() {
	particlePool.resize(MAX_PARTICLE);
	poolIndex = MAX_PARTICLE - 1;

	canDestroyOnInactive = false;
}

void ParticleSystem::update(std::list<DrawableObject*>& objectsList) {
	updateParticlePool(0, MAX_PARTICLE - 1, objectsList);
}

void ParticleSystem::updateParticlePool(unsigned int startIndex, unsigned int endIndex, std::list<DrawableObject*>& objectsList) {
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	bool isSomeActive = false;

	for (unsigned int i = startIndex; i <= endIndex; i++) {
		Particle& particle = particlePool[i];
		
		if (!particle.getIsActive()) {
			continue;
		}

		if (particle.getLifeRemaining() <= 0.0f) {
			particle.setActive(false);
			continue;
		}

		isSomeActive = true;

		particle.decrementLife(dt);
		particle.getTransform().rotateRad(0.01f * dt);
		particle.update(objectsList);
	}

	if (!isSomeActive && canDestroyOnInactive) {
		destroyObject(this);
	}
}

void ParticleSystem::emit(const ParticleProperties& particleProperties) {
	Particle& particle = particlePool[poolIndex];
	particle.getTransform().setPosition(particleProperties.position);
	particle.getTransform().setRotation(Random::Float() * 2.0f * glm::pi<float>());

	particle.getPhysicsComponent()->setVelocity(particleProperties.velocity);
	glm::vec2 variableVel = particle.getPhysicsComponent()->getVelocity();
	variableVel.x += particleProperties.velocityVariation.x * (Random::Float() - 0.5f);
	variableVel.y += particleProperties.velocityVariation.y * (Random::Float() - 0.5f);
	particle.getPhysicsComponent()->setVelocity(variableVel);

	//particle.setColor(particleProperties.color);
	particle.setInitColor(particleProperties.initColor);
	particle.setEndColor(particleProperties.endColor);

	particle.setLifespan(particleProperties.lifespan);
	particle.setLifeRemaining(particleProperties.lifespan);
	particle.setInitSize(particleProperties.initSize);
	particle.setEndSize(particleProperties.endSize);

	particle.getPhysicsComponent()->setEnableGravity(particleProperties.isPhysics);

	particle.setActive(true);

	poolIndex = --poolIndex % MAX_PARTICLE;
}

void ParticleSystem::render(glm::mat4 globalModelTransform) {
	//DrawableObject::render(globalModelTransform);
	
	for (Particle& p : particlePool) {
		if (p.getIsActive()) {
			p.render(globalModelTransform);
		}
	}
}

void ParticleSystem::setDestroyOnInactive(bool value) {
	this->canDestroyOnInactive = value;
}