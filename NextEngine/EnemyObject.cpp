#include "EnemyObject.h"
#include "DamageCollider.h"
#include <limits>

//#include "ParticleSystem.h" // temporary testing
#include <iostream>

EnemyObject::EnemyObject(const EnemyInfo& enemyInfo) : LivingEntity(enemyInfo.name, enemyInfo.health) {
	this->movementInfo = enemyInfo.movementInfo;
	this->aggroRange = enemyInfo.health;
	this->attackRange = enemyInfo.attackRange;
	this->attackCooldown = enemyInfo.attackCooldown;
	this->damage = enemyInfo.damage;
	currentState = IDLE;
	addColliderComponent();
	addPhysicsComponent();

	this->stunnedTime = 1.0f;
	this->currentStunnedTime = 0;
	//attackHitbox = new SimpleObject();
	//attackHitbox->setColor(1.0f, 0.0f, 0.0f); // Red color for debugging
	//attackHitbox->getTransform().setScale(glm::vec3(1.0f, 1.0f, 0.0f)); // Adjust size as needed
	//attackHitbox->addColliderComponent();
	//attackHitbox->getColliderComponent()->setTrigger(true); // Hitbox should be a trigger

	this->flinchTimer = 0.0f;

	targetEntity = nullptr;

	/*deactivateHitbox();*/
	attackHitbox = nullptr;
	//hitboxActiveTime = 0.5f;
	attackCooldownTimer = 0.0f;
	canAttack = true;

	isAttacking = false;
	attackFrameStart = 2;
	attackFrameEnd = 4;

	canMoveTowardTarget = true;
}

EnemyObject::~EnemyObject() {
	if (attackHitbox != nullptr) {
		destroyObject(attackHitbox);
	}
}

void EnemyObject::setCurrentState(State state) {
	if (state == STUNNED) {
		currentStunnedTime = stunnedTime;
	}
	else if (state == FLINCH) {
		flinchTimer = EnemyStat::FLINCH_TIME;
		attackCooldownTimer = attackCooldown;
	}
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
	if (attackHitbox == nullptr) {
		return;
	}

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
	if (targetEntity == nullptr) {
		return std::numeric_limits<float>::infinity();
	}

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
	if (targetEntity == nullptr || !canMoveTowardTarget) {
		this->getAnimationComponent()->setState("Idle");
		return;
	}

	glm::vec3 targetPos = targetEntity->getTransform().getPosition();

	glm::vec3 currentPos = this->transform.getPosition();
	glm::vec2 newVelocity = this->physics->getVelocity();

	bool grounded = this->collider->getCollisionFlag() & COLLISION_DOWN;

	/// Disable jumping
	//if (targetPos.y > currentPos.y + this->collider->getHeight() * 0.5f && grounded) {
	//	newVelocity.y = movementInfo.jumpVelocity;
	//	//cout << "jump" << endl;
	//}
	
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

	this->targetEntity = nullptr;
}

void EnemyObject::setTarget(LivingEntity* target) {
	this->targetEntity = target;
}

PlayerObject* EnemyObject::findPlayer(std::list<DrawableObject*>& objectList) {
	for (DrawableObject* obj : objectList) {
		PlayerObject* player = dynamic_cast<PlayerObject*>(obj);
		if (player != NULL) {
			return player;
		}
	}

	return nullptr;
}

void EnemyObject::start(list<DrawableObject*>& objectsList) {
	//setTexture("../Resource/Texture/incineratorSizeFlip.png");
	setTexture("../Resource/Texture/enemyplaceholder.png");
	//initAnimation(6, 2);
	initAnimation(3, 6);
	targetEntity = nullptr;
	//getAnimationComponent()->addState("Idle", 0, 6);
	//getAnimationComponent()->addState("Moving", 1, 5);
	//getAnimationComponent()->addState("Attacking", 1, 5);
	getAnimationComponent()->addState("Idle", 0, 0, 6, true);
	getAnimationComponent()->addState("Moving", 1, 0, 5, true);
	getAnimationComponent()->addState("Attacking", 2, 0, 6, false);
	getAnimationComponent()->setState("Idle");
	attackHitbox = new DamageCollider<PlayerObject>(this, damage, -1);
	attackHitbox->setActive(false);
	attackHitbox->setFollowOwner(true);
	attackHitbox->setFollowOffset(glm::vec3(0.5f, 0, 0));
	attackHitbox->getColliderComponent()->setWidth(1.5f);
	objectsList.emplace_back(attackHitbox);

	targetEntity = EnemyObject::findPlayer(objectsList);
}

void EnemyObject::updateState() {
	State prevState = currentState;
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	if (currentState == State::STUNNED) {
		return;
	}

	if (currentState == State::FLINCH) {
		return;
	}

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
	if (targetEntity != nullptr) {
		isFacingRight = this->getTransform().getPosition().x < targetEntity->getTransform().getPosition().x;

	}
}

void EnemyObject::updateBehavior(list<DrawableObject*>& objectsList) {
	/// Using particle emitter ///
	emitter->update(objectsList);
	///

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

		case ATTACKING: {
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
		case STUNNED:
			cout << "stun" << endl;
			getAnimationComponent()->setState("Idle");
			attackHitbox->setActive(false);

			if (currentStunnedTime > 0) {
				currentStunnedTime -= dt;
			}
			else {
				currentState = IDLE;
			}
			//if (currentStunnedTime < 0.6f) {
			//	GameEngine::getInstance()->getRenderer()->getCamera()->CanShake = false;
			//}
			break;

		case FLINCH:
			getAnimationComponent()->setState("Idle");
			attackHitbox->setActive(false);
			if (flinchTimer > 0) {
				flinchTimer -= dt;
			}
			else {
				currentState = IDLE;
			}

			break;
	}

	if (attackCooldownTimer > 0.0f) {
		attackCooldownTimer -= dt;
	}
}

void EnemyObject::postUpdateBehavior() {
	canMoveTowardTarget = true;
}

DamageCollider<PlayerObject>* EnemyObject::getDamageCollider() const {
	return attackHitbox;
}

void EnemyObject::onCollisionStay(Collider* collider) {
	if (targetEntity == nullptr) {
		return;
	}

	if (this->getDistanceFromTarget() > this->getAggroRange()) {
		return;
	}

	EnemyObject* otherEnemy = dynamic_cast<EnemyObject*>(collider->getObject());
	if (otherEnemy != NULL) {
		MovementInfo otherMovement = otherEnemy->getMovementInfo();
		if (this->movementInfo.speed > otherMovement.speed) {
			return;
		}

		float currentXPos = this->getTransform().getPosition().x;
		
		float targetXPos = targetEntity->getTransform().getPosition().x;
		float targetXOffset = targetXPos - currentXPos;

		float otherXPos = otherEnemy->getTransform().getPosition().x;
		float otherXOffset = otherXPos - currentXPos;

		if ((otherXOffset > 0.0f) == (targetXOffset > 0.0f)) {
			canMoveTowardTarget = false;
		}
	}
}

MovementInfo EnemyObject::getMovementInfo() const {
	return movementInfo;
}