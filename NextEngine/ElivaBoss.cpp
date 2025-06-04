#include "ElivaBoss.h"
#include "ElivaBossState.h"
#include "ObjectHeader.h"
#include "Random.h"
#include "PoisonCloud.h"
#include <vector>

ElivaBoss::ElivaBoss(): EnemyObject(DefaultEnemyStat::ELIVA_INFO) {
	this->setMaxHealth(ElivaStat::MAX_HEALTH);
	this->setHealth(ElivaStat::MAX_HEALTH);

	normalSprite = 0;
	shieldedSprite = 0;

	cooldownTimer = ElivaStat::COOLDOWN_DURATION;
	stunnedTimer = 0.0f;
	hasShield = false;
	isFuryUsed = false;
	hasInjectedSerum = false;
	canBlink = false;
	isDead = false;
	canUsePoisonCloud = true;

	canStart = false;
	hasSignalDeath = false;

	deathTimer = ElivaStat::DEATH_DIALOGUE_TIMER;
	isDyingDialoguePlaying = false;

	blinkOrigin = glm::vec3();

	for (int i = 0; i < 3; i++) {
		rifleProjectiles[i] = nullptr;
	}
	currentProjectileIndex = 0;
	hasRifleBeenFired = false;

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
	states[BossState::Intro] = State(BossState::Intro);

	states[BossState::Cooldown].nextStateAndTransitionCheck[&states[BossState::Blink]] = (&StateTransition::cooldownToBlink);

	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::SerumInject]] = (&StateTransition::blinkToSerumInject);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::Fury]] = (&StateTransition::blinkToFury);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::RifleShot]] = (&StateTransition::blinkToRifleShot);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::blinkToBayonetSlash);
	states[BossState::Blink].nextStateAndTransitionCheck[&states[BossState::PoisonCloud]] = (&StateTransition::blinkToPoisonCloud);

	states[BossState::RifleShot].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::rifleShotToBayonetSlash);
	states[BossState::RifleShot].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::rifleShotToCooldown);

	states[BossState::BayonetSlash].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::bayonetSlashToCooldown);

	states[BossState::SerumInject].nextStateAndTransitionCheck[&states[BossState::PoisonCloud]] = (&StateTransition::serumInjectToPoisonCloud);

	states[BossState::PoisonCloud].nextStateAndTransitionCheck[&states[BossState::BayonetSlash]] = (&StateTransition::poisonCloudToBayonetSlash);
	states[BossState::PoisonCloud].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::bayonetSlashToCooldown);

	states[BossState::Fury].nextStateAndTransitionCheck[&states[BossState::FuryBlink]] = (&StateTransition::furyToFuryBlink);
	states[BossState::FuryBlink].nextStateAndTransitionCheck[&states[BossState::RapidBurst]] = (&StateTransition::furyBlinkToRapidBurst);
	states[BossState::RapidBurst].nextStateAndTransitionCheck[&states[BossState::CloseBlink]] = (&StateTransition::rapidBurstToCloseBlink);
	states[BossState::CloseBlink].nextStateAndTransitionCheck[&states[BossState::FuryBayonetSlash]] = (&StateTransition::closeBlinkToBayonetSlash);

	states[BossState::Stunned].nextStateAndTransitionCheck[&states[BossState::Cooldown]] = (&StateTransition::stunnedToCooldown);
	states[BossState::Stunned].nextStateAndTransitionCheck[&states[BossState::FuryCooldown]] = (&StateTransition::stunnedToFuryCooldown);

	states[BossState::FuryBayonetSlash].nextStateAndTransitionCheck[&states[BossState::FuryCooldown]] = (&StateTransition::bayonetSlashToCooldown);

	states[BossState::FuryCooldown].nextStateAndTransitionCheck[&states[BossState::FuryBlink]] = (&StateTransition::cooldownToBlink);

	states[BossState::Intro].nextStateAndTransitionCheck[&states[BossState::Blink]] = (&StateTransition::introToBlink);

	currentState = &states[BossState::Intro];

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
	statesHandler[BossState::Intro] = &ElivaBoss::handleIntro;

	currentPhase = Phase::First;
}

void ElivaBoss::start(list<DrawableObject*>& objectsList) {
	normalSprite = setTexture("../Resource/Texture/Boss.png");
	shieldedSprite = setTexture("../Resource/Texture/Boss_Shield.png");
	//initAnimation(6, 2);
	initAnimation(12, 21);
	getAnimationComponent()->setTexture(normalSprite);
	targetEntity = nullptr;
	getAnimationComponent()->addState("Idle", 0, 0, 6, true, ElivaStat::IDLE_TIME_PER_FRAME);
	getAnimationComponent()->addState("Blinking", 1, 0, 9, false, ElivaStat::BLINKING_TIME_PER_FRAME);
	getAnimationComponent()->addState("FuryBlinking", 1, 0, 9, false, ElivaStat::FURY_BLINKING_TIME_PER_FRAME);
	getAnimationComponent()->addState("RifleShot", 4, 0, 10, false, ElivaStat::RIFLE_SHOT_TIME_PER_FRAME);
	getAnimationComponent()->addState("BayonetSlash", 2, 0, 9, false, ElivaStat::BAYONET_SLASH_TIME_PER_FRAME);
	getAnimationComponent()->addState("FuryBayonetSlash", 2, 0, 9, false, ElivaStat::FURY_BAYONET_SLASH_TIME_PER_FRAME);
	getAnimationComponent()->addState("Parried", 9, 0, 2, true, ElivaStat::PARRIED_TIME_PER_FRAME);
	getAnimationComponent()->addState("PoisonCloud", 7, 0, 21, false, ElivaStat::POISON_CLOUD_TIME_PER_FRAME);
	getAnimationComponent()->addState("SerumInject", 8, 0, 21, false, ElivaStat::SERUM_INJECT_TIME_PER_FRAME);
	getAnimationComponent()->addState("RapidBurst", 5, 0, 13, false, ElivaStat::RAPID_BURST_TIME_PER_FRAME);
	getAnimationComponent()->addState("Dead", 11, 0, 9, false, ElivaStat::DEAD_TIME_PER_FRAME);

	for (int i = 0; i < 3; i++) {
		rifleProjectiles[i] = new ProjectileObject<PlayerObject>(this, ElivaStat::RIFLE_SHOT_DAMAGE, this->getTransform().getPosition(), glm::vec2(), ElivaStat::RIFLE_SHOT_LIFESPAN);
		rifleProjectiles[i]->setName(std::string("RifleProjectile") + std::to_string(i));
		rifleProjectiles[i]->setDestroyOnDespawn(false);
		rifleProjectiles[i]->getParticleEmitter()->update(objectsList);
		rifleProjectiles[i]->setActive(false);
		rifleProjectiles[i]->setDrawCollider(true); // debug
		objectsList.emplace_back(rifleProjectiles[i]);
	}

	bayonetCollider = new DamageCollider<PlayerObject>(this, ElivaStat::BAYONET_DAMAGE, -1);
	bayonetCollider->setActive(false);
	bayonetCollider->setFollowOwner(true);
	bayonetCollider->setFollowOffset(glm::vec3(-1.0f, -1.0f, 0));
	bayonetCollider->getColliderComponent()->setDimension(2.0f, 2.0f);
	bayonetCollider->setDrawCollider(true); // debug
	bayonetCollider->setCanDamage(false);
	objectsList.emplace_back(bayonetCollider);

	poisonCollider = new PoisonCloudCollider(this);
	poisonCollider->setActive(false);
	poisonCollider->setFollowOwner(true);
	poisonCollider->setFollowOffset(glm::vec3(0.0f, -1.0f, 0.0f));
	poisonCollider->getColliderComponent()->setWidth(2.0f);
	poisonCollider->setDrawCollider(true); // debug
	objectsList.emplace_back(poisonCollider);

	this->getTransform().setScale(5.0f, 3.75f);
	this->getColliderComponent()->setHeight(0.5f);
	this->getColliderComponent()->getTransform().setScale(0.5f, 1.0f);
	this->getColliderComponent()->getTransform().setPosition(0.0f, -0.925f);
	this->setDrawCollider(true);

	targetEntity = EnemyObject::findPlayer(objectsList);
}

ElivaBoss::~ElivaBoss() {
	for (int i = 0; i < 3; i++) {
		if (rifleProjectiles[i] != nullptr) {
			destroyObject(rifleProjectiles[i]);
		}
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
	std::cout << "current phase: " << (int)currentPhase << std::endl;
	std::cout << "Health: " << this->getHealth() << std::endl;
}

void ElivaBoss::handleCooldown() {
	this->getAnimationComponent()->setState("Idle");
	GameEngine::getInstance()->playSoundEffect("Boss_Idle.wav");

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
		GameEngine::getInstance()->playSoundEffect("Boss_BlinkOut.wav");
		return;
	}

	if (canBlink && currentFrame == 6 + 1) {
		canBlink = false;

		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 origin = blinkOrigin;
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float direction = Random::Float() < 0.5f ? -1.0f : 1.0f;
		float distance = Random::Float() * ElivaStat::MAX_BLINK_DISTANCE_FROM_PLAYER;
		while (distance < ElivaStat::MIN_BLINK_DISTANCE_FOR_BAYONET_FROM_PLAYER) {
			distance = Random::Float() * ElivaStat::MAX_BLINK_DISTANCE_FROM_PLAYER;
		}
		float offsetX = direction * distance;
		glm::vec3 newPos = elivaPos;
		newPos.x = origin.x + offsetX;
		this->getTransform().setPosition(newPos);

		float offsetFromPlayer = playerPos.x - elivaPos.x;
		isFacingRight = offsetFromPlayer < 0.0f;

		return;
	}

	if (currentFrame == 8 + 1) {
		this->setCanTakeDamage(true);
		canBlink = false;
		GameEngine::getInstance()->playSoundEffect("Boss_BlinkIn.wav");
		return;
	}
}

void ElivaBoss::handleFuryBlink() {
	this->getAnimationComponent()->setState("FuryBlinking");
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	int currentFrame = animState.currentFrame;
	if (currentFrame == 3 + 1) {
		GameEngine::getInstance()->playSoundEffect("Boss_BlinkOut.wav");
		canBlink = true;
		this->setCanTakeDamage(false);
		return;
	}

	if (canBlink && currentFrame == 6 + 1) {
		canBlink = false;

		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 origin = blinkOrigin;
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float direction = Random::Float() < 0.5f ? -1.0f : 1.0f;
		float distance = Random::Float() * ElivaStat::MAX_BLINK_DISTANCE_FROM_PLAYER;
		while (distance < ElivaStat::MIN_BLINK_DISTANCE_FOR_RIFLE_SHOT_FROM_PLAYER) {
			distance = Random::Float() * ElivaStat::MAX_BLINK_DISTANCE_FROM_PLAYER;
		}
		float offsetX = direction * distance;
		glm::vec3 newPos = elivaPos;
		newPos.x = origin.x + offsetX;
		this->getTransform().setPosition(newPos);

		float offsetFromPlayer = playerPos.x - elivaPos.x;
		isFacingRight = offsetFromPlayer < 0.0f;

		return;
	}

	if (currentFrame == 8 + 1) {
		this->setCanTakeDamage(true);
		GameEngine::getInstance()->playSoundEffect("Boss_BlinkIn.wav");
		canBlink = false;
		return;
	}
}

void ElivaBoss::handleCloseBlink() {
	this->getAnimationComponent()->setState("FuryBlinking");
	GameEngine::getInstance()->playSoundEffect("Boss_BlinkIn.wav");
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
		//glm::vec3 origin = glm::vec3();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float direction = Random::Float() < 0.5f ? -1.0f : 1.0f;
		float distance = Random::Float() * ElivaStat::BAYONET_SLASH_RANGE;
		while (distance < ElivaStat::MIN_BLINK_DISTANCE_FOR_BAYONET_FROM_PLAYER) {
			distance = Random::Float() * ElivaStat::BAYONET_SLASH_RANGE;
		}
		float offsetX = direction * distance;
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
		GameEngine::getInstance()->playSoundEffect("Boss_Slash.wav");
		return;
	}
}

void ElivaBoss::handleRifleShot() {
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	if (animState.name != "RifleShot") {

		this->getAnimationComponent()->setState("RifleShot");
		GameEngine::getInstance()->playSoundEffect("Boss_RifleCharge.wav");
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		isFacingRight = offsetX < 0.0f;
		hasRifleBeenFired = false;
		canUsePoisonCloud = true;
		hasInjectedSerum = false;
		return;
	}

	int currentFrame = animState.currentFrame;

	if (currentFrame == 6 + 1 && !hasRifleBeenFired) {
		hasRifleBeenFired = true;
		GameEngine::getInstance()->playSoundEffect("Boss_RifleShoot.wav");
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		float direction = isFacingRight ? -1.0f : 1.0f;
		glm::vec2 bulletVelocity = glm::vec2(direction * ElivaStat::RIFLE_SHOT_SPEED, 0);
		glm::vec3 spawnPos = elivaPos;
		spawnPos.x += (direction * 0.5f); // horizontal offset, tweak later
		spawnPos.y -= 0.5f; // vertical offset, tweak later
		rifleProjectiles[++currentProjectileIndex % 3]->activate(spawnPos, bulletVelocity, ElivaStat::RIFLE_SHOT_LIFESPAN);

		return;

	}
}

void ElivaBoss::handleBayonetSlash() {
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	if (animState.name != "BayonetSlash") {
		this->getAnimationComponent()->setState("BayonetSlash");
		GameEngine::getInstance()->playSoundEffect("Boss_Slash.wav");
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		isFacingRight = offsetX < 0.0f;

		canUsePoisonCloud = true;
		hasInjectedSerum = false;

		return;
	}

	int currentFrame = animState.currentFrame;

	if (currentFrame == 4 + 1) {
		bayonetCollider->trigger(this->getTransform().getPosition());
		bayonetCollider->setCanDamage(false);
		bayonetCollider->setCanDecreaseTime(false);

		return;
	}

	if (currentFrame == 6 + 1) {
		bayonetCollider->setCanDamage(true);

		return;
	}

	if (currentFrame == 7 + 1) {
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
		canUsePoisonCloud = false;
		GameEngine::getInstance()->playSoundEffect("Boss_Posion.wav");

		return;
	}

	if (currentFrame == 17 + 1) {
		poisonCollider->setActive(false);
		return;
	}
}

void ElivaBoss::handleRapidBurst() {
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();

	if (animState.name != "RapidBurst") {
		
		this->getAnimationComponent()->setState("RapidBurst");
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		isFacingRight = offsetX < 0.0f;
		hasRifleBeenFired = false;
		return;
	}

	int currentFrame = animState.currentFrame;

	if (currentFrame == 6 + 1 && !hasRifleBeenFired) {
		hasRifleBeenFired = true;
		GameEngine::getInstance()->playSoundEffect("Boss_Burst.wav");
		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		float direction = isFacingRight ? -1.0f : 1.0f;
		glm::vec2 bulletVelocity = glm::vec2(direction * ElivaStat::RIFLE_SHOT_SPEED, 0);
		glm::vec3 spawnPos = elivaPos;
		spawnPos.x += (direction * 0.5f); // horizontal offset, tweak later
		spawnPos.y -= 0.5f; // vertical offset, tweak later
		rifleProjectiles[++currentProjectileIndex % 3]->activate(spawnPos, bulletVelocity, ElivaStat::RIFLE_SHOT_LIFESPAN);

		return;
	}

	if (currentFrame == 7 + 1) {
		hasRifleBeenFired = false;
	}

	if (currentFrame == 11 + 1 && !hasRifleBeenFired) {
		hasRifleBeenFired = true;

		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		float direction = isFacingRight ? -1.0f : 1.0f;
		glm::vec2 bulletVelocity = glm::vec2(direction * ElivaStat::RIFLE_SHOT_SPEED, 0);
		glm::vec3 spawnPos = elivaPos;
		spawnPos.x += (direction * 0.5f); // horizontal offset, tweak later
		spawnPos.y -= 0.5f; // vertical offset, tweak later
		rifleProjectiles[++currentProjectileIndex % 3]->activate(spawnPos, bulletVelocity, ElivaStat::RIFLE_SHOT_LIFESPAN);

		return;
	}

	if (currentFrame == 12 + 1) {
		hasRifleBeenFired = false;
	}

	if (currentFrame == 16 + 1 && !hasRifleBeenFired) {
		hasRifleBeenFired = true;

		glm::vec3 playerPos = targetEntity->getTransform().getPosition();
		glm::vec3 elivaPos = this->getTransform().getPosition();
		float offsetX = playerPos.x - elivaPos.x;
		float direction = isFacingRight ? -1.0f : 1.0f;
		glm::vec2 bulletVelocity = glm::vec2(direction * ElivaStat::RIFLE_SHOT_SPEED, 0);
		glm::vec3 spawnPos = elivaPos;
		spawnPos.x += (direction * 0.5f); // horizontal offset, tweak later
		spawnPos.y -= 0.5f; // vertical offset, tweak later
		rifleProjectiles[++currentProjectileIndex % 3]->activate(spawnPos, bulletVelocity, ElivaStat::RIFLE_SHOT_LIFESPAN);

		return;
	}
}

void ElivaBoss::handleSerumInject() {
	this->getAnimationComponent()->setState("SerumInject");
	GameEngine::getInstance()->playSoundEffect("Boss_SerumInject.wav");
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();
	int currentFrame = animState.currentFrame;

	if (currentFrame == 0 + 1) {
		setCanTakeDamage(false);
		canUsePoisonCloud = true;
		return;
	}

	if (currentFrame == 13 + 1) {
		hasShield = true;
		this->getAnimationComponent()->setTexture(shieldedSprite);
		currentPhase = Phase::Second;
		return;
	}

	if (currentFrame == 20 + 1) {
		setCanTakeDamage(true);
		hasInjectedSerum = true;
		return;
	}
}

void ElivaBoss::handleFury() {
	std::cout << "enter fury" << std::endl;
	isFuryUsed = true;
	breakShield();
	GameEngine::getInstance()->playSoundEffect("Boss_BlinkIn.wav");
	currentPhase = Phase::Third;
}

void ElivaBoss::handleStunned() {
	this->getAnimationComponent()->setState("Parried");
	GameEngine::getInstance()->playSoundEffect("Boss_Idle.wav");
	std::cout << "boss stunned" << std::endl;

	if (stunnedTimer <= 0.0f) {
		stunnedTimer = ElivaStat::STUNNED_DURATION;
		return;
	}

	stunnedTimer -= GameEngine::getInstance()->getTime()->getDeltaTime();
}

void ElivaBoss::handleFuryBayonetSlash() {
	const Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();
	GameEngine::getInstance()->playSoundEffect("Boss_Slash.wav");
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
		bayonetCollider->setCanDamage(false);
		bayonetCollider->setCanDecreaseTime(false);

		return;
	}

	if (currentFrame == 6 + 1) {
		bayonetCollider->setCanDamage(true);

		return;
	}

	if (currentFrame == 7 + 1) {
		bayonetCollider->setActive(false);

		return;
	}
}

void ElivaBoss::handleFuryCooldown(){
	this->getAnimationComponent()->setState("Idle");
	GameEngine::getInstance()->playSoundEffect("Boss_Idle.wav");

	if (cooldownTimer <= 0.0f) {
		cooldownTimer = ElivaStat::FURY_COOLDOWN_DURATION;
		return;
	}

	cooldownTimer -= GameEngine::getInstance()->getTime()->getDeltaTime();
	std::cout << "boss under fury cooldown" << std::endl;
}

void ElivaBoss::handleDead() {
	this->getAnimationComponent()->setState("Dead");

	Animation::State& animState = this->getAnimationComponent()->getCurrentAnimationStateRef();
	int currentFrame = animState.currentFrame;

	if (currentFrame == 0) {
		deathTimer = ElivaStat::DEATH_DIALOGUE_TIMER;
		isDyingDialoguePlaying = false;
		return;
	}

	if (currentFrame == 3 + 1 && !isDyingDialoguePlaying) {
		animState.paused = true;
		isDyingDialoguePlaying = true;

		if (!hasSignalDeath) {
			hasSignalDeath = true;
			GameEngine::getInstance()->signalToCurrentLevel();
		}
		return;
	}

	if (isDyingDialoguePlaying) {
		deathTimer -= GameEngine::getInstance()->getTime()->getDeltaTime();
		if (deathTimer <= 0.0f) {
			animState.paused = false;
			isDyingDialoguePlaying = false;
		}
	}

	if (!animState.isPlaying) {
		this->setHealth(0);

		if (!deathSoundPlayed) {
			deathSoundPlayed = true;
			GameEngine::getInstance()->playSoundEffect("Boss_Died.wav");
		}
	}
}

void ElivaBoss::handleIntro() {
	this->getAnimationComponent()->setState("Idle");
	this->setCanTakeDamage(false);
}

void ElivaBoss::postUpdateBehavior() {}

void ElivaBoss::onCollisionStay(Collider* collider) {}

void ElivaBoss::takeDamage(int damage) {
	if (this->getHealth() - damage <= 0 && !isDead) {
		isDead = true;

		LivingEntity::takeDamage(0);

		this->setHealth(1);
		this->setCanTakeDamage(false);
		currentState = &states[BossState::Dead];

		GameEngine::getInstance()->pauseTimeForSeconds(0.125f);
		//GameEngine::getInstance()->freezeGameForSeconds(0.125f);

		float direction = (targetEntity->getTransform().getPosition().x < this->getTransform().getPosition().x) ? 1.0f : -1.0f;

		for (int i = 0; i < 8; i++) {
			float angle = (static_cast<float>(i) / 8.0f) * 2.0f * glm::pi<float>();
			glm::vec2 vel1(
				direction * (15.0f * Random::Float() + 35.0f),
				glm::sin(angle) * 50.0f
			);

			glm::vec2 vel2(
				direction * (15.0f * Random::Float() + 25.0f),
				glm::sin(angle) * 40.0f
			);

			ParticleProperties p1 = ParticleProperties(
				this->getTransform().getPosition(),
				vel1,
				glm::vec2(-2.5f, 2.5f),
				glm::vec3(0.863f, 0.078f, 0.235f),
				glm::vec3(0.733f, 0.039f, 0.118f),
				0.3f, 0.2f, 0.15f, true, 1.0f, true
			);

			ParticleProperties p2 = ParticleProperties(
				this->getTransform().getPosition(),
				vel2,
				glm::vec2(-2.5f, 2.5f),
				glm::vec3(0.863f, 0.078f, 0.235f),
				glm::vec3(0.733f, 0.039f, 0.118f),
				0.3f, 0.2f, 0.15f, true, 1.0f, true
			);

			this->emitter->emit(p1);
			this->emitter->emit(p2);
		}

		return;
	}

	if (hasShield) {
		LivingEntity::takeDamage(static_cast<int>(damage * 0.5f));
		return;
	}

	LivingEntity::takeDamage(static_cast<int>(damage));
}

void ElivaBoss::onDeath(list<DrawableObject*>& objectsList) {}

void ElivaBoss::signalCanStart() {
	this->canStart = true;
}

void ElivaBoss::breakShield() {
	if (hasShield) {
		for (int i = 0; i < 10; i++) {
			ParticleProperties particleProps = ParticleProperties(
				this->getTransform().getPosition(),
				glm::vec2(10.0f * Random::Float() - 5.0f, 5.0f * Random::Float() + 20.0f),
				glm::vec2(-0.5f, 0.5f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				0.15f, 0.1f, 0.05f
			);
			particleProps.isPhysics = true;
			this->getEmitter()->emit(particleProps);
		}
	}

	this->getAnimationComponent()->setTexture(normalSprite);

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

void ElivaBoss::setBlinkOrigin(float x) {
	this->blinkOrigin.x = x;
}

bool ElivaBoss::getCanStart() const {
	return this->canStart;
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

bool ElivaBoss::hasSerumBeenInjected() const {
	return hasInjectedSerum;
}

float ElivaBoss::getStunnedTimer() const {
	return stunnedTimer;
}

bool ElivaBoss::getCanUsePoisonCloud() const {
	return canUsePoisonCloud;
}