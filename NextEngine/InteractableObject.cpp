#include "interactableObject.h"

InteractableObject::InteractableObject(string describe, PlayerObject* player) : TexturedObject() {
	description = describe;
	descriptionText = new Dialogue(24, player, false);
	descriptionText->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	this->player = player;
}

void InteractableObject::update(list<DrawableObject*>& objectsList) {
	if (abs(player->getTransform().getPosition().x - this->getTransform().getPosition().x) <= 0.5f) {
		this->getAnimationComponent()->setState("clickAble");
		isClickable = true;
	}
	else {
		this->getAnimationComponent()->setState("idle");
		isClickable = false;
	}
	
}

void InteractableObject::setDescriptionActive(bool b) {
	if (!isClickable) {
		return;
	}
	descriptionText->isDialogueActive = b;
}

bool InteractableObject::getDescriptionActive() {
	return isClickable;
}