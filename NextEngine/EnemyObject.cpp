#include "EnemyObject.h"
#include "GameEngine.h"
#include "PlayerObject.h"
#include "ProjectileObject.h"
//#include "ParticleSystem.h" // temporary testing
#include <iostream>

EnemyObject::EnemyObject(EnemyInfo& enemyInfo) : LivingEntity(enemyInfo.name, enemyInfo.health) {
	this->movementInfo = enemyInfo.movementInfo;
	this->aggroRange = enemyInfo.health;
	this->attackRange = enemyInfo.attackRange;
	this->attackCooldown = enemyInfo.attackCooldown;
	this->damage = enemyInfo.damage;
	currentState = IDLE;

	//setTexture("../Resource/Texture/incineratorSizeFlip.png");
	setTexture("../Resource/Texture/PlaceHolder.png");
	//initAnimation(6, 2);
	initAnimation(9, 2);
	addColliderComponent();
	addPhysicsComponent();
	targetEntity = nullptr;
	//getAnimationComponent()->addState("Idle", 0, 6);
	//getAnimationComponent()->addState("Moving", 1, 5);
	//getAnimationComponent()->addState("Attacking", 1, 5);
	getAnimationComponent()->addState("Idle", 1, 1, true);
	getAnimationComponent()->addState("Moving", 1, 3, true);
	getAnimationComponent()->addState("Attacking", 0, 3, false);
	getAnimationComponent()->setState("Idle");

	//attackHitbox = new SimpleObject();
	//attackHitbox->setColor(1.0f, 0.0f, 0.0f); // Red color for debugging
	//attackHitbox->getTransform().setScale(glm::vec3(1.0f, 1.0f, 0.0f)); // Adjust size as needed
	//attackHitbox->addColliderComponent();
	//attackHitbox->getColliderComponent()->setTrigger(true); // Hitbox should be a trigger

	/*deactivateHitbox();*/
	attackHitbox = nullptr;
	hitboxActiveTime = 0.5f;
	attackingCooldownTimer = 0.0f;
	canAttack = true;

	/// Test ///
	isAttacking = false;
	attackFrameStart = 1;
	attackFrameEnd = 1;
}

EnemyObject::~EnemyObject() {
	if (attackHitbox != nullptr) {
		destroyObject(attackHitbox);
	}
}

//void EnemyObject::activateHitbox() {
//	//hitboxTimer = hitboxActiveTime;
//}
//
//void EnemyObject::deactivateHitbox() {
//}
//
//bool EnemyObject::isHitboxActive() const { 
//	//return hitboxTimer >= 0.0f; 
//}

void EnemyObject::setCurrentState(State state) {
	currentState = state;
}

EnemyObject::State EnemyObject::getCurrentState() {
	return currentState;
}

bool EnemyObject::isPlayerInAttackRange(glm::vec3 targetPos) {
	float distance = glm::length(targetPos - this->transform.getPosition());
	if (distance <= attackRange) {
		return true;
	}
	return false;
}

bool EnemyObject::isPlayerInAggroRange(glm::vec3 targetPos) {
	float distance = glm::length(targetPos - this->transform.getPosition());
	if (distance <= aggroRange) {
		return true;
	}
	return false;
}

void EnemyObject::setMovementInfo(MovementInfo movementInfo) {
	this->movementInfo = movementInfo;
}

void EnemyObject::setAggroRange(float range) {
	aggroRange = range;
}

void EnemyObject::setAttackRange(float range) {
	attackRange = range;
}

void EnemyObject::setDamage(int damage) {
	this->damage = damage;
	attackHitbox->setDamage(this->damage);
}

float EnemyObject::getAggroRange() const {
	return aggroRange;
}

float EnemyObject::getAttackRange() const {
	return attackRange;
}

void EnemyObject::setCanAttack(bool value) {
	canAttack = value;
}

void EnemyObject::attack() {
	attackHitbox->trigger(transform.getPosition());
	attackHitbox->setCanDecreaseTime(false);

	attackCooldown = 1.0f;
	std::cout << "Enemy attacked!" << std::endl;
}

void EnemyObject::setAttackCooldown(float cooldown) {
	attackCooldown = cooldown;
}

float EnemyObject::getAttackCooldown() const {
	return attackCooldown;
}

void EnemyObject::moveTowards(glm::vec3 targetPos) {
	glm::vec3 currentPos = this->transform.getPosition();

	if (glm::length(currentPos - targetPos) > aggroRange) {
		glm::vec2 vel = physics->getVelocity();
		this->physics->setVelocity(glm::vec2(0, vel.y));
		return;
	}

	glm::vec2 newVelocity = this->physics->getVelocity();

	bool grounded = this->collider->getCollisionFlag() & COLLISION_DOWN;
	if (targetPos.y > currentPos.y + this->collider->getHeight() * 0.5f && grounded) {
		newVelocity.y = movementInfo.jumpVelocity;
		//cout << "jump" << endl;
	}

	if (glm::length(currentPos - targetPos) <= attackRange) {
		glm::vec2 vel = physics->getVelocity();
		this->physics->setVelocity(glm::vec2(0, vel.y));
		return;
	}

	if (targetPos.x < currentPos.x + aggroRange &&
		targetPos.x > currentPos.x - aggroRange) {
		float moveAmount = targetPos.x - currentPos.x;
		moveAmount = moveAmount > 0 ? 1 : -1;
		moveAmount *= movementInfo.speed;
		newVelocity.x = moveAmount;
		getAnimationComponent()->setState("Moving");
	}
	else {
		newVelocity.x = 0.0f;
		getAnimationComponent()->setState("Idle");
	}

	this->physics->setVelocity(newVelocity);

	glm::vec2 vel = this->physics->getVelocity();
	//cout << "x: " << vel.x << " y: " << vel.y << endl;
	//cout << boolalpha;
	//cout << "grounded: " << grounded << endl;
}

void EnemyObject::findTarget(std::string name, std::list<DrawableObject*>& objectList) {
	for (DrawableObject* obj : objectList) {
		if (obj->getName() == name) {
			LivingEntity* entity = dynamic_cast<LivingEntity*>(obj);
			if (entity != NULL) {
				this->targetEntity = entity;
			}
			break;
		}
	}
}

void EnemyObject::setTarget(LivingEntity* target) {
	this->targetEntity = target;
}

void EnemyObject::start(list<DrawableObject*>& objectsList) {
	attackHitbox = new DamageCollider<PlayerObject>(this, damage, hitboxActiveTime);
	attackHitbox->setActive(false);
	attackHitbox->setFollowOwner(true);
	attackHitbox->setFollowOffset(glm::vec3(0.5f, 0, 0));
	attackHitbox->getColliderComponent()->setWidth(1.5f);
	objectsList.emplace_back(attackHitbox);
}

void EnemyObject::updateState(glm::vec3 targetPos) {

	if (isAttacking) {
		currentState = ATTACKING;
		return;
	}

	if (isPlayerInAttackRange(targetPos) && attackingCooldownTimer <= 0.0f && canAttack) {
		currentState = ATTACKING;
	}
	else if (isPlayerInAggroRange(targetPos)) {
		currentState = AGGRO;
	}
	else if (this->getIsStun()) {
		currentState = STUNNED;
	}
	else {
		currentState = IDLE;
	}
}

void EnemyObject::updateBehavior(list<DrawableObject*>& objectsList) {
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	glm::vec3 targetPos = transform.getPosition();
	if (targetEntity != nullptr) {
		targetPos = targetEntity->getTransform().getPosition();

		glm::vec3 currentPos = this->transform.getPosition();

		isFacingRight = targetPos.x >= currentPos.x;

		moveTowards(targetPos);
		updateState(targetPos);
	}
	// debug
	Animation::State animState = getAnimationComponent()->getCurrentAnimationState();
	std::cout << "current enemy state: " << currentState << std::endl;
	std::cout << "current state: " << animState.name << std::endl;
	std::cout << "is playing: " << getAnimationComponent()->getIsPlaying() << std::endl;

	// Manage attack and hitbox activation
	if (currentState == ATTACKING) {
		if (attackingCooldownTimer <= 0.0f) {
			if (canAttack) {
				attackingCooldownTimer = attackCooldown;
				if (isAttacking) {

					if (getAnimationComponent()->getCurrentFrame() == attackFrameStart) {
						attack();
					}

					if (getAnimationComponent()->getCurrentFrame() > attackFrameEnd) {
						attackHitbox->setActive(false);
					}

					if (!getAnimationComponent()->getIsPlaying()) {
						isAttacking = false;
						getAnimationComponent()->setState("Idle");
					}
				}
				else {
					isAttacking = true;

					this->getAnimationComponent()->setState("Attacking");
				}
			}
			//activateHitbox();
			
			//std::cout << "Enemy attacks!" << std::endl;

			// Deal damage to the player if within attack range
			//if (targetEntity != nullptr && canAttack) {
			//	if (isPlayerInAttackRange(targetEntity->getTransform().getPosition())) {
			//		//player->takeDamage(10); // Subtract 10 health points
			//		//std::cout << player->getName() << " took 10 damage!" << std::endl;
			//		
			//		//attack();
			//		std::cout << this->getName() << " attacked" << std::endl;
			//	}
			//	else {
			//		Transform targetTranform = targetEntity->getTransform();
			//		glm::vec3 targetPos = targetTranform.getPosition();

			//		//glm::vec2 velocity((targetPos.x > this->getTransform().getPosition().x) ? 5 : -5, 0);
			//		//ProjectileObject<PlayerObject>* projectile = new ProjectileObject<PlayerObject>(this, damage, this->getTransform().getPosition(), velocity, 5);
			//		//objectsList.emplace_back(projectile);

			//		/// test ///
			//		//ParticleSystem* ps = projectile->getEmitter();
			//		//objectsList.emplace_back(ps);
			//	}
			//}
		}
	}
	else if (currentState == AGGRO) {
		getAnimationComponent()->setState("Moving");
	}
	else if (currentState == IDLE) {
		getAnimationComponent()->setState("Idle");
	}

	if (!isAttacking) {
		attackingCooldownTimer -= dt;
	}

	// Handle hitbox timer
	/*if (hitboxTimer > 0.0f) {
		hitboxTimer -= dt;
		if (hitboxTimer <= 0.0f) {
			deactivateHitbox();
		}
	}*/

	// Other state handling...
	if (currentState == STUNNED) {
		std::cout << "Enemy stunned!" << std::endl;
	}
	/*else if (currentState == IDLE) {
		this->getAnimationComponent()->setState("Idle");
	}*/
}