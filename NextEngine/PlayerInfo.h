#pragma once
#include <string>
#include "MovementInfo.h"

struct PlayerInfo {
	PlayerInfo() : name("Player"), health(10), movementInfo(5, 5) {}
	PlayerInfo(std::string name, int health, MovementInfo movementInfo) :
		name(name), health(health), movementInfo(movementInfo) {
	}
	std::string name;
	int health;
	MovementInfo movementInfo;
};