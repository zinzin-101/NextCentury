#include "Time.h"



Time::Time() : tickCounter(0), timeScale(1.0f) {}

void Time::updateTick(uint64_t time) {
	//uint64_t endCounter = SDL_GetTicks();
	uint64_t elapsedTime = time - tickCounter;
	//deltaTime = (float)elapsedTime / 1000.0f;
	deltaTime = elapsedTime * 0.001f; // to  make it seconds
	tickCounter = time;
}

void Time::updateTickCounterOnly(uint64_t time) {
	deltaTime = 0.0f;
	tickCounter = time;
}

float Time::getDeltaTime() const {
	return this->deltaTime * this->timeScale;
}

float Time::getDeltaTimeRealTime() const {
	return this->deltaTime;
}

float Time::getTimeScale() const {
	return timeScale;
}

void Time::setTimeScale(float timeScale) {
	this->timeScale = timeScale;
}

uint64_t Time::getTicks() {
	return this->tickCounter;
}