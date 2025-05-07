#pragma once
#include "Particle.h"
#include "TexturedObject.h"

class TexturedParticle : public Particle, public TexturedObject {
public:
	TexturedParticle();
	virtual void update(std::list<DrawableObject*>& objectsList);
};