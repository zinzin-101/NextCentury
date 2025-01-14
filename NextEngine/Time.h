#pragma once
#include <SDL.h>

class Time {
	private:
		uint64_t tickCounter;

		float timeScale;
		float deltaTime;

	public:
		Time();
		float getDeltaTime();
		float getDeltaTimeRealTime();
		uint64_t getTicks();
		void updateTick(uint64_t time);
		void setTimeScale(float timeScale);
};