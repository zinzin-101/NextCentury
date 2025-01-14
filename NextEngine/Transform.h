#pragma once
#include "glm.hpp"

class Transform {
	private:
		glm::vec3 position;
		float rotation;
		glm::vec3 scale;
	public:
		Transform();
		Transform(glm::vec3 position, float rotation, glm::vec3 scale);
		void setPosition(glm::vec3 position);
		void setPosition(float x, float y);
		void setRotationRad(float rotation);
		void setRotationDeg(float rotation);
		void setRotation(float angle);
		void setScale(glm::vec3 scale);
		void setScale(float scale);
		void setScale(float scaleX, float scaleY);
		void scales(glm::vec3 scale);
		void scales(float scale);
		void scales(float scaleX, float scaleY);
		void translate(glm::vec3 translate);
		void translate(float x, float y);
		void rotateRad(float rad);
		void rotateDeg(float deg);
		void rotate(float angle);
		Transform getGlobalTransform(Transform& objectTransform);
		glm::vec3 getPosition();
		float getRotationRad();
		float getRotationDeg();
		float getRotation();
		glm::vec3 getScale();
		glm::mat4 getTransformMat4();
		
		Transform operator+(const Transform& other);
		Transform operator-(const Transform& other);
		Transform operator*(const float& multiplier);
};