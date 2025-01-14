#pragma once
#include "DrawableObject.h"
#include "ParticleObject.h"

class ParticleSystem : public DrawableObject {
	private:
		ParticleObject particle;
};