#include "Dialogue.h"
#include "GameEngine.h"

Dialogue::Dialogue(int fontSize) {
	text = TextObject();
	fontsize = fontSize;
	placeHolderBG = SimpleObject();
	placeHolderBG.setColor(255, 255, 255);
	placeHolderBG.getTransform().setScale(100, 100);
	activeRange = 3.0f;
	isActive = false;
}

void Dialogue::render(glm::mat4 globalModelTransform) {
	//if (isActive) {
	//	placeHolderBG.render(globalModelTransform);
	//	text.render(globalModelTransform);
	//}
	placeHolderBG.render(globalModelTransform);
	text.render(globalModelTransform);
}

void Dialogue::update(list<DrawableObject*>& objectsList) {
	DrawableObject::setActive(isActive);
	if (text.getText() != sentences.front()) {
		
		text.loadText(sentences.front(), { 0, 0, 0, 0 }, fontsize);
	}
}

void Dialogue::nextSentence() {
	sentences.pop();
}

void Dialogue::addSentence(string s) {
	sentences.push(s);
}

void Dialogue::setSentences(queue<string> s) {
	sentences = s;
}

void Dialogue::checkActiveRange(glm::vec3 playerPos) {
	if (abs(playerPos.x - this->getTransform().getPosition().x) <= activeRange) {
		isActive = true;
	}
	else {
		isActive = false;
	}
}