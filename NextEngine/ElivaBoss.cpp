#include "ElivaBoss.h"
#include "ElivaBossState.h"
#include "ObjectHeader.h"
#include "Random.h"
#include "PoisonCloud.h"
#include <vector>

ElivaBoss::ElivaBoss(): EnemyObject(DefaultEnemyStat::ELIVA_INFO) {
	cooldownTimer = ElivaStat::COOLDOWN_DURATION;
	hasShield = false;
	isFuryUsed = false;

	rifleProjectile = nullptr;
	bayonetCollider = nullptr;
	poisonCollider = nullptr;

	states[BossState::Cooldown] = State(BossState::Cooldown);
	states[BossState::Blink] = State(BossState::Blink);
	states[BossState::FuryBlink] = State(BossState::FuryBlink);
	states[BossState::CloseBlink] = State(BossState::CloseBlink);
	states[BossState::RifleShot] = State(BossState::RifleShot);
	states[BossState::BayonetSlash] = State(BossState::BayonetSlash);
	states[BossState::PoisonCloud] = State(BossState::PoisonCloud);
	states[BossState::RapidBurst] = State(BossState::RapidBurst);
	states[BossState::SerumInject] = State(BossState::SerumInject);
	states[BossState::Fury] = State(BossState::Fury);

	states[BossState::Cooldown].nextStateAndTransitionCheck[&states[BossState::Blink]] = (&StateTransition::cooldownToBlink);

	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::SerumInject]] = (&StateTransition::blinkToSerumInject);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::RifleShot]] = (&StateTransition::blinkToRifleShot);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::blinkToBayonetSlash);

	states[BossState::RifleShot].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::rifleShotToBayonetSlash);
	states[BossState::RifleShot].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::rifleShotToCooldown);

	states[BossState::BayonetSlash].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::bayonetSlashToCooldown);

	states[BossState::SerumInject].nextStateAndTransitionCheck[&states[BossState::RifleShot]] = (&StateTransition::serumInjectToRifleShot);
	states[BossState::SerumInject].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::serumInjectToBayonetSlash);
	states[BossState::SerumInject].nextStateAndTransitionCheck[&states[BossState::PoisonCloud]] = (&StateTransition::serumInjectToPoisonCloud);
	states[BossState::SerumInject].nextStateAndTransitionCheck[&states[BossState::Fury]] = (&StateTransition::serumInjectToFury);

	states[BossState::PoisonCloud].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::poisonCloudToBayonetSlash);

	states[BossState::Fury].nextStateAndTransitionCheck[&states[BossState::FuryBlink]] = (&StateTransition::furyToFuryBlink);
	states[BossState::FuryBlink].nextStateAndTransitionCheck[&states[BossState::RapidBurst]] = (&StateTransition::furyBlinkToRapidBurst);
	states[BossState::RapidBurst].nextStateAndTransitionCheck[&states[BossState::CloseBlink]] = (&StateTransition::rapidBurstToCloseBlink);
	states[BossState::CloseBlink].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::closeBlinkToBayonetSlash);

	currentState = &states[BossState::Cooldown];

	statesHandler[BossState::Cooldown] = &(ElivaBoss::handleCooldown);
	statesHandler[BossState::Blink] = &(ElivaBoss::handleBlink);
	statesHandler[BossState::FuryBlink] = &(ElivaBoss::handleFuryBlink);
	statesHandler[BossState::CloseBlink] = &(ElivaBoss::handleCloseBlink);
	statesHandler[BossState::RifleShot] = &(ElivaBoss::handleRifleShot);
	statesHandler[BossState::BayonetSlash] = &(ElivaBoss::handleBayonetSlash);
	statesHandler[BossState::PoisonCloud] = &(ElivaBoss::handlePoisonCloud);
	statesHandler[BossState::RapidBurst] = &(ElivaBoss::handleRapidBurst);
	statesHandler[BossState::SerumInject] = &(ElivaBoss::handleSerumInject);
	statesHandler[BossState::Fury] = &(ElivaBoss::handleFury);
}

void ElivaBoss::start(list<DrawableObject*>& objectsList) {
	setTexture("../Resource/Texture/Purifier2.png");
	//initAnimation(6, 2);
	initAnimation(9, 22);
	targetEntity = nullptr;
	getAnimationComponent()->addState("Idle", 0, 0, 6, true, ElivaStat::IDLE_TIME_PER_FRAME);
	getAnimationComponent()->addState("Blinking", 1, 0, 9, false, ElivaStat::BLINKING_TIME_PER_FRAME);
	getAnimationComponent()->addState("RifleShot", 4, 0, 10, false, ElivaStat::RIFLE_SHOT_TIME_PER_FRAME);
	getAnimationComponent()->addState("BayonetSlash", 2, 0, 9, false, ElivaStat::BAYONET_SLASH_TIME_PER_FRAME);
	getAnimationComponent()->addState("PoisonCloud", 6, 0, 22, false, ElivaStat::POISON_CLOUD_TIME_PER_FRAME);
	getAnimationComponent()->addState("SerumInject", 7, 0, 21, false, ElivaStat::SERUM_INJECT_TIME_PER_FRAME);
	getAnimationComponent()->addState("RapidBurst", 3, 2, 5, false, ElivaStat::RAPID_BURST_TIME_PER_FRAME); // placeholder


	rifleProjectile = new ProjectileObject<PlayerObject>(this, ElivaStat::RIFLE_SHOT_DAMAGE, this->getTransform().getPosition(), glm::vec2(), ElivaStat::RIFLE_SHOT_LIFESPAN);
	rifleProjectile->setDestroyOnDespawn(false);
	rifleProjectile->setActive(false);
	objectsList.emplace_back(rifleProjectile);

	bayonetCollider = new DamageCollider<PlayerObject>(this, ElivaStat::BAYONET_DAMAGE, -1);
	bayonetCollider->setActive(false);
	bayonetCollider->setFollowOwner(true);
	bayonetCollider->setFollowOffset(glm::vec3(1.0f, 0.0f, 0));
	bayonetCollider->getColliderComponent()->setWidth(3.0f);
	objectsList.emplace_back(bayonetCollider);

	poisonCollider = new PoisonCloudCollider(this);
	poisonCollider->setActive(false);
	poisonCollider->setFollowOwner(true);
	poisonCollider->setFollowOffset(glm::vec3(0.0f, 0.0f, 0.0f));
	poisonCollider->getColliderComponent()->setWidth(5.0f);
	objectsList.emplace_back(poisonCollider);

	targetEntity = EnemyObject::findPlayer(objectsList);
}

ElivaBoss::~ElivaBoss() {
	if (rifleProjectile != nullptr) {
		destroyObject(rifleProjectile);
	}

	if (bayonetCollider != nullptr) {
		destroyObject(bayonetCollider);
	}

	if (poisonCollider != nullptr) {
		destroyObject(poisonCollider);
	}
}

void ElivaBoss::processState() {
	const BossState& state = currentState->currentState;

	if (state == BossState::Blink) { // Special case 1
		if (currentState->nextStateAndTransitionCheck.at(&states[BossState::SerumInject])(this)) {
			currentState = &states[BossState::SerumInject];
		}
	}
	else if (state == BossState::SerumInject) { // Special case 2
		if (currentState->nextStateAndTransitionCheck.at(&states[BossState::Fury])(this)) {
			currentState = &states[BossState::Fury];
		}
	}
	else {
		std::vector<State*> possibleStates;

		for (const std::pair<State*, TransitionCheckFunction>& stateCheckerPair : currentState->nextStateAndTransitionCheck) {
			State* bossState = stateCheckerPair.first;
			bool isPossible = stateCheckerPair.second(this);
			if (isPossible) {
				possibleStates.emplace_back(bossState);
			}
		}

		int numOfPossibleStates = possibleStates.size();
		if (numOfPossibleStates != 0) {
			unsigned int randInt = Random::Int() % numOfPossibleStates;
			currentState = possibleStates[randInt];
		}
	}

	(this->*statesHandler[currentState->currentState])();
}

void ElivaBoss::updateBehavior(list<DrawableObject*>& objectsList) {
	//+
}

void ElivaBoss::handleCooldown() {
	this->getAnimationComponent()->setState("Idle");
	cooldownTimer -= GameEngine::getInstance()->getTime()->getDeltaTime();
}

void ElivaBoss::handleBlink() {
	this->getAnimationComponent()->setState("Blinking");
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	int currentFrame = animState.currentFrame;
	if (currentFrame == 6 + 1) {
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float direction = Random::Float() < 0.5f ? -1.0f : 1.0f;
		float offsetX = direction * (Random::Float() * ElivaStat::MAX_BLINK_DISTANCE_FROM_PLAYER);
		glm::vec3 newPos = elivaPos;
		newPos.x = playerPos.x + offsetX;
		this->getTransform().setPosition(newPos);

		float offsetFromPlayer = playerPos.x - elivaPos.x;
		isFacingRight = offsetFromPlayer > 0.0f;

		return;
	}
}

void ElivaBoss::handleFuryBlink() {
	this->getAnimationComponent()->setState("Blinking");
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	int currentFrame = animState.currentFrame;
	if (currentFrame == 6 + 1) {
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float direction = Random::Float() < 0.5f ? -1.0f : 1.0f;
		float offsetX = direction * (Random::Float() * ElivaStat::MAX_BLINK_DISTANCE_FROM_PLAYER);
		glm::vec3 newPos = elivaPos;
		newPos.x = playerPos.x + offsetX;
		this->getTransform().setPosition(newPos);

		float offsetFromPlayer = playerPos.x - elivaPos.x;
		isFacingRight = offsetFromPlayer > 0.0f;

		return;
	}
}

void ElivaBoss::handleCloseBlink() {
	this->getAnimationComponent()->setState("Blinking");
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	int currentFrame = animState.currentFrame;
	if (currentFrame == 6 + 1) {
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float direction = Random::Float() < 0.5f ? -1.0f : 1.0f;
		float offsetX = direction * (Random::Float() * ElivaStat::MAX_BLINK_DISTANCE_FROM_PLAYER * 0.5f);
		glm::vec3 newPos = elivaPos;
		newPos.x = playerPos.x + offsetX;
		this->getTransform().setPosition(newPos);

		float offsetFromPlayer = playerPos.x - elivaPos.x;
		isFacingRight = offsetFromPlayer > 0.0f;

		return;
	}
}

void ElivaBoss::handleRifleShot() {
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	if (animState.name != "RifleShot") {
		this->getAnimationComponent()->setState("RifleShot");
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		isFacingRight = offsetX > 0.0f;
		return;
	}

	int currentFrame = animState.currentFrame;

	if (currentFrame == 6 + 1) {
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		float direction = offsetX >= 0.0f ? 1.0f : -1.0f;
		glm::vec2 bulletVelocity = glm::vec2(direction * ElivaStat::RIFLE_SHOT_SPEED, 0);
		glm::vec3 spawnPos = elivaPos;
		spawnPos.x + (direction * 0.5f); // horizontal offset, tweak later
		rifleProjectile->activate(spawnPos, bulletVelocity, ElivaStat::RIFLE_SHOT_LIFESPAN);

		return;
	}
}

void ElivaBoss::handleBayonetSlash() {
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	if (animState.name != "BayonetSlash") {
		this->getAnimationComponent()->setState("BayonetSlash");
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		isFacingRight = offsetX > 0.0f;
		return;
	}

	int currentFrame = animState.currentFrame;

	if (currentFrame == 4 + 1) {
		bayonetCollider->trigger(this->getTransform().getPosition());
		bayonetCollider->setCanDecreaseTime(false);

		return;
	}

	if (currentFrame == 5 + 1) {
		bayonetCollider->setActive(false);

		return;
	}
}

void ElivaBoss::handlePoisonCloud() {
	this->getAnimationComponent()->setState("PoisonCloud");

	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();
	int currentFrame = animState.currentFrame;

	if (currentFrame == 9 + 1) {
		poisonCollider->setActive(true);
		return;
	}

	if (currentFrame == 18 + 1) {
		poisonCollider->setActive(false);
		return;
	}
}

void ElivaBoss::handleRapidBurst() {
	this->getAnimationComponent()->setState("RapidBurst");
	std::cout << "in rapid burst" << std::endl;
}

void ElivaBoss::handleSerumInject() {
	this->getAnimationComponent()->setState("SerumInject");

	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();
	int currentFrame = animState.currentFrame;

	if (currentFrame == 13 + 1) {
		hasShield = true;
		return;
	}
}

void ElivaBoss::handleFury() {
	std::cout << "in fury" << std::endl;
	isFuryUsed = true;
}

void ElivaBoss::postUpdateBehavior() {}

void ElivaBoss::onCollisionStay(Collider* collider) {}

void ElivaBoss::breakShield() {
	this->hasShield = false;
}

float ElivaBoss::getCoolDownTimer() const {
	return cooldownTimer;
}

float ElivaBoss::getDistanceFromPlayer() const {
	return EnemyObject::getDistanceFromTarget();
}

bool ElivaBoss::isShieldActivated() const {
	return hasShield;
}

bool ElivaBoss::hasFuryBeenActivated() const{
	return isFuryUsed;
}