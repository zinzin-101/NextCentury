#include "GotItemText.h"
#include "GameEngine.h"

GotItemText::GotItemText(string itemText, PlayerObject* player, list<DrawableObject*>& objectsList) {
	appearTime = 2.0f;
	isReturning = false;
	isAppeared = false;
	text = new Dialogue(24, player, false);
	text->addSentence(itemText);
	text->isDialogueActive = true;
	objectsList.emplace_back(text);
	appearPosX = 0;
	appearPosY = 0;
	//appearPos = Set appear pos de fault as KHOB JOR
	text->getTransform().setPosition(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition() + glm::vec3(10.0f, 1.0f, 0.0f));
}
void GotItemText::update(list<DrawableObject*>& objectsList) {
	glm::vec3 targetPos = GameEngine::getInstance()->getRenderer()->getCamera()->getPosition() + glm::vec3(appearPosX, 1.0f + appearPosY, 0.0f);
	glm::vec3 targetPos2 = GameEngine::getInstance()->getRenderer()->getCamera()->getPosition() + glm::vec3(10.0f, 1.0f, 0.0f);
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	if (isAppeared) {
		glm::vec3 currentPos = text->getTransform().getPosition();
		float lerpFactor = 0.2f;
		glm::vec3 lerpedPos;

		if (glm::length(currentPos - targetPos) < 0.1f) {
			isReturning = true;
		}
		if (isReturning) {
			if (appearTime < 0.0f) {
				lerpedPos = glm::mix(currentPos, targetPos2, lerpFactor);
				text->getTransform().setPosition(lerpedPos);
			}
			else {
				appearTime -= dt;
			}
		}
		else {
			lerpedPos = glm::mix(currentPos, targetPos, lerpFactor);
			text->getTransform().setPosition(lerpedPos);
		}
		
	}
}

void GotItemText::activateAppear() {
	isAppeared = true;
}

void GotItemText::setAppearPos(float x, float y) {
	appearPosX = x;
	appearPosY = y;
}