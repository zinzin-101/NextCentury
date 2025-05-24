#include "ElivaBoss.h"
#include "ElivaBossState.h"
#include "ObjectHeader.h"
#include "Random.h"
#include "PoisonCloud.h"
#include <vector>

ElivaBoss::ElivaBoss(): EnemyObject(DefaultEnemyStat::ELIVA_INFO) {
	cooldownTimer = ElivaStat::COOLDOWN_DURATION;
	stunnedTimer = 0.0f;
	hasShield = false;
	isFuryUsed = false;
	canBlink = false;

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
	states[BossState::Stunned] = State(BossState::Stunned);
	states[BossState::FuryBayonetSlash] = State(BossState::FuryBayonetSlash);
	states[BossState::FuryCooldown] = State(BossState::FuryCooldown);
	states[BossState::Dead] = State(BossState::Dead);

	states[BossState::Cooldown].nextStateAndTransitionCheck[&states[BossState::Blink]] = (&StateTransition::cooldownToBlink);

	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::SerumInject]] = (&StateTransition::blinkToSerumInject);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::Fury]] = (&StateTransition::blinkToFury);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::RifleShot]] = (&StateTransition::blinkToRifleShot);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::blinkToBayonetSlash);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::PoisonCloud]] = (&StateTransition::blinkToPoisonCloud);

	states[BossState::RifleShot].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::rifleShotToBayonetSlash);
	states[BossState::RifleShot].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::rifleShotToCooldown);

	states[BossState::BayonetSlash].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::bayonetSlashToCooldown);

	states[BossState::SerumInject].nextStateAndTransitionCheck[&states[BossState::RifleShot]] = (&StateTransition::serumInjectToRifleShot);
	states[BossState::SerumInject].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::serumInjectToBayonetSlash);
	states[BossState::SerumInject].nextStateAndTransitionCheck[&states[BossState::PoisonCloud]] = (&StateTransition::serumInjectToPoisonCloud);
	states[BossState::SerumInject].nextStateAndTransitionCheck[&states[BossState::Fury]] = (&StateTransition::serumInjectToFury);

	states[BossState::PoisonCloud].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::poisonCloudToBayonetSlash);
	states[BossState::PoisonCloud].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::bayonetSlashToCooldown);

	states[BossState::Fury].nextStateAndTransitionCheck[&states[BossState::FuryBlink]] = (&StateTransition::furyToFuryBlink);
	states[BossState::FuryBlink].nextStateAndTransitionCheck[&states[BossState::RapidBurst]] = (&StateTransition::furyBlinkToRapidBurst);
	states[BossState::RapidBurst].nextStateAndTransitionCheck[&states[BossState::CloseBlink]] = (&StateTransition::rapidBurstToCloseBlink);
	states[BossState::CloseBlink].nextStateAndTransitionCheck[&states[BossState::FuryBayonetSlash]] = (&StateTransition::closeBlinkToBayonetSlash);

	states[BossState::Stunned].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::stunnedToCooldown);

	states[BossState::FuryBayonetSlash].nextStateAndTransitionCheck[&states[BossState::FuryCooldown]] = (&StateTransition::bayonetSlashToCooldown);

	states[BossState::FuryCooldown].nextStateAndTransitionCheck[&states[BossState::FuryBlink]] = (&StateTransition::cooldownToBlink);

	currentState = &states[BossState::Cooldown];

	statesHandler[BossState::Cooldown] = &ElivaBoss::handleCooldown;
	statesHandler[BossState::Blink] = &ElivaBoss::handleBlink;
	statesHandler[BossState::FuryBlink] = &ElivaBoss::handleFuryBlink;
	statesHandler[BossState::CloseBlink] = &ElivaBoss::handleCloseBlink;
	statesHandler[BossState::RifleShot] = &ElivaBoss::handleRifleShot;
	statesHandler[BossState::BayonetSlash] = &ElivaBoss::handleBayonetSlash;
	statesHandler[BossState::PoisonCloud] = &ElivaBoss::handlePoisonCloud;
	statesHandler[BossState::RapidBurst] = &ElivaBoss::handleRapidBurst;
	statesHandler[BossState::SerumInject] = &ElivaBoss::handleSerumInject;
	statesHandler[BossState::Fury] = &ElivaBoss::handleFury;
	statesHandler[BossState::Stunned] = &ElivaBoss::handleStunned;
	statesHandler[BossState::FuryBayonetSlash] = &ElivaBoss::handleFuryBayonetSlash;
	statesHandler[BossState::FuryCooldown] = &ElivaBoss::handleFuryCooldown;
	statesHandler[BossState::Dead] = &ElivaBoss::handleDead;

	currentPhase = Phase::First;
}

void ElivaBoss::start(list<DrawableObject*>& objectsList) {
	setTexture("../Resource/Texture/Boss.png");
	//initAnimation(6, 2);
	initAnimation(11, 21);
	targetEntity = nullptr;
	getAnimationComponent()->addState("Idle", 0, 0, 6, true, ElivaStat::IDLE_TIME_PER_FRAME);
	getAnimationComponent()->addState("Blinking", 1, 0, 9, false, ElivaStat::BLINKING_TIME_PER_FRAME);
	getAnimationComponent()->addState("FuryBlinking", 1, 0, 9, false, ElivaStat::FURY_BLINKING_TIME_PER_FRAME);
	getAnimationComponent()->addState("RifleShot", 4, 0, 10, false, ElivaStat::RIFLE_SHOT_TIME_PER_FRAME);
	getAnimationComponent()->addState("BayonetSlash", 2, 0, 9, false, ElivaStat::BAYONET_SLASH_TIME_PER_FRAME);
	getAnimationComponent()->addState("FuryBayonetSlash", 2, 0, 9, false, ElivaStat::FURY_BAYONET_SLASH_TIME_PER_FRAME);
	getAnimationComponent()->addState("Parried", 8, 0, 2, true, ElivaStat::PARRIED_TIME_PER_FRAME);
	getAnimationComponent()->addState("PoisonCloud", 6, 0, 21, false, ElivaStat::POISON_CLOUD_TIME_PER_FRAME);
	getAnimationComponent()->addState("SerumInject", 7, 0, 21, false, ElivaStat::SERUM_INJECT_TIME_PER_FRAME);
	getAnimationComponent()->addState("RapidBurst", 3, 2, 5, false, ElivaStat::RAPID_BURST_TIME_PER_FRAME); // placeholder
	getAnimationComponent()->addState("Dead", 10, 0, 9, false, ElivaStat::DEAD_TIME_PER_FRAME);


	rifleProjectile = new ProjectileObject<PlayerObject>(this, ElivaStat::RIFLE_SHOT_DAMAGE, this->getTransform().getPosition(), glm::vec2(), ElivaStat::RIFLE_SHOT_LIFESPAN);
	rifleProjectile->setName("RifleProjectile");
	rifleProjectile->setDestroyOnDespawn(false);
	rifleProjectile->getParticleEmitter()->update(objectsList);
	rifleProjectile->setActive(false);
	rifleProjectile->setDrawCollider(true); // debug
	objectsList.emplace_back(rifleProjectile);

	bayonetCollider = new DamageCollider<PlayerObject>(this, ElivaStat::BAYONET_DAMAGE, -1);
	bayonetCollider->setActive(false);
	bayonetCollider->setFollowOwner(true);
	bayonetCollider->setFollowOffset(glm::vec3(-1.0f, 0.0f, 0));
	bayonetCollider->getColliderComponent()->setWidth(2.0f);
	bayonetCollider->setDrawCollider(true); // debug
	objectsList.emplace_back(bayonetCollider);

	poisonCollider = new PoisonCloudCollider(this);
	poisonCollider->setActive(false);
	poisonCollider->setFollowOwner(true);
	poisonCollider->setFollowOffset(glm::vec3(0.0f, 0.0f, 0.0f));
	poisonCollider->getColliderComponent()->setWidth(1.5f);
	poisonCollider->setDrawCollider(true); // debug
	objectsList.emplace_back(poisonCollider);

	this->getTransform().setScale(4, 3);
	this->getColliderComponent()->setHeight(0.5f);
	this->getColliderComponent()->getTransform().setScale(0.5f, 1.0f);
	this->getColliderComponent()->getTransform().setPosition(0.0f, -0.7f);
	this->setDrawCollider(true);

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
	(this->*statesHandler[currentState->currentState])();

	const BossState& state = currentState->currentState;

	if (currentPhase == Phase::First && state == BossState::Blink) { // Special case 1
		if (currentState->nextStateAndTransitionCheck.at(&states[BossState::SerumInject])(this)) {
			currentState = &states[BossState::SerumInject];
			return;
		}
	}

	if (currentPhase == Phase::Second && state == BossState::Blink) { // Special case 2
		if (currentState->nextStateAndTransitionCheck.at(&states[BossState::Fury])(this)) {
			currentState = &states[BossState::Fury];
			return;
		}
	}
	
	static std::vector<State*> possibleStates;
	possibleStates.clear();

	for (const std::pair<State*, TransitionCheckFunction>& stateCheckerPair : currentState->nextStateAndTransitionCheck) {
		State* bossState = stateCheckerPair.first;
		bool isPossible = stateCheckerPair.second(this);
		if (isPossible) {
			possibleStates.emplace_back(bossState);
		}
	}

	int numOfPossibleStates = possibleStates.size();
	if (numOfPossibleStates != 0) {
		unsigned int randInt = Random::Int();
		unsigned int index = randInt % numOfPossibleStates;
		currentState = possibleStates[index];
	}
}

void ElivaBoss::updateBehavior(list<DrawableObject*>& objectsList) {
	if (emitter != nullptr) {
		emitter->update(objectsList);
	}

	processState();

	if (!isInKnockback) {
		this->getPhysicsComponent()->setVelocity(glm::vec2(0.0f, 0.0f));
	}

	std::cout << "current state: " << (int)currentState->currentState << std::endl;
	std::cout << "Health: " << this->getHealth() << std::endl;
}

void ElivaBoss::handleCooldown() {
	this->getAnimationComponent()->setState("Idle");

	if (cooldownTimer <= 0.0f) {
		cooldownTimer = ElivaStat::COOLDOWN_DURATION;
		return;
	}

	cooldownTimer -= GameEngine::getInstance()->getTime()->getDeltaTime();
	std::cout << "boss under cooldown" << std::endl;
}

void ElivaBoss::handleBlink() {
	this->getAnimationComponent()->setState("Blinking");
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	int currentFrame = animState.currentFrame;
	if (currentFrame == 3 + 1) {
		canBlink = true;
		this->setCanTakeDamage(false);
		return;
	}

	if (canBlink && currentFrame == 6 + 1) {
		canBlink = false;

		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float direction = Random::Float() < 0.5f ? -1.0f : 1.0f;
		float offsetX = direction * (Random::Float() * ElivaStat::MAX_BLINK_DISTANCE_FROM_PLAYER);
		glm::vec3 newPos = elivaPos;
		newPos.x = playerPos.x + offsetX;
		this->getTransform().setPosition(newPos);

		float offsetFromPlayer = playerPos.x - elivaPos.x;
		isFacingRight = offsetFromPlayer < 0.0f;

		return;
	}

	if (currentFrame == 8 + 1) {
		this->setCanTakeDamage(true);
		canBlink = false;
		return;
	}
}

void ElivaBoss::handleFuryBlink() {
	this->getAnimationComponent()->setState("FuryBlinking");
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	int currentFrame = animState.currentFrame;
	if (currentFrame == 3 + 1) {
		canBlink = true;
		this->setCanTakeDamage(false);
		return;
	}

	if (canBlink && currentFrame == 6 + 1) {
		canBlink = false;

		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float direction = Random::Float() < 0.5f ? -1.0f : 1.0f;
		float offsetX = direction * (Random::Float() * ElivaStat::MAX_BLINK_DISTANCE_FROM_PLAYER);
		glm::vec3 newPos = elivaPos;
		newPos.x = playerPos.x + offsetX;
		this->getTransform().setPosition(newPos);

		float offsetFromPlayer = playerPos.x - elivaPos.x;
		isFacingRight = offsetFromPlayer < 0.0f;

		return;
	}

	if (currentFrame == 8 + 1) {
		this->setCanTakeDamage(true);
		canBlink = false;
		return;
	}
}

void ElivaBoss::handleCloseBlink() {
	this->getAnimationComponent()->setState("FuryBlinking");
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	int currentFrame = animState.currentFrame;
	if (currentFrame == 3 + 1) {
		canBlink = true;
		this->setCanTakeDamage(false);
		return;
	}

	if (canBlink && currentFrame == 6 + 1) {
		canBlink = false;

		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float direction = Random::Float() < 0.5f ? -1.0f : 1.0f;
		float offsetX = direction * (Random::Float() * ElivaStat::BAYONET_SLASH_RANGE);
		glm::vec3 newPos = elivaPos;
		newPos.x = playerPos.x + offsetX;
		this->getTransform().setPosition(newPos);

		float offsetFromPlayer = playerPos.x - elivaPos.x;
		isFacingRight = offsetFromPlayer < 0.0f;

		return;
	}

	if (currentFrame == 8 + 1) {
		this->setCanTakeDamage(true);
		canBlink = false;
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
		isFacingRight = offsetX < 0.0f;
		return;
	}

	int currentFrame = animState.currentFrame;

	if (currentFrame == 6 + 1) {
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		float direction = isFacingRight ? -1.0f : 1.0f;
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
		isFacingRight = offsetX < 0.0f;
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

	if (currentFrame == 17 + 1) {
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

	if (currentFrame == 0 + 1) {
		setCanTakeDamage(false);
		return;
	}

	if (currentFrame == 13 + 1) {
		hasShield = true;
		currentPhase = Phase::Second;
		return;
	}

	if (currentFrame == 20 + 1) {
		setCanTakeDamage(true);
		return;
	}
}

void ElivaBoss::handleFury() {
	std::cout << "enter fury" << std::endl;
	isFuryUsed = true;
	breakShield();
	currentPhase = Phase::Third;
}

void ElivaBoss::handleStunned() {
	this->getAnimationComponent()->setState("Parried");

	std::cout << "boss stunned" << std::endl;

	if (stunnedTimer <= 0.0f) {
		stunnedTimer = ElivaStat::STUNNED_DURATION;
		return;
	}

	stunnedTimer -= GameEngine::getInstance()->getTime()->getDeltaTime();
}

void ElivaBoss::handleFuryBayonetSlash() {
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	if (animState.name != "FuryBayonetSlash") {
		this->getAnimationComponent()->setState("FuryBayonetSlash");
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		isFacingRight = offsetX < 0.0f;
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

void ElivaBoss::handleFuryCooldown(){
	this->getAnimationComponent()->setState("Idle");

	if (cooldownTimer <= 0.0f) {
		cooldownTimer = ElivaStat::FURY_COOLDOWN_DURATION;
		return;
	}

	cooldownTimer -= GameEngine::getInstance()->getTime()->getDeltaTime();
	std::cout << "boss under fury cooldown" << std::endl;
}

void ElivaBoss::handleDead() {
	this->getAnimationComponent()->setState("Dead");

	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();
	
	if (!animState.isPlaying) {
		this->setHealth(0);
	}
}

void ElivaBoss::postUpdateBehavior() {}

void ElivaBoss::onCollisionStay(Collider* collider) {}

void ElivaBoss::takeDamage(int damage) {
	if (this->getHealth() - damage <= 0) {
		this->setHealth(1);
		this->setCanTakeDamage(false);
		currentState = &states[BossState::Dead];
		return;
	}

	if (hasShield) {
		LivingEntity::takeDamage(static_cast<int>(damage * 0.5f));
		return;
	}

	LivingEntity::takeDamage(static_cast<int>(damage));
}

void ElivaBoss::breakShield() {
	if (hasShield) {
		for (int i = 0; i < 5; i++) {
			ParticleProperties particleProps = ParticleProperties(
				this->getTransform().getPosition(),
				glm::vec2(5.0f * Random::Float() - 2.5f, 1.0f * Random::Float() + 1.0f),
				glm::vec2(-0.5f, 0.5f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				0.15f, 0.1f, 0.05f
			);
			particleProps.isPhysics = true;
			this->getEmitter()->emit(particleProps);
		}
	}

	this->hasShield = false;
}

void ElivaBoss::signalStun() {
	currentState = &states[BossState::Stunned];
	stunnedTimer = ElivaStat::STUNNED_DURATION;
	bayonetCollider->setActive(false);
}

void ElivaBoss::signalStagger() {
	if (currentPhase != Phase::Second) {
		return;
	}

	breakShield();

	if (currentState->currentState == BossState::PoisonCloud) {
		return;
	}

	currentState = &states[BossState::Cooldown];
	cooldownTimer = ElivaStat::STAGGERED_DURATION;
	bayonetCollider->setActive(false);
}

ElivaBoss::Phase ElivaBoss::getCurrentPhase() const {
	return currentPhase;
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

float ElivaBoss::getStunnedTimer() const {
	return stunnedTimer;
}