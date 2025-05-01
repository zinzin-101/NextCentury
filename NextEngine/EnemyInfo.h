#pragma once
#include <string>
#include "MovementInfo.h"

enum EnemyType {
	NORMAL,
	ZEALOT,
	BLIGHTFLAME,
	WAILER,
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

namespace DefaultEnemyStat {
	const EnemyInfo NORMAL_INFO = EnemyInfo();
	const EnemyInfo ZEALOT_INFO = EnemyInfo("Zealot", 40, MovementInfo(2.5f, 25), 30, 1.0f, 2.0f, 1.0f);
	const EnemyInfo BLIGHT_FLAME_INFO = EnemyInfo("BlightFlame", 60, MovementInfo(2, 2), 7.0f, 4.0f, 1.0f, 1);
	const EnemyInfo WAILER_INFO = EnemyInfo("Wailer", 50, MovementInfo(2.4f, 2), 10.0f, 4.0f, 1.0f, 1);

}