#pragma once
#include "glm.hpp"
#include "Collider.h"
#include "Physics.h"
#include "Transform.h"
#include <string>
#include <list>

using namespace std;

class Collider; // forward declaration

class DrawableObject {
	private:
		bool isActive;
		bool isMarkedForDelete;

	protected:
		string name;
		Transform transform;
		Physics* physics;
		Collider* collider;

		bool canDrawCollider;

		float textureBrightness;

		void processCollider();
		virtual void onCollisionEnter(Collider* collider);
		virtual void onCollisionStay(Collider* collider);
		virtual void onCollisionExit(Collider* collider);

		virtual void onTriggerEnter(Collider* collider);
		virtual void onTriggerStay(Collider* collider);
		virtual void onTriggerExit(Collider* collider);

	public:
		string getName();
		void setName(string name);

		Transform& getTransform();
		glm::mat4 getTransformMat4();

		Physics* getPhysicsComponent();
		Collider* getColliderComponent();

		void addPhysicsComponent();
		void addPhysicsComponent(Physics& physics);
		void addColliderComponent();
		void addColliderComponent(Collider& collider);

		//virtual void updateBehavior();
		virtual void start(list<DrawableObject*>& objectsList);
		virtual void update(list<DrawableObject*>& objectsList);

		DrawableObject();
		DrawableObject(string name);
		virtual ~DrawableObject();
		virtual void render(glm::mat4 globalModelTransform) = 0;
		void setDrawCollider(bool value);
		virtual void drawCollider();

		void setActive(bool value);
		
		bool getIsActive();
		bool getMarkedForDelete();

		void setBrightness(float brightness);

		static void destroyObject(DrawableObject* obj);
};

