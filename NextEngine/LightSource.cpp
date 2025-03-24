#include "LightSource.h"

LightSource::LightSource(float brightness, float maxDistance): brightness(brightness), maxDistance(maxDistance) {}

float LightSource::getTextureBrightness(DrawableObject* obj) {
	float distance = glm::length(this->getTransform().getPosition() - obj->getTransform().getPosition());

	if (distance >= maxDistance) {
		return LightSourceData::MINIMUM_BRIGHTNESS;
	}

	float resultBrightness = (1.0f - (distance / maxDistance)) * this->brightness;
	
	if (resultBrightness <= LightSourceData::MINIMUM_BRIGHTNESS) {
		return LightSourceData::MINIMUM_BRIGHTNESS;
	}

	return resultBrightness;
}

float LightSource::normalizeBrightness(float brightness) {
	if (brightness <= LightSourceData::MINIMUM_BRIGHTNESS) {
		return LightSourceData::MINIMUM_BRIGHTNESS;
	}

	if (brightness > 1.0f) {
		return 1.0f;
	}

	return brightness;
}

void LightSource::render(glm::mat4 globalModelTransform) {
	return;
}