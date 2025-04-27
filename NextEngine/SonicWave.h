#pragma once
#include "ColliderObject.h"
#include "TexturedObject.h"
#include "PlayerObject.h"

class SonicWave : public TexturedObject {
	private:
		bool initiated;
		glm::vec3 currentPosition;
		ColliderObject* groundMarker;

	public:
		SonicWave();
		virtual void onCollisionEnter(Collider* collider);
		void trigger(glm::vec3 position);
		
		virtual void start(std::list<DrawableObject*>& objectsList);
		virtual void update(std::list<DrawableObject*>& objectsList);
};