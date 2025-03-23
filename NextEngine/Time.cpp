#include "Time.h"



Time::Time() : tickCounter(0), timeScale(1.0f) {
	updateTick(SDL_GetTicks());
}

void Time::updateTick(uint64_t time) {
	//uint64_t endCounter = SDL_GetTicks();
	uint64_t elapsedTime = time - tickCounter;
	//deltaTime = (float)elapsedTime / 1000.0f;
	deltaTime = elapsedTime * 0.001f; // to  make it seconds
	tickCounter = time;
}

void Time::updateTick() {
	deltaTime = 0.0f;
}

float Time::getDeltaTime() {
	return this->deltaTime * this->timeScale;
}

float Time::getDeltaTimeRealTime() {
	return this->deltaTime;
}

void Time::setTimeScale(float timeScale) {
	this->timeScale = timeScale;
}

uint64_t Time::getTicks() {
	return this->tickCounter;
}