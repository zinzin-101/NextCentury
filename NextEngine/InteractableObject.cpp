#include "interactableObject.h"
#include "GameEngine.h"
#include <iostream>

InteractableObject::InteractableObject(string fileName, PlayerObject* player) : TexturedObject() {
	
	string myText;

	ifstream meFile(fileName);

	if (!meFile.is_open()) {
		cout << "File Not Open Interactable Object." << endl;
		return;
	}

	while (getline(meFile, myText)) {
		string dialogueText = myText;
		string fontSize;
		getline(meFile, myText);
		Dialogue k = Dialogue(stoi(myText), player, false);
		k.addSentence(dialogueText);
		
		txtEachLine.emplace_back(k);
	}
	isShowingTxt = false;

	//descriptionText->getTransform().setPosition(glm::vec3(5.0f, -2.0f, 0.0f));
	this->player = player;
	//descriptionText->isDialogueActive = true;
	Backdrop = TexturedObject();
	Backdrop.setTexture("../Resource/Texture/StoryStuff/InteracableObject_DescriotionBox_Blackdrop.png");
	Backdrop.getTransform().setScale(16.0f,9.0f);
	Backdrop.setActive(false);
	Backdrop.setRenderOrder(1);
}

void InteractableObject::update(list<DrawableObject*>& objectsList) {
	//This one is for the text to be above the object
	//descriptionText->getTransform().setPosition(glm::vec3(this->getTransform().getPosition().x, this->getTransform().getPosition().y + this->getTransform().getScale().y / 2, 0.0f));
	
	Backdrop.getTransform().setPosition(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition());
	//descriptionText->getTransform().setPosition(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition());

	float offsetY = 0;
	for (int i = 0; i < txtEachLine.size(); i++) {
		txtEachLine[i].getTransform().setPosition(glm::vec3(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition().x, GameEngine::getInstance()->getRenderer()->getCamera()->getPosition().y, 0.0f));
		offsetY += txtEachLine[i].getTransform().getScale().y;
		txtEachLine[i].update(objectsList);
	}

	if (abs(player->getTransform().getPosition().x - this->getTransform().getPosition().x) <= 0.5f * this->getTransform().getScale().x) {
		this->getAnimationComponent()->setState("clickAble");
		isClickable = true;
	}
	else {
		this->getAnimationComponent()->setState("idle");
		isClickable = false;
	}
	//setDescriptionActive(isClickable);

	if (!isClickable) {
		setDescriptionActive(false);
	}
	
}

void InteractableObject::render(glm::mat4 globalModelTransform) {
	TexturedObject::render(globalModelTransform);
	if (Backdrop.getIsActive()) {
		Backdrop.render(globalModelTransform);
	}
	for (int i = 0; i < txtEachLine.size(); i++) {
		txtEachLine[i].render(globalModelTransform);
	}
}

void InteractableObject::setDescriptionActive(bool b) {
	//if (!isClickable) {
	//	return;
	//}
	isShowingTxt = b;
	Backdrop.setActive(b);
	for (int i = 0; i < txtEachLine.size(); i++) {
		txtEachLine[i].isDialogueActive = b;
	}
}

bool InteractableObject::getDescriptionActive() {
	return isShowingTxt; // FAKE
}

bool InteractableObject::getIsClickable() {
	return isClickable;
}