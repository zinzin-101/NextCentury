#pragma once
#include "Collider.h"
#include "DrawableObject.h"

class RayObject : public DrawableObject {
	private:
		glm::vec3 origin;
		glm::vec3 dir;
		float length;

	public:
		RayObject(glm::vec3 pos, glm::vec3 dir, float length);
		void setRayOrigin(glm::vec3 pos);
		void setRayDirection(glm::vec3 dir);
		void setRayLength(float length);
		glm::vec3 getRayOrigin() const;
		glm::vec3 getRayDirection() const;
		float getRayLength() const;

		virtual void render(glm::mat4 globalModelTransform);

		virtual void onCollisionEnter(Collider* collider);
		virtual void onCollisionStay(Collider* collider);
		virtual void onCollisionExit(Collider* collider);
		virtual void onTriggerEnter(Collider* collider);
		virtual void onTriggerStay(Collider* collider);
		virtual void onTriggerExit(Collider* collider);
};