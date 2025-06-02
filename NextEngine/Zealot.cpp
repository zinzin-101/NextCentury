#include "Zealot.h"
#include "ObjectHeader.h"
#include "Random.h"

Zealot::Zealot(const EnemyInfo& enemyinfo) : EnemyObject(enemyinfo) {
	//cout << attackCooldown << endl;

	wailerSummonner = nullptr;

	getTransform().setScale(1.58f, 2.05f);
	getColliderComponent()->setDimension(0.5f, 0.85f);
	getColliderComponent()->getTransform().setPosition(0.0f, -0.15f);

	attackFrameStart = 3;
	attackFrameActivate = 6;
	attackFrameEnd = 7;
}

Zealot::~Zealot() {
	if (wailerSummonner != nullptr) {
		wailerSummonner->removeSummonedZealot(this);
	}
}

void Zealot::start(list<DrawableObject*>& objectsList) {
	//setTexture("../Resource/Texture/incineratorSizeFlip.png");
	setTexture("../Resource/Texture/Purifier.png");
	//initAnimation(6, 2);
	initAnimation(6, 9);
	targetEntity = nullptr;
	getAnimationComponent()->addState("Idle", 0, 0, 6, true);
	getAnimationComponent()->addState("Moving", 1, 0, 8, true);
	getAnimationComponent()->addState("Attack1", 2, 0, 9, false, ZealotStat::ATTACK_ANIMATION_TIME_PER_FRAME);
	getAnimationComponent()->addState("Attack2", 2, 0, 9, false, ZealotStat::ATTACK_ANIMATION_TIME_PER_FRAME);
	getAnimationComponent()->addState("Stunned", 4, 0, 2, true);
	getAnimationComponent()->setState("Idle");
	attackHitbox = new DamageCollider<PlayerObject>(this, damage, -1);
	attackHitbox->setActive(false);
	attackHitbox->setCanDamage(false);
	attackHitbox->setFollowOwner(true);
	attackHitbox->setFollowOffset(glm::vec3(0.5f, 0.25f, 0));
	//attackHitbox->getColliderComponent()->getTransform().translate(0.0f, -0.5f);
	//attackHitbox->setDrawCollider(true); // debug
	attackHitbox->getColliderComponent()->setWidth(1.0f);
	objectsList.emplace_back(attackHitbox);

	stunnedTime = ZealotStat::STUN_DURATION;
	flinchTimer = 0.0f;

	targetEntity = EnemyObject::findPlayer(objectsList);

	endAttack();
}

void Zealot::updateState() {
	State prevState = currentState;
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	if (currentState == State::STUNNED) {
		attackHitbox->setActive(false);
		return;
	}

	if (currentState == State::FLINCH) {
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
	if (emitter != nullptr) {
		emitter->update(objectsList);

	}

	if (isInKnockback) {
		return;
	}

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

		if (currentAnimFrame == attackFrameActivate + 1) {
			attack();
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

		//cout << "stun" << endl;
		if (currentStunnedTime > 0) {
			currentStunnedTime -= dt;
		}
		else {
			currentState = IDLE;
		}

		break;

	case FLINCH:
		getAnimationComponent()->setState("Idle");
		attackHitbox->setActive(false);
		//cout << "flinching" << endl;
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

void Zealot::startAttack() {
	attackHitbox->trigger(transform.getPosition());
	attackHitbox->setCanDecreaseTime(false);
	attackHitbox->setCanDamage(false);
}

void Zealot::attack() {
	attackHitbox->setCanDamage(true);
}

void Zealot::endAttack() {
	if (currentAttack == Variation1) {
		if (Random::Float() <= 0.3f) {
			currentAttack = Variation2;
			//cout << "attack2" << endl;
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

void Zealot::setWailerSummoner(Wailer* wailer) {
	this->wailerSummonner = wailer;
}