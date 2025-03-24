#pragma once
#include "DrawableObject.h"

namespace LightSourceData {
	constexpr float MINIMUM_BRIGHTNESS = 0.2f;
}

class LightSource : public DrawableObject {
	private:
		float brightness;
		float maxDistance;

	public:
		LightSource(float brightness, float maxDistance);
		float getTextureBrightness(DrawableObject* obj);

		virtual void render(glm::mat4 globalModelTransform);

		void setBrightness(float brightness);
		void setMaxDistance(float distance);

		float getBrightness() const;
		float getMaxDistance() const;

		static float normalizeBrightness(float brightness);
};