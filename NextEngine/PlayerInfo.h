#pragma once
#include <string>
#include "MovementInfo.h"

struct PlayerInfo {
	PlayerInfo() : name("Player"), health(10), movementInfo(5, 5), damage(10) {}
	PlayerInfo(std::string name, int health, MovementInfo movementInfo, int damage) :
		name(name), health(health), movementInfo(movementInfo), damage(damage) {
	}
	std::string name;
	int health;
	MovementInfo movementInfo;
	int damage;
};