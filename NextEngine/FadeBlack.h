#pragma once
#include "SimpleObject.h"

class FadeBlack : public SimpleObject {
	float targetOpacity;
	float elapsedTime = 0.0f;
	bool fadingIn = false;
	bool fadingOut = false;
	bool followCam = true;

public:
	float timeToFade;
	FadeBlack(float time);
	void FadeToBlack();
	void FadeToTransparent();
	void update(list<DrawableObject*>& objectsList); // Call this every frame
	void setFollowCam(bool f);
};
