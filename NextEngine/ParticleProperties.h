#pragma once
#include "gtc/matrix_transform.hpp"

struct ParticleProperties {
	ParticleProperties() {}
	ParticleProperties(glm::vec3 position, glm::vec2 velocity, glm::vec2 velocityVariation,
		glm::vec3 color, float initSize, float endSize, float sizeVariation, float lifespan = 1.0f):
		position(position), velocity(velocity), velocityVariation(velocityVariation), color(color), initSize(initSize),
		endSize(endSize), sizeVariation(sizeVariation), lifespan(lifespan) {}

	glm::vec3 position;
	glm::vec2 velocity;
	glm::vec2 velocityVariation;
	glm::vec3 color;
	float initSize;
	float endSize;
	float sizeVariation;
	float lifespan;
};