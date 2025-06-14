#include "PlayerObject.h"
#include "SonicWave.h"
#include <iostream>

SonicWave::SonicWave() {
	groundOffset = -1.0f;

	this->addColliderComponent();
	this->getColliderComponent()->setTrigger(true);
	//this->setDrawCollider(true);
	//this->setDrawColliderColor(glm::vec3(1, 0, 0));

	this->setRenderOrder(1);

	/// set texture and init animation ///
	setTexture("../Resource/Texture/MadPreacher.png");
	initAnimation(7, 41);
	getAnimationComponent()->addState("Marking", 6, 0, 4, false, SonicWaveStat::ANIMATION_TIME_PER_FRAME);
	getAnimationComponent()->addState("Attack", 6, 8, 16, false, SonicWaveStat::ANIMATION_TIME_PER_FRAME);

	this->getAnimationComponent()->setState("Marking");

	damageStartFrame = 4;

	getTransform().scales(2.0f);
	setGroundOffset(-0.25f);

	reset();
}

void SonicWave::onCollisionStay(Collider* collider) {
	if (currentStage == DAMAGE && !used) {
		PlayerObject* player = dynamic_cast<PlayerObject*>(collider->getObject());
		if (player == NULL) {
			return;
		}
		used = true;
		std::cout << "Sonic hit player" << std::endl;
		player->takeDamage(SonicWaveStat::SONIC_WAVE_DAMAGE);
		player->flinch(0.5f);
	}
}

void SonicWave::setGroundOffset(float offset) {
	this->groundOffset = offset;
}

void SonicWave::trigger() {
	currentStage = DAMAGE;
	this->getAnimationComponent()->setState("Attack");
}

void SonicWave::mark(glm::vec3 position) {
	position.y += groundOffset;
	this->getTransform().setPosition(position);
	currentStage = MARKING;
	this->getAnimationComponent()->setState("Marking");
	this->setActive(true);
}

void SonicWave::reset() {
	used = false;
	currentStage = NONE;
	this->setActive(false);
}

void SonicWave::update(std::list<DrawableObject*>& objectsList) {
	DrawableObject::update(objectsList);

	if (currentStage == NONE) {
		return;
	}

	Animation::State currentState = this->getAnimationComponent()->getCurrentAnimationState();

	if (!currentState.isPlaying && currentStage == DAMAGE) {
		reset();
	}
}