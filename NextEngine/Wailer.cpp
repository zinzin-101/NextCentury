#include "Wailer.h"
#include "Random.h"
#include "ObjectHeader.h"

Wailer::Wailer(EnemyInfo& enemyinfo) : EnemyObject(enemyinfo) {
	cout << attackCooldown << endl;
}
void Wailer::start(list<DrawableObject*>& objectsList) {
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
	attackHitbox->getColliderComponent()->setWidth(1.5f);
	objectsList.emplace_back(attackHitbox);
}

void Wailer::updateState() {
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
	isFacingRight = this->getTransform().getPosition().x < targetEntity->getTransform().getPosition().x;
}

void Wailer::updateBehavior(list<DrawableObject*>& objectsList) {
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
		//if (currentStunnedTime < 0.6f) {
		//	GameEngine::getInstance()->getRenderer()->getCamera()->CanShake = false;
		//}
		break;
	}

	if (attackCooldownTimer > 0.0f) {
		attackCooldownTimer -= dt;
	}
}

void Wailer::startAttack() {
	attackHitbox->trigger(transform.getPosition());
	attackHitbox->setCanDecreaseTime(false);
}

void Wailer::endAttack() {

}