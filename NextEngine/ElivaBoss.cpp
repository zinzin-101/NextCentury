#include "ElivaBoss.h"
#include "ElivaBossState.h"
#include "PlayerObject.h"

ElivaBoss::ElivaBoss(): EnemyObject(DefaultEnemyStat::ELIVA_INFO) {
	cooldownTimer = 0.0f;
	hasShield = false;
	isFuryUsed = false;

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

	currentState = BossState::Cooldown;
}

void ElivaBoss::start(list<DrawableObject*>& objectsList) {
	setTexture("../Resource/Texture/Purifier2.png");
	//initAnimation(6, 2);
	initAnimation(9, 22);
	targetEntity = nullptr;
	getAnimationComponent()->addState("Idle", 0, 0, 6, true, ElivaStat::IDLE_TIME_PER_FRAME);
	getAnimationComponent()->addState("Blinking", 1, 0, 9, false, ElivaStat::BLINKING_TIME_PER_FRAME);
	getAnimationComponent()->addState("RifleShot", 5, 0, 11, false, ElivaStat::RIFLE_SHOT_TIME_PER_FRAME);
	getAnimationComponent()->addState("BayonetSlash", 2, 0, 9, false, ElivaStat::BAYONET_SLASH_TIME_PER_FRAME);
	getAnimationComponent()->addState("PoisonCloud", 6, 0, 22, false, ElivaStat::POISON_CLOUD_TIME_PER_FRAME);
	getAnimationComponent()->addState("SerumInject", 7, 0, 21, false, ElivaStat::SERUM_INJECT_TIME_PER_FRAME);

	targetEntity = EnemyObject::findPlayer(objectsList);
}

ElivaBoss::~ElivaBoss() {

}

void ElivaBoss::processState() {

}

void ElivaBoss::updateBehavior(list<DrawableObject*>& objectsList) {

}

void ElivaBoss::postUpdateBehavior() {}


void ElivaBoss::onCollisionStay(Collider* collider) {}

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