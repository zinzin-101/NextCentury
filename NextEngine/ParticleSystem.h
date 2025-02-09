#pragma once
#include "DrawableObject.h"
#include "Particle.h"
#include "ParticleProperties.h"
#include <list>
#include <vector>

constexpr int MAX_PARTICLE = 1000;

class ParticleSystem : virtual public DrawableObject {
	private:
		std::vector<Particle> particlePool;
		unsigned int poolIndex;
		unsigned int midIndex;
		void updateParticlePool(unsigned int startIndex, unsigned int endIndex, std::list<DrawableObject*>& objectsList);

	public:
		ParticleSystem();
		virtual void update(std::list<DrawableObject*>& objectsList);
		void emit(const ParticleProperties& particleProperties);
		virtual void render(glm::mat4 globalModelTransform);
};