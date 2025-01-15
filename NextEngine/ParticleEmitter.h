#pragma once
#include "DrawableObject.h"
#include "ParticleObject.h"

class ParticleEmitter : public DrawableObject {
	private:
		ParticleObject particle;

	public:
		ParticleEmitter(ParticleObject particle);
};