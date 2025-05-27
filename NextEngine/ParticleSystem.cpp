#include "GameEngine.h"
#include "ParticleSystem.h"
#include "Random.h"

ParticleSystem::ParticleSystem() {
	simpleParticlePool.resize(MAX_PARTICLE);
	simplePoolIndex = MAX_PARTICLE - 1;

	texturedParticlePool.resize(MAX_PARTICLE);
	texturedPoolIndex = MAX_PARTICLE - 1;

	canDestroyOnInactive = false;
}

void ParticleSystem::update(std::list<DrawableObject*>& objectsList) {
	bool simpleParticleActive = updateParticlePool<SimpleParticle>(this->simpleParticlePool, objectsList);
	bool texturedParticleActive = updateParticlePool<TexturedParticle>(this->texturedParticlePool, objectsList);

	if (!(simpleParticleActive || texturedParticleActive) && canDestroyOnInactive) {
		destroyObject(this);
	}
}

template <class ParticleType>
bool ParticleSystem::updateParticlePool(std::vector<ParticleType>& particlePool, std::list<DrawableObject*>& objectsList) {
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	bool isSomeActive = false;

	for (ParticleType& particle : particlePool) {		
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

	return isSomeActive;
}

void ParticleSystem::emitSimpleParticle(const ParticleProperties& particleProperties) {
	SimpleParticle& particle = simpleParticlePool[simplePoolIndex];

	particle.getTransform().setPosition(particleProperties.position);
	particle.getTransform().setRotation(Random::Float() * 2.0f * glm::pi<float>());

	particle.getPhysicsComponent()->setVelocity(particleProperties.velocity);
	glm::vec2 variableVel = particle.getPhysicsComponent()->getVelocity();
	variableVel.x += particleProperties.velocityVariation.x * (Random::Float() - 0.5f);
	variableVel.y += particleProperties.velocityVariation.y * (Random::Float() - 0.5f);
	particle.getPhysicsComponent()->setVelocity(variableVel);

	particle.setInitColor(particleProperties.initColor);
	particle.setEndColor(particleProperties.endColor);

	particle.setLifespan(particleProperties.lifespan);
	particle.setLifeRemaining(particleProperties.lifespan);
	particle.setInitSize(particleProperties.initSize);
	particle.setEndSize(particleProperties.endSize);

	particle.getPhysicsComponent()->setEnableGravity(particleProperties.isPhysics);
	particle.getPhysicsComponent()->setRealTime(particleProperties.isRealTime);

	particle.setActive(true);

	simplePoolIndex = --simplePoolIndex % MAX_PARTICLE;
}
void ParticleSystem::emitTexturedParticle(const ParticleProperties& particleProperties) {
	TexturedParticle& particle = texturedParticlePool[texturedPoolIndex];

	particle.getTransform().setPosition(particleProperties.position);
	particle.getTransform().setRotation(Random::Float() * 2.0f * glm::pi<float>());

	particle.getPhysicsComponent()->setVelocity(particleProperties.velocity);
	glm::vec2 variableVel = particle.getPhysicsComponent()->getVelocity();
	variableVel.x += particleProperties.velocityVariation.x * (Random::Float() - 0.5f);
	variableVel.y += particleProperties.velocityVariation.y * (Random::Float() - 0.5f);
	particle.getPhysicsComponent()->setVelocity(variableVel);

	particle.setTexture(particleProperties.texturePath);

	particle.setLifespan(particleProperties.lifespan);
	particle.setLifeRemaining(particleProperties.lifespan);
	particle.setInitSize(particleProperties.initSize);
	particle.setEndSize(particleProperties.endSize);

	particle.getPhysicsComponent()->setEnableGravity(particleProperties.isPhysics);
	particle.getPhysicsComponent()->setRealTime(particleProperties.isRealTime);

	particle.setActive(true);

	texturedPoolIndex = --texturedPoolIndex % MAX_PARTICLE;
}

void ParticleSystem::emit(const ParticleProperties& particleProperties) {
	particleProperties.texturePath.empty() ? emitSimpleParticle(particleProperties) : emitTexturedParticle(particleProperties);
}

void ParticleSystem::render(glm::mat4 globalModelTransform) {
	//DrawableObject::render(globalModelTransform);
	
	for (SimpleParticle& p : simpleParticlePool) {
		if (p.getIsActive()) {
			p.render(globalModelTransform);
		}
	}

	for (TexturedParticle& p : texturedParticlePool) {
		if (p.getIsActive()) {
			p.render(globalModelTransform);
		}
	}
}

void ParticleSystem::setDestroyOnInactive(bool value) {
	this->canDestroyOnInactive = value;
}

void ParticleSystem::recenterParticles() {
	for (SimpleParticle& p : simpleParticlePool) {
		if (p.getIsActive()) {
			glm::vec3 newPos = p.getTransform().getPosition() - this->getTransform().getPosition();
			p.getTransform().setPosition(newPos);
		}
	}

	for (TexturedParticle& p : texturedParticlePool) {
		if (p.getIsActive()) {
			glm::vec3 newPos = p.getTransform().getPosition() - this->getTransform().getPosition();
			p.getTransform().setPosition(newPos);
		}
	}
}

void ParticleSystem::clearParticles() {
	for (SimpleParticle& p : simpleParticlePool) {
		if (p.getIsActive()) {
			p.setActive(false);
		}
	}

	for (TexturedParticle& p : texturedParticlePool) {
		if (p.getIsActive()) {
			p.setActive(false);
		}
	}
}