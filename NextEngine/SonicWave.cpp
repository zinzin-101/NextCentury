#include "PlayerObject.h"
#include "SonicWave.h"
#include <iostream>

SonicWave::SonicWave() {
	groundOffset = -1.0f;

	addColliderComponent();
	this->setDrawCollider(true);
	this->setDrawColliderColor(glm::vec3(1, 0, 0));

	/// set texture and init animation ///
	setTexture("../Resource/Texture/BOOM.png");
	initAnimation(1, 9);
	getAnimationComponent()->addState("Marking", 0, 0, 4, false);
	getAnimationComponent()->addState("Attack", 0, 4, 5, false);

	damageStartFrame = 4;

	reset();
}

void SonicWave::onCollisionEnter(Collider* collider) {
	if (currentStage == DAMAGE) {
		PlayerObject* player = dynamic_cast<PlayerObject*>(collider->getObject());
		if (player == NULL) {
			return;
		}

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