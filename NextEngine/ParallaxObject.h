#pragma once
#include "TexturedObject.h"
#include "PlayerObject.h"

class ParallaxObject : public TexturedObject {
private:
	//glm::vec3 camPos;
	//glm::vec3 playerPos;
	float z;
	bool vertical;
	glm::vec3 startPos;
	float nearPlane = 0.3f;
	float farPlane = 300.0f;

public:
	ParallaxObject(float z, bool vertical, PlayerObject* player);
	ParallaxObject(float x, float y, float z, bool vertical, PlayerObject* player);
	void setStartPos(glm::vec3 startPos);
	void setPlane(float nearPlane, float farPlane);
	virtual void update(list<DrawableObject*>& objectsList);
	void setTexture(string path);
	void initAnimation(int row, int column);
	void render(glm::mat4 globalModelTransform);
	Animation* getAnimationComponent();
	bool isAnimated();
	static PlayerObject* player;
	void setPlayer(PlayerObject* player);
};