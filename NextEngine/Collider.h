#pragma once
#include "Transform.h"
#include <map>

class DrawableObject; // forward declaration

#define COLLISION_FLAG unsigned char
constexpr COLLISION_FLAG COLLISION_UP = 1;
constexpr COLLISION_FLAG COLLISION_DOWN = 2;
constexpr COLLISION_FLAG COLLISION_LEFT = 4;
constexpr COLLISION_FLAG COLLISION_RIGHT = 8;

class Collider {
	public:
		enum CollisionState;
	private:
		DrawableObject* object;
		bool enableCollision;
		bool trigger;
		std::map<Collider*, CollisionState> collisionMap;
		COLLISION_FLAG collisionFlag;

		Transform transform;
		float width, height;

	public:
		enum CollisionState {
			ENTER,
			STAY,
			EXIT,
			NONE
		};

		Collider(DrawableObject* object);
		Collider(DrawableObject* object, float width, float height);
		void setEnableCollision(bool value);
		void setTrigger(bool value);
		void setTransform(const Transform& transform);
		void setWidth(float width);
		void setHeight(float height);
		void setDimension(float width, float height);
		void setCollisionFlag(COLLISION_FLAG flag);

		Transform& getTransform();
		Transform getGlobalTransform(Transform& selfTransform);
		float getWidth() const;
		float getHeight() const;
		std::map<Collider*, CollisionState>& getCollisionMap();
		CollisionState getCollisionState(Collider* other);
		bool isEnable() const;
		bool isTrigger() const;
		COLLISION_FLAG getCollisionFlag() const;

		DrawableObject* getObject();
};