#include "DamageCollider.h"
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
	initAnimation(2, 9);
	addColliderComponent();
	addPhysicsComponent();
	targetEntity = nullptr;
	//getAnimationComponent()->addState("Idle", 0, 6);
	//getAnimationComponent()->addState("Moving", 1, 5);
	//getAnimationComponent()->addState("Attacking", 1, 5);
	getAnimationComponent()->addState("Idle", 1, 6, 3, true);
	getAnimationComponent()->addState("Moving", 1, 0, 3, true);
	getAnimationComponent()->addState("Attacking", 0, 0, 9, false);
	getAnimationComponent()->setState("Idle");

	//attackHitbox = new SimpleObject();
	//attackHitbox->setColor(1.0f, 0.0f, 0.0f); // Red color for debugging
	//attackHitbox->getTransform().setScale(glm::vec3(1.0f, 1.0f, 0.0f)); // Adjust size as needed
	//attackHitbox->addColliderComponent();
	//attackHitbox->getColliderComponent()->setTrigger(true); // Hitbox should be a trigger

	/*deactivateHitbox();*/
	attackHitbox = nullptr;
	//hitboxActiveTime = 0.5f;
	attackCooldownTimer = 0.0f;
	canAttack = true;

	/// Test ///
	isAttacking = false;
	attackFrameStart = 3;
	attackFrameEnd = 6;
}

EnemyObject::~EnemyObject() {
	if (attackHitbox != nullptr) {
		destroyObject(attackHitbox);
	}
}

void EnemyObject::setCurrentState(State state) {
	currentState = state;
}

EnemyObject::State EnemyObject::getCurrentState() {
	return currentState;
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

float EnemyObject::getDistanceFromTarget() const {
	glm::vec3 targetPos = targetEntity->getTransform().getPosition();
	return glm::length(targetPos - this->transform.getPosition());
}

void EnemyObject::startAttack() {
	attackHitbox->trigger(transform.getPosition());
	attackHitbox->setCanDecreaseTime(false);

	//attackCooldownTimer = attackCooldown;
	//std::cout << "Enemy attacked!" << std::endl;
}

void EnemyObject::endAttack() {
	attackCooldownTimer = attackCooldown;
	attackHitbox->setActive(false);
}

void EnemyObject::setAttackCooldown(float cooldown) {
	attackCooldown = cooldown;
}

float EnemyObject::getAttackCooldown() const {
	return attackCooldown;
}

void EnemyObject::moveTowardsTarget() {
	glm::vec3 targetPos = targetEntity->getTransform().getPosition();

	glm::vec3 currentPos = this->transform.getPosition();
	glm::vec2 newVelocity = this->physics->getVelocity();

	bool grounded = this->collider->getCollisionFlag() & COLLISION_DOWN;
	if (targetPos.y > currentPos.y + this->collider->getHeight() * 0.5f && grounded) {
		newVelocity.y = movementInfo.jumpVelocity;
		//cout << "jump" << endl;
	}
	
	float moveAmount = targetPos.x - currentPos.x;
	moveAmount = moveAmount > 0 ? 1 : -1;
	moveAmount *= movementInfo.speed;
	newVelocity.x = moveAmount;

	isFacingRight = currentPos.x < targetPos.x;

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
	attackHitbox = new DamageCollider<PlayerObject>(this, damage, -1);
	attackHitbox->setActive(false);
	attackHitbox->setFollowOwner(true);
	attackHitbox->setFollowOffset(glm::vec3(0.5f, 0, 0));
	attackHitbox->getColliderComponent()->setWidth(1.5f);
	objectsList.emplace_back(attackHitbox);
}

void EnemyObject::updateState() {
	State prevState = currentState;

	if (prevState == State::ATTACKING) {
		Animation::State animState = getAnimationComponent()->getCurrentAnimationState();
		if (animState.name == "Attacking" && animState.isPlaying) {
			return;
		}
	}

	float distance = getDistanceFromTarget();
	if (distance > aggroRange) {
		currentState = State::IDLE;
		return;
	}

	if (distance > attackRange || attackCooldownTimer > 0.0f) {
		currentState = State::AGGRO;
		return;
	}

	if (!canAttack) {
		currentState = State::AGGRO;
		return;
	}

	currentState = State::ATTACKING;
	// facing target
	isFacingRight = this->getTransform().getPosition().x < targetEntity->getTransform().getPosition().x;
}

void EnemyObject::updateBehavior(list<DrawableObject*>& objectsList) {
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	
	updateState();
	
	float distance = getDistanceFromTarget();

	// Resetting movement
	glm::vec2 vel = getPhysicsComponent()->getVelocity();
	vel.x = 0.0f;
	getPhysicsComponent()->setVelocity(vel);

	switch (currentState) {
		case IDLE:
			getAnimationComponent()->setState("Idle");
			break;

		case AGGRO:
			if (distance <= attackRange) {
				getAnimationComponent()->setState("Idle");
			}
			else {
				getAnimationComponent()->setState("Moving");
				moveTowardsTarget();
			}
			break;

		case ATTACKING:
			getAnimationComponent()->setState("Attacking");
			
			int currentAnimFrame = getAnimationComponent()->getCurrentFrame();
			
			if (currentAnimFrame == attackFrameStart + 1) {
				startAttack();
				break;
			}

			if (currentAnimFrame == attackFrameEnd + 1) {
				endAttack();
				break;
			}

			break;
	}

	if (attackCooldownTimer > 0.0f) {
		attackCooldownTimer -= dt;
	}
}

DamageCollider<PlayerObject>* EnemyObject::getDamageCollider() const {
	return attackHitbox;
}