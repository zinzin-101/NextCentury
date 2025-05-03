#pragma once

//#include "CollisionHandler.h"
#include "glm.hpp"
#include "Transform.h"
#include <vector>

constexpr float DEFAULT_GRAVITY = -98.1f; // fine tune later
constexpr float CONST_DELTA_TIME = 0.001f; // for testing

using namespace std;

class Physics {
	private:
		glm::vec3 lastPos;
		glm::vec2 velocity;
		glm::vec2 acceleration;
		glm::vec2 gravity;
		float mass;
		glm::vec2 force;
		float drag;
		bool enablePhysics;
		bool enableGravity;
		bool isRealTime;

	public:
		Physics(Transform& transform);
		void setVelocity(glm::vec2 vel);
		void setAcceleration(glm::vec2 accel);
		void setGravity(glm::vec2 g);
		void setMass(float m);
		void addForce(glm::vec2 f);
		void addVelocity(glm::vec2 vel);
		void setDrag(float drag);
		void applyGravity();
		void update(Transform& transform);

		glm::vec3 getLastPosition();
		glm::vec2 getVelocity();
		glm::vec2 getAcceleration();
		glm::vec2 getGravity();
		float getMass();
		glm::vec2 getForce();
		float getDrag();

		void setLastPosition(glm::vec3 pos);
		void setEnablePhysics(bool flag);
		void setEnableGravity(bool flag);

		bool isGravityEnable() const;
		bool isPhysicsEnable() const;

		void setRealTime(bool value);

		//void operator=(Physics& other);
};