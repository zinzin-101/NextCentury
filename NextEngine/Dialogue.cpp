#include "Dialogue.h"
#include "GameEngine.h"

Dialogue::Dialogue(int fontSize) {
	text = TextObject();
	fontsize = fontSize;
	placeHolderBG = SimpleObject();
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
	if (text.getText() != sentences.front()) {
		text.loadText(sentences.front(), { 255, 255, 255, 255 }, fontsize);
	}
}

void Dialogue::nextSentence() {
	sentences.pop_front();
}

void Dialogue::addSentence(string s) {
	sentences.push_back(s);
}

void Dialogue::setSentences(list<string> s) {
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