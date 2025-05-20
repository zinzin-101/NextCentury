#include "GotItemText.h"
#include "GameEngine.h"

GotItemText::GotItemText(string itemText, PlayerObject* player, list<DrawableObject*>& objectsList) {
	appearTime = 2.0f;
	isAppeared = false;
	text = new Dialogue(24, player, false);
	text->addSentence(itemText);
	text->isDialogueActive = true;
	objectsList.emplace_back(text);
	text->getTransform().setPosition(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition() + glm::vec3(2.0f, 0.0f, 0.0f));
}
void GotItemText::update(list<DrawableObject*>& objectsList) {
	glm::vec3 setPos(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition().x + 2.0f, 0.0f, 0.0f);
	if (isAppeared) {
		glm::vec3 lerpedPos = ((1 - 0.2f) * (GameEngine::getInstance()->getRenderer()->getCamera()->getPosition().x) + 0.2f * setPos);
		text->getTransform().setPosition(lerpedPos);
	}
}
void GotItemText::activateAppear() {
	isAppeared = true;
}