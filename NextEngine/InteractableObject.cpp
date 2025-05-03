#include "interactableObject.h"
#include <iostream>

InteractableObject::InteractableObject(string describe, PlayerObject* player, Dialogue* dialogue) : TexturedObject() {
	description = describe;
	descriptionText = dialogue;
	descriptionText->addSentence(describe);
	//descriptionText->getTransform().setPosition(glm::vec3(5.0f, -2.0f, 0.0f));
	this->player = player;
	//descriptionText->isDialogueActive = true;
}

void InteractableObject::update(list<DrawableObject*>& objectsList) {
	descriptionText->getTransform().setPosition(glm::vec3(this->getTransform().getPosition().x, this->getTransform().getPosition().y + this->getTransform().getScale().y / 2, 0.0f));

	if (abs(player->getTransform().getPosition().x - this->getTransform().getPosition().x) <= 0.5f) {
		this->getAnimationComponent()->setState("clickAble");
		isClickable = true;
	}
	else {
		this->getAnimationComponent()->setState("idle");
		isClickable = false;
	}
	setDescriptionActive(isClickable);
}

void InteractableObject::setDescriptionActive(bool b) {
	//if (!isClickable) {
	//	return;
	//}
	descriptionText->isDialogueActive = b;
}

bool InteractableObject::getDescriptionActive() {
	return isClickable;
}