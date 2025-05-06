#pragma once
#include "DrawableObject.h"
#include "SimpleParticle.h"
#include "TexturedParticle.h"
#include "ParticleProperties.h"
#include <list>
#include <vector>

constexpr int MAX_PARTICLE = 1000;

class ParticleSystem : virtual public DrawableObject {
	private:
		std::vector<SimpleParticle> simpleParticlePool;
		std::vector<TexturedParticle> texturedParticlePool;
		unsigned int simplePoolIndex;
		unsigned int texturedPoolIndex;

		template <class ParticleType>
		static bool updateParticlePool(std::vector<ParticleType>& particlePool, std::list<DrawableObject*>& objectsList);
		
		void emitSimpleParticle(const ParticleProperties& particleProperties);
		void emitTexturedParticle(const ParticleProperties& particleProperties);

		bool canDestroyOnInactive;

	public:
		ParticleSystem();
		virtual void update(std::list<DrawableObject*>& objectsList);
		void emit(const ParticleProperties& particleProperties);
		virtual void render(glm::mat4 globalModelTransform);

		void setDestroyOnInactive(bool value);
};