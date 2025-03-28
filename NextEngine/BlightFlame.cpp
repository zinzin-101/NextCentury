#include "BlightFlame.h"
#include "FlameDamage.h"
#include "Random.h"

BlightFlame::BlightFlame(EnemyInfo& enemyinfo) : EnemyObject(enemyinfo) {
	flameHitbox = nullptr;
}
void BlightFlame::start(list<DrawableObject*>& objectsList) {
	//setTexture("../Resource/Texture/incineratorSizeFlip.png");
	setTexture("../Resource/Texture/BlightFlameplaceholder.png");
	//initAnimation(6, 2);
	initAnimation(6, 6);
	targetEntity = nullptr;
	//getAnimationComponent()->addState("Idle", 0, 6);
	//getAnimationComponent()->addState("Moving", 1, 5);
	//getAnimationComponent()->addState("Attacking", 1, 5);
	getAnimationComponent()->addState("Idle", 0, 0, 6, true);
	getAnimationComponent()->addState("Moving", 1, 0, 5, true);
	getAnimationComponent()->addState("WindUp", 2, 0, 4, false);
	getAnimationComponent()->addState("Attack", 3, 0, 2, true);
	getAnimationComponent()->addState("WindDown", 4, 0, 4, false);
	getAnimationComponent()->addState("Stunned", 5, 0, 3, true);
	getAnimationComponent()->setState("Idle");
	attackHitbox = nullptr;
	flameHitbox = new FlameDamage<PlayerObject>(this, damage, 0.2f);
	flameHitbox->setActive(false);
	flameHitbox->setFollowOwner(true);
	flameHitbox->setFollowOffset(glm::vec3(0.5f, 0, 0));
	flameHitbox->getColliderComponent()->setWidth(1.5f);
	objectsList.emplace_back(flameHitbox);
}

BlightFlame::~BlightFlame() {
	if (flameHitbox != nullptr) {
		destroyObject(flameHitbox);
	}
}

void BlightFlame::setDamage(int damage) {
	if (flameHitbox != nullptr) {
		flameHitbox->setDamage(damage);
	}
}

FlameDamage<PlayerObject>* BlightFlame::getFlameCollider() const {
	return flameHitbox;
}

void BlightFlame::updateState() {
	State prevState = currentState;
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	if (currentState == State::STUNNED) {
		flameHitbox->setActive(false);
		return;
	}

	if (prevState == State::ATTACKING && attackCooldownTimer <= 0.0f) {
		Animation::State animState = getAnimationComponent()->getCurrentAnimationState();
		if (animState.name == "WindUp" && animState.isPlaying ||
			animState.name == "Attack" && animState.isPlaying ||
			animState.name == "WindDown" && animState.isPlaying) {
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

void BlightFlame::updateBehavior(list<DrawableObject*>& objectsList) {
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
		Animation::State currAnim = getAnimationComponent()->getCurrentAnimationState();
		if (currAnim.name == "Idle" || currAnim.name == "Moving" || currAnim.name == "Stunned") {
			getAnimationComponent()->setState("WindUp");
		}
		if (currAnim.name == "WindUp" && !currAnim.isPlaying) {
			flameTimeKeep = flameTime;
			startAttack();
			getAnimationComponent()->setState("Attack");
		}
		if (currAnim.name == "Attack") {
			if (flameTimeKeep > 0) {
				flameTimeKeep -= dt;
			}
			else {
				endAttack();
				getAnimationComponent()->setState("WindDown");
			}
		}
		if (currAnim.name == "WindDown") {
			if (!currAnim.isPlaying) {
				attackCooldownTimer = attackCooldown;
			}
		}
		break;
	}
	case STUNNED:
		getAnimationComponent()->setState("Stunned");

		cout << "stun" << endl;
		if (currentStunnedTime > 0) {
			currentStunnedTime -= dt;
		}
		else {
			currentState = IDLE;
		}
		if (currentStunnedTime < 0.6f) {
			GameEngine::getInstance()->getRenderer()->getCamera()->shake = false;
		}
		break;
	}

	if (attackCooldownTimer > 0.0f) {
		attackCooldownTimer -= dt;
	}
}

void BlightFlame::moveTowardsTarget() {
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

void BlightFlame::startAttack() {
	flameHitbox->trigger(transform.getPosition());
}

void BlightFlame::endAttack() {
	flameHitbox->setActive(false);
}