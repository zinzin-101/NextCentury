#pragma once
#include "gtc/matrix_transform.hpp"

struct ParticleProperties {
	ParticleProperties() {}
	ParticleProperties(glm::vec3 position, glm::vec2 velocity, glm::vec2 velocityVariation,
		glm::vec3 color, float initSize, float endSize, float sizeVariation, float lifespan = 1.0f):
		position(position), velocity(velocity), velocityVariation(velocityVariation), initColor(color), endColor(color),
		initSize(initSize), endSize(endSize), sizeVariation(sizeVariation), lifespan(lifespan) {}
	ParticleProperties(glm::vec3 position, glm::vec2 velocity, glm::vec2 velocityVariation,
		glm::vec3 initColor, glm::vec3 endColor, float initSize, float endSize, float sizeVariation, float lifespan = 1.0f) :
		position(position), velocity(velocity), velocityVariation(velocityVariation), initColor(initColor), endColor(endColor),
		initSize(initSize), endSize(endSize), sizeVariation(sizeVariation), lifespan(lifespan) {
	}

	glm::vec3 position;
	glm::vec2 velocity;
	glm::vec2 velocityVariation;
	glm::vec3 initColor;
	glm::vec3 endColor;
	float initSize;
	float endSize;
	float sizeVariation;
	float lifespan;
};