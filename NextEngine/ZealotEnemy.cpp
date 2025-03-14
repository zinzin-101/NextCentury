#include "Zealot.h"

Zealot::Zealot(EnemyInfo& enemyinfo) : EnemyObject(enemyinfo) {
	cout << attackCooldown << endl;
}
void Zealot::start(list<DrawableObject*>& objectsList) {
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

	case ATTACKING:
		getAnimationComponent()->setState("Attacking");

		int currentAnimFrame = getAnimationComponent()->getCurrentFrame();

		if (currentAnimFrame == attackFrameStart + 1) {
			startAttack();
			break;
		}

		if (currentAnimFrame == attackFrameEnd + 1) {
			endAttack();
			GameEngine::getInstance()->getRenderer()->getCamera()->shake = false;
			//cout << attackCooldownTimer << endl;
			break;
		}

		break;
	}

	if (attackCooldownTimer > 0.0f) {
		attackCooldownTimer -= dt;
	}
}

void Zealot::moveTowardsTarget() {
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