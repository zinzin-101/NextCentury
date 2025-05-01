#include "Dialogue.h"
#include "GameEngine.h"

Dialogue::Dialogue(int fontSize, PlayerObject* player, bool isActiveByPosition) {
	text = TextObject();
	fontsize = fontSize;
	placeHolderBG = SimpleObject();
	placeHolderBG.setColor(255, 255, 255);
	placeHolderBG.getTransform().setScale(100, 100);
	activeRange = 2.0f;
	isDialogueActive = false;
	this->player = player;
	this->isActiveByPosition = isActiveByPosition;
}

void Dialogue::render(glm::mat4 globalModelTransform) {
	if (isDialogueActive) {
		placeHolderBG.render(globalModelTransform);
		text.render(globalModelTransform);
	}
	//placeHolderBG.render(globalModelTransform);
	//text.render(globalModelTransform);
}

void Dialogue::update(list<DrawableObject*>& objectsList) {
	text.getTransform().setPosition(glm::vec3(this->getTransform().getPosition().x * 120, this->getTransform().getPosition().y * 120, 0.0f));
	placeHolderBG.getTransform().setPosition(glm::vec3(this->getTransform().getPosition().x * 120, this->getTransform().getPosition().y * 120, 0.0f));
	if (isActiveByPosition) {
		checkActiveRange();
	}
	if (!sentences.empty() && text.getText() != sentences.front()) {
		
		text.loadText(sentences.front(), { 0, 0, 0, 0 }, fontsize);
	}
	placeHolderBG.getTransform().setScale(glm::vec3(text.getTransform().getScale().x + 10.0f, text.getTransform().getScale().y, 0.0f));
	//placeHolderBG.getTransform().setPosition(glm::vec3(placeHolderBG.getTransform().getPosition().x + 0, placeHolderBG.getTransform().getPosition().y + 2.5f, 0.0f));
}

void Dialogue::nextSentence() {
	if (sentences.empty()) {
		endDialogue();
		return;
	}
	sentences.pop();

	if (sentences.empty()) {
		endDialogue();
	}
}

void Dialogue::addSentence(string s) {
	sentences.push(s);
}

void Dialogue::setSentences(queue<string> s) {
	sentences = s;
}

void Dialogue::endDialogue() {
	isActiveByPosition = false;
	isDialogueActive = false;
}

void Dialogue::checkActiveRange() {
	if (abs(player->getTransform().getPosition().x - this->getTransform().getPosition().x) <= activeRange) {
		isDialogueActive = true;
	}
	else {
		isDialogueActive = false;
	}
}