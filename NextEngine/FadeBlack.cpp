#include "FadeBlack.h"
#include "GameEngine.h"

FadeBlack::FadeBlack(float time) {
	timeToFade = time;
	targetOpacity = 0.0f;
	this->setRenderOpacity(targetOpacity);
	this->setColor(0.0f, 0.0f, 0.0f);
	this->setRenderOrder(3);
	this->getTransform().setScale(18.0f, 11.0f);
}

void FadeBlack::FadeToBlack() {
	fadingIn = true;
	fadingOut = false;
	elapsedTime = 0.0f;
}

void FadeBlack::FadeToTransparent() {
	fadingIn = false;
	fadingOut = true;
	elapsedTime = 0.0f;
}

void FadeBlack::update(list<DrawableObject*>& objectsList) {
	this->getTransform().setPosition(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition());

	float deltaTime = GameEngine::getInstance()->getTime()->getDeltaTimeRealTime();
	if (fadingIn) {
		elapsedTime += deltaTime;
		float t = std::min(elapsedTime / timeToFade, 1.0f);
		this->setRenderOpacity(t);
		if (t >= 1.0f) fadingIn = false;
	}
	else if (fadingOut) {
		elapsedTime += deltaTime;
		float t = std::min(elapsedTime / timeToFade, 1.0f);
		this->setRenderOpacity(1.0f - t);
		if (t >= 1.0f) fadingOut = false;
	}
	cout << this->getRenderOpacity() << endl;
}
