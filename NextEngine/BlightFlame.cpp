#include "BlightFlame.h"
#include "ObjectHeader.h"
#include "Random.h"

BlightFlame::BlightFlame(const EnemyInfo& enemyinfo) : EnemyObject(enemyinfo) {
	flameHitbox = nullptr;
	stunnedTime = BlightFlameStat::STUN_DURATION;
	getTransform().setScale(1.0f, 1.6f);
}
void BlightFlame::start(list<DrawableObject*>& objectsList) {
	//setTexture("../Resource/Texture/incineratorSizeFlip.png");
	setTexture("../Resource/Texture/incine1.png");
	//initAnimation(6, 2);
	initAnimation(4, 8);
	targetEntity = nullptr;
	//getAnimationComponent()->addState("Idle", 0, 6);
	//getAnimationComponent()->addState("Moving", 1, 5);
	//getAnimationComponent()->addState("Attacking", 1, 5);
	getAnimationComponent()->addState("Idle", 0, 0, 6, true);
	getAnimationComponent()->addState("Moving", 1, 0, 5, true);
	getAnimationComponent()->addState("WindUp", 0, 0, 6, false);
	getAnimationComponent()->addState("Attack", 0, 0, 2, true);
	getAnimationComponent()->addState("WindDown", 0, 0, 6, false);
	getAnimationComponent()->addState("Melee", 2, 0, 4, false);
	getAnimationComponent()->addState("Stunned", 3, 0, 3, true);
	getAnimationComponent()->setState("Idle");
	attackHitbox = new DamageCollider<PlayerObject>(this, BlightFlameStat::MELEE_DAMAGE, -1);
	attackHitbox->setActive(false);
	attackHitbox->setFollowOwner(true);
	attackHitbox->setFollowOffset(glm::vec3(0.6f, 0.0f ,0.0f));
	objectsList.emplace_back(attackHitbox);
	flameHitbox = new FlameDamage<PlayerObject>(this, BlightFlameStat::FLAME_DAMAGE, 0.2f);
	flameHitbox->DrawableObject::setActive(false);
	flameHitbox->setFollowOwner(true);
	flameHitbox->setFollowOffset(glm::vec3(2.0f, 0, 0));
	flameHitbox->getColliderComponent()->setWidth(4.0f);
	objectsList.emplace_back(flameHitbox);

	targetEntity = EnemyObject::findPlayer(objectsList);
	
	currentAttackType = AttackType::FLAME;
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

	if (distance <= BlightFlameStat::MELEE_ATTACK_DISTANCE) {
		currentAttackType = AttackType::MELEE;
	}
	else {
		currentAttackType = AttackType::FLAME;
	}

	// facing target
	if (targetEntity != nullptr) {
		isFacingRight = this->getTransform().getPosition().x < targetEntity->getTransform().getPosition().x;
	}
}

void BlightFlame::updateBehavior(list<DrawableObject*>& objectsList) {
	/// testing ///
	emitter->update(objectsList);
	///

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
		attackHitbox->setActive(false);
		if (distance <= attackRange) {
			getAnimationComponent()->setState("Idle");
		}
		else {
			getAnimationComponent()->setState("Moving");
			moveTowardsTarget();
		}
		break;

	case ATTACKING: {
		(distance <= BlightFlameStat::MELEE_ATTACK_DISTANCE) ? handleMeleeAttack() : handleFlameAttack();
		break;
	}
	case STUNNED:
		getAnimationComponent()->setState("Stunned");
		attackHitbox->setActive(false);
		flameHitbox->reset();
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

	case FLINCH:
		getAnimationComponent()->setState("Idle");
		attackHitbox->setActive(false);
		flameHitbox->reset();
		cout << "blightflame flinching" << endl;
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

void BlightFlame::startAttack() {
	flameHitbox->trigger(transform.getPosition(), isFacingRight);
}

void BlightFlame::endAttack() {
	flameHitbox->setActive(false);
}

void BlightFlame::handleFlameAttack() {
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	Animation::State currAnim = getAnimationComponent()->getCurrentAnimationState();
	if (currAnim.name == "Idle" || currAnim.name == "Moving" || currAnim.name == "Stunned") {
		getAnimationComponent()->setState("WindUp");
		return;
	}
	if (currAnim.name == "WindUp" && !currAnim.isPlaying) {
		flameTimeKeep = flameTime;
		startAttack();
		getAnimationComponent()->setState("Attack");
		return;
	}
	if (currAnim.name == "Attack") {
		if (flameTimeKeep > 0) {
			flameTimeKeep -= dt;
		}
		else {
			endAttack();
			getAnimationComponent()->setState("WindDown");
			return;
		}
	}
	if (currAnim.name == "WindDown") {
		if (!currAnim.isPlaying) {
			attackCooldownTimer = attackCooldown;
			return;
		}
	}
}

void BlightFlame::handleMeleeAttack() {
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	Animation::State currAnim = getAnimationComponent()->getCurrentAnimationState();
	if (currAnim.name == "Idle" || currAnim.name == "Moving" || currAnim.name == "Stunned" || currAnim.name == "WindUp") {
		meleeTimeKeep = meleeTime;
		EnemyObject::startAttack();
		getAnimationComponent()->setState("Melee");
	}
	if (currAnim.name == "Melee") {
		if (meleeTimeKeep > 0) {
			meleeTimeKeep -= dt;
		}
		else {
			EnemyObject::endAttack();
			getAnimationComponent()->setState("Idle");
		}
	}
}