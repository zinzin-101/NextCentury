#include "Dialogue.h"
#include "GameEngine.h"

Dialogue::Dialogue(int fontSize, PlayerObject* player, bool isActiveByPosition) {
	text = TextObject();
	fontsize = fontSize;
	Backdrop = TexturedObject();
	Backdrop.setTexture("../Resource/Texture/StoryStuff/InteracableObject_DescriotionBox.png");
	Backdrop.getTransform().setScale(1000, 1000);
	activeRange = 2.0f;
	isDialogueActive = false;
	this->player = player;
	this->isActiveByPosition = isActiveByPosition;
	isBackdropActive = true;
	this->setRenderOrder(2);
	isEnd = false;
}

glm::vec3 Dialogue::getTextScale() {
	return text.getTransform().getScale();
}

void Dialogue::render(glm::mat4 globalModelTransform) {
	if (isDialogueActive) {
		if (isBackdropActive) {
			Backdrop.render(globalModelTransform);
		}
		text.render(globalModelTransform);
	}
	//placeHolderBG.render(globalModelTransform);
	//text.render(globalModelTransform);
}

void Dialogue::update(list<DrawableObject*>& objectsList) {
	Backdrop.getTransform().setScale(glm::vec3(text.getTransform().getScale().x + 30.0f, text.getTransform().getScale().y - 30.0f, 0.0f));
	text.getTransform().setPosition(glm::vec3(this->getTransform().getPosition().x * 120, this->getTransform().getPosition().y * -120, 0.0f));
	Backdrop.getTransform().setPosition(glm::vec3(this->getTransform().getPosition().x * 120, this->getTransform().getPosition().y * 120, 0.0f));
	if (isActiveByPosition) {
		checkActiveRange();
	}
	if (!sentences.empty() && text.getText() != sentences.front()) {
		text.loadText(sentences.front(), { 255, 255, 255, 255 }, fontsize);
	}
	
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
	isEnd = true;
}

void Dialogue::checkActiveRange() {
	if (abs(player->getTransform().getPosition().x - this->getTransform().getPosition().x) <= activeRange) {
		isDialogueActive = true;
	}
	else {
		isDialogueActive = false;
	}
}

void Dialogue::setBackDropActive(bool b) {
	isBackdropActive = b;
}

void Dialogue::setDialogueTextBold() {
	text.setFontStyleBold();
}
void Dialogue::setDialogueTextNormal() {
	text.setFontStyleNormal();
}