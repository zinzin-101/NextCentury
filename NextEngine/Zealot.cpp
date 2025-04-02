#include "Zealot.h"
#include "ObjectHeader.h"
#include "Random.h"

Zealot::Zealot(const EnemyInfo& enemyinfo) : EnemyObject(enemyinfo) {
	cout << attackCooldown << endl;
}
void Zealot::start(list<DrawableObject*>& objectsList) {
	//setTexture("../Resource/Texture/incineratorSizeFlip.png");
	setTexture("../Resource/Texture/Zealotplaceholder3.png");
	//initAnimation(6, 2);
	initAnimation(5, 6);
	targetEntity = nullptr;
	//getAnimationComponent()->addState("Idle", 0, 6);
	//getAnimationComponent()->addState("Moving", 1, 5);
	//getAnimationComponent()->addState("Attacking", 1, 5);
	getAnimationComponent()->addState("Idle", 0, 0, 6, true);
	getAnimationComponent()->addState("Moving", 1, 0, 5, true);
	getAnimationComponent()->addState("Attack1", 2, 0, 6, false);
	getAnimationComponent()->addState("Attack2", 3, 0, 6, false);
	getAnimationComponent()->addState("Stunned", 4, 0, 3, true);
	getAnimationComponent()->setState("Idle");
	attackHitbox = new DamageCollider<PlayerObject>(this, damage, -1);
	attackHitbox->setActive(false);
	attackHitbox->setFollowOwner(true);
	attackHitbox->setFollowOffset(glm::vec3(0.5f, 0, 0));
	attackHitbox->getColliderComponent()->getTransform().translate(0.0f, -0.5f);
	attackHitbox->getColliderComponent()->setWidth(1.5f);
	objectsList.emplace_back(attackHitbox);

	stunnedTime = ZealotStat::STUN_DURATION;

	targetEntity = EnemyObject::findPlayer(objectsList);
}

void Zealot::updateState() {
	State prevState = currentState;
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	if (currentState == State::STUNNED) {
		attackHitbox->setActive(false);
		return;
	}

	if (prevState == State::ATTACKING && attackCooldownTimer <= 0.0f) {
		Animation::State animState = getAnimationComponent()->getCurrentAnimationState();
		if (animState.name == "Attack1" && animState.isPlaying || animState.name == "Attack2" && animState.isPlaying) {
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

void Zealot::updateBehavior(list<DrawableObject*>& objectsList) {
	/// testing ///
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
		if (currentAttack == Variation1) {
			getAnimationComponent()->setState("Attack1");
		}
		else {
			getAnimationComponent()->setState("Attack2");
		}

		int currentAnimFrame = getAnimationComponent()->getCurrentFrame();

		if (currentAnimFrame == attackFrameStart + 1) {
			startAttack();
			break;
		}

		if (currentAnimFrame == attackFrameEnd + 1) {
			endAttack();
			//cout << attackCooldownTimer << endl;
			break;
		}
		break;
	}
	case STUNNED:
		getAnimationComponent()->setState("Stunned");
		attackHitbox->setActive(false);

		cout << "stun" << endl;
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
	}

	if (attackCooldownTimer > 0.0f) {
		attackCooldownTimer -= dt;
	}
}

void Zealot::moveTowardsTarget() {
	if (targetEntity == nullptr) {
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

void Zealot::startAttack() {
	attackHitbox->trigger(transform.getPosition());
	attackHitbox->setCanDecreaseTime(false);
}

void Zealot::endAttack() {
	if (currentAttack == Variation1) {
		if (Random::Float() <= 0.3f) {
			currentAttack = Variation2;
			cout << "attack2" << endl;
		}
		else {
			attackCooldownTimer = attackCooldown;
		}
	}
	else {
		currentAttack = Variation1;
		attackCooldownTimer = attackCooldown;
	}
	attackHitbox->setActive(false);
}