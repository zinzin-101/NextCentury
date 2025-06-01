#pragma once
#include "ColliderObject.h"
#include "TexturedObject.h"
#include "PlayerObject.h"

namespace SonicWaveStat {
	constexpr int SONIC_WAVE_DAMAGE = 10;
	constexpr float ANIMATION_TIME_PER_FRAME = 0.1667f;
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
		bool used;

	public:
		SonicWave();
		virtual void onCollisionStay(Collider* collider);

		void setGroundOffset(float offset);

		void trigger();
		void mark(glm::vec3 position);
		void reset();
		
		virtual void update(std::list<DrawableObject*>& objectsList);
};