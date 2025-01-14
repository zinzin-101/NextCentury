#pragma once

struct MovementInfo {
	MovementInfo(float speed, float jumpVelocity) : speed(speed), jumpVelocity(jumpVelocity) {}
	MovementInfo(): speed(0.0f), jumpVelocity(0.0f) {}
	float speed;
	float jumpVelocity;
};