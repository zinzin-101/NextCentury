#pragma once
#include <string>
#include "MovementInfo.h"

enum EnemyType {
	NORMAL,
	ZEALOT,
	SPECIAL
};

struct EnemyInfo {
	EnemyInfo(): name("Enemy"), health(10), movementInfo(5, 5), aggroRange(5.0f), attackRange(2.0f), attackCooldown(2.0f), damage(1) {}
	EnemyInfo(std::string name, int health, MovementInfo movementInfo, float aggroRange, float attackRange, float attackCooldown, int damage):
	name(name), health(health), movementInfo(movementInfo), aggroRange(aggroRange), attackRange(attackRange), attackCooldown(attackCooldown), damage(damage) {}
	std::string name;
	int health;
	MovementInfo movementInfo;
	float aggroRange;
	float attackRange;
	float attackCooldown;
	int damage;
};