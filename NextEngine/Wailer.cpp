#include "Wailer.h"
#include "Random.h"
#include "ObjectHeader.h"

Wailer::Wailer(const EnemyInfo& enemyinfo) : EnemyObject(enemyinfo) {
	sonicAttack = nullptr;
	currentState = State::IDLE;
	currentAttackState = AttackState::NONE;
	repositionTimer = 0.0f;
	isInSonicAttack = false;
	zealotCounter = 0;

	getTransform().setScale(1.3f, 1.8f);
	getColliderComponent()->setDimension(0.5f, 0.85f);
	getColliderComponent()->getTransform().setPosition(0.0f, -0.15f);
}

Wailer::~Wailer() {
	if (sonicAttack != nullptr) {
		destroyObject(sonicAttack);
	}
}

void Wailer::start(list<DrawableObject*>& objectsList) {
	setTexture("../Resource/Texture/wailerPlacholder.png");
	initAnimation(8, 6);
	getAnimationComponent()->addState("Idle", 0, 0, 6, true);
	getAnimationComponent()->addState("Moving", 1, 0, 6, true);
	getAnimationComponent()->addState("WindUp", 2, 0, 3, false, 0.67f);
	getAnimationComponent()->addState("SonicAttack", 3, 0, 4, false, 0.2f);
	getAnimationComponent()->addState("WindDown", 4, 0, 3, false, 0.4f);
	getAnimationComponent()->addState("Summoning", 5, 0, 4, false, 0.67f);
	getAnimationComponent()->addState("Stunned", 6, 0, 3, true);

	sonicAttack = new SonicWave();
	sonicAttack->setName("WailerSonicWave");
	objectsList.emplace_back(sonicAttack);

	targetEntity = EnemyObject::findPlayer(objectsList);

	setDrawCollider(true);
}

void Wailer::updateState() {
	State prevState = currentState;
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

	if (currentState == State::STUNNED || currentState == State::FLINCH) {
		resetAttack();
		return;
	}

	if (prevState == State::ATTACKING) {
		return;
	}

	float distance = getDistanceFromTarget();
	if (distance < WailerStat::DISTANCE_FROM_PLAYER_TO_REPOSITION) {
		currentState = State::REPOSITIONING;
		return;
	}

	repositionTimer = 0.0f;

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


void Wailer::handleReposition(float dt) {
	repositionTimer += dt;
	if (repositionTimer < WailerStat::TIME_UNTIL_REPOSITION) {
		this->getAnimationComponent()->setState("Idle");
		return;
	}
	
	float targetXPos = targetEntity->getTransform().getPosition().x;
	float currentXPos = this->getTransform().getPosition().x;
	float offsetFromTarget = targetXPos - currentXPos;

	float newDistance = WailerStat::DISTANCE_FROM_PLAYER_TO_REPOSITION + 0.1f;
	float newXPos = offsetFromTarget > 0.0f ? currentXPos - newDistance : currentXPos + newDistance;
	moveTowardsPosition(newXPos);
	this->getAnimationComponent()->setState("Moving");
}

void Wailer::moveTowardsPosition(float xPosition) {
	glm::vec3 targetPos = this->getTransform().getPosition();
	targetPos.x = xPosition;

	glm::vec3 currentPos = this->transform.getPosition();
	glm::vec2 newVelocity = this->physics->getVelocity();

	bool grounded = this->collider->getCollisionFlag() & COLLISION_DOWN;

	float moveAmount = targetPos.x - currentPos.x;
	moveAmount = moveAmount > 0 ? 1 : -1;
	moveAmount *= movementInfo.speed;
	newVelocity.x = moveAmount;

	isFacingRight = currentPos.x < targetPos.x;

	this->physics->setVelocity(newVelocity);

	glm::vec2 vel = this->physics->getVelocity();
}

void Wailer::updateBehavior(list<DrawableObject*>& objectsList) {
	if (emitter != nullptr) {
		emitter->update(objectsList);

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

		case REPOSITIONING:
			handleReposition(dt);
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
			handleAttackState(objectsList);
			break;
		}
		case STUNNED:
			getAnimationComponent()->setState("Stunned");
			resetAttack();
			cout << "stun" << endl;
			if (currentStunnedTime > 0) {
				currentStunnedTime -= dt;
			}
			else {
				currentState = IDLE;
			}
			break;
		case FLINCH:
			getAnimationComponent()->setState("Idle");
			resetAttack();
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

void Wailer::handleAttackState(std::list<DrawableObject*>& objectlist) {
	if (attackCooldownTimer > 0.0f && currentAttackState == AttackState::NONE) {
		this->getAnimationComponent()->setState("Idle");
		return;
	}

	if (currentAttackState == AttackState::NONE) {
		currentAttackState = (Random::Float() <= 0.5f) ? AttackState::SONICBLAST : AttackState::SUMMONING;

		if (currentAttackState == AttackState::SUMMONING && zealotCounter >= WailerStat::MAX_ZEALOT_PER_WAILER) {
			currentAttackState = AttackState::SONICBLAST;
		}
	}

	switch (currentAttackState) {
		case AttackState::SONICBLAST:
			handleSonicBlastState();
			break;

		case AttackState::SUMMONING:
			this->getAnimationComponent()->setState("Summoning");
			handleSummoningState(objectlist);
			break;
	}
}

void Wailer::handleSonicBlastState() {
	Animation::State animState = this->getAnimationComponent()->getCurrentAnimationState();
	if (animState.name != "WindUp" && !isInSonicAttack) {
		this->getAnimationComponent()->setState("WindUp");
		isInSonicAttack = true;
		sonicAttack->mark(targetEntity->getTransform().getPosition());
		return;
	}

	if (animState.name == "WindUp" && !animState.isPlaying) {
		this->getAnimationComponent()->setState("SonicAttack");
		sonicAttack->trigger();
		return;
	}

	if (animState.name == "SonicAttack" && !animState.isPlaying) {
		this->getAnimationComponent()->setState("WindDown");
		return;
	}

	if (animState.name == "WindDown" && !animState.isPlaying) {
		currentAttackState = AttackState::NONE;
		currentState = State::IDLE;
		isInSonicAttack = false;
		attackCooldownTimer = attackCooldown;
		return;
	}
}

void Wailer::spawnZealot(glm::vec3 playerPos, std::list<DrawableObject*>& objectlist) {
	EnemyInfo summonedZealotInfo = DefaultEnemyStat::ZEALOT_INFO;
	summonedZealotInfo.aggroRange = 100.0f;
	Zealot* zealot = new Zealot(summonedZealotInfo);

	float xPos = (Random::Float() >= 0.5f ? -WailerStat::DISTANCE_TO_SPAWN_ZEALOT : WailerStat::DISTANCE_TO_SPAWN_ZEALOT);

	zealot->getTransform().setPosition(playerPos.x + xPos, playerPos.y + 1.0f);

	objectlist.emplace_back(zealot);
	zealot->start(objectlist);

	zealot->setWailerSummoner(this);
	zealotCounter++;
}

void Wailer::handleSummoningState(std::list<DrawableObject*>& objectlist) {
	Animation::State animState = this->getAnimationComponent()->getCurrentAnimationState();
	if (animState.isPlaying) {
		return;
	}

	spawnZealot(targetEntity->getTransform().getPosition(), objectlist);

	std::cout << "summoning enemies" << std::endl;

	currentAttackState = AttackState::NONE;
	currentState = State::IDLE;
	attackCooldownTimer = attackCooldown;
}

void Wailer::setCurrentState(Wailer::State state) {
	if (state == STUNNED) {
		currentStunnedTime = stunnedTime;
	}
	else if (state == FLINCH) {
		flinchTimer = EnemyStat::FLINCH_TIME;
		attackCooldownTimer = attackCooldown;
	}
	currentState = state;
}

void Wailer::resetAttack() {
	currentAttackState = AttackState::NONE;
	repositionTimer = 0.0f;
	isInSonicAttack = false;
	sonicAttack->reset();
}

void Wailer::removeSummonedZealot() {
	zealotCounter--;
}