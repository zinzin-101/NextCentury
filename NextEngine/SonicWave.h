#pragma once
#include "ColliderObject.h"
#include "TexturedObject.h"
#include "PlayerObject.h"

namespace SonicWaveStat {
	constexpr int SONIC_WAVE_DAMAGE = 10;
}

class SonicWave : public TexturedObject {
	private:
		enum DamageStage {
			MARKING,
			DAMAGE,
			NONE
		};
		DamageStage currentStage;
		float groundOffset;
		int damageStartFrame;

	public:
		SonicWave();
		virtual void onCollisionEnter(Collider* collider);

		void setGroundOffset(float offset);

		void trigger();
		void mark(glm::vec3 position);
		void reset();
		
		virtual void update(std::list<DrawableObject*>& objectsList);
};