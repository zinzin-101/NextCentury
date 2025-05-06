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
		int renderOrder;

	protected:
		string name;
		Transform transform;
		Physics* physics;
		Collider* collider;

		bool canDrawCollider;
		glm::vec3 drawColliderColor;

		float renderBrightness;
		float renderOpacity;

		void processCollider();
		virtual void onCollisionEnter(Collider* collider);
		virtual void onCollisionStay(Collider* collider);
		virtual void onCollisionExit(Collider* collider);

		virtual void onTriggerEnter(Collider* collider);
		virtual void onTriggerStay(Collider* collider);
		virtual void onTriggerExit(Collider* collider);

	public:
		string getName() const;
		void setName(string name);

		Transform& getTransform();
		glm::mat4 getTransformMat4() const;

		Physics* getPhysicsComponent() const;
		Collider* getColliderComponent() const;

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
		void setDrawColliderColor(glm::vec3 color);
		virtual void drawCollider();

		virtual void setActive(bool value);
		
		bool getIsActive() const;
		bool getCanDrawCollider() const;
		bool getMarkedForDelete() const;

		void setRenderBrightness(float brightness);
		virtual void setRenderOpacity(float opacity);
		void setRenderOrder(int renderOrder);
		int getRenderOrder() const;

		static void destroyObject(DrawableObject* obj);
};

