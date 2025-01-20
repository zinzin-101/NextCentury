#include "GameEngine.h"
#include "ParticleSystem.h"
#include "Random.h"
#include <thread>

ParticleSystem::ParticleSystem() {
	particlePool.resize(MAX_PARTICLE);
	poolIndex = MAX_PARTICLE - 1;
	midIndex = MAX_PARTICLE / 2;
}

void ParticleSystem::update(std::list<DrawableObject*>& objectsList) {
	//std::thread t1(&ParticleSystem::updateParticlePool, 0, midIndex, objectsList);
	//std::thread t2(&ParticleSystem::updateParticlePool, midIndex + 1, MAX_PARTICLE - 1, objectsList);
	//t1.join();
	//t2.join();
	updateParticlePool(0, MAX_PARTICLE - 1, objectsList);
}

void ParticleSystem::updateParticlePool(unsigned int startIndex, unsigned int endIndex, std::list<DrawableObject*>& objectsList) {
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	for (unsigned int i = startIndex; i <= endIndex; i++) {
		Particle& particle = particlePool[i];
		
		if (!particle.getIsActive())
			continue;

		if (particle.getLifeRemaining() <= 0.0f)
		{
			particle.setActive(false);
			continue;
		}

		particle.decrementLife(dt);
		particle.getTransform().rotateRad(0.01f * dt);
		particle.update(objectsList);
	}
}

void ParticleSystem::emit(const ParticleProperties& particleProperties) {
	Particle& particle = particlePool[poolIndex];
	particle.setActive(true);
	particle.getTransform().setPosition(particleProperties.position);
	particle.getTransform().setRotation(Random::Float() * 2.0f * glm::pi<float>());

	particle.getPhysicsComponent()->setVelocity(particleProperties.velocity);
	glm::vec2 variableVel = particle.getPhysicsComponent()->getVelocity();
	variableVel.x += particleProperties.velocityVariation.x * (Random::Float() - 0.5f);
	variableVel.y += particleProperties.velocityVariation.y * (Random::Float() - 0.5f);
	particle.getPhysicsComponent()->setVelocity(variableVel);

	particle.setColor(particleProperties.color);

	particle.setLifespan(particleProperties.lifespan);
	particle.setLifeRemaining(particleProperties.lifespan);
	particle.setInitSize(particleProperties.initSize);
	particle.setEndSize(particleProperties.endSize);

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