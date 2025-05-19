#include "interactableObject.h"
#include "GameEngine.h"
#include <iostream>

InteractableObject::InteractableObject(string fileName, PlayerObject* player, string texture, list<DrawableObject*>& objectsList) : TexturedObject() {
	this->setTexture(texture);
	this->initAnimation(2, 1);
	this->getAnimationComponent()->addState("idle", 0, 0, 1, true);
	this->getAnimationComponent()->addState("clickAble", 1, 0, 1, true);
	this->getAnimationComponent()->setState("idle");

	string myText;

	ifstream meFile(fileName);

	if (!meFile.is_open()) {
		cout << "File Not Open Interactable Object." << endl;
		return;
	}

	while (getline(meFile, myText)) {
		string dialogueText = myText; // text

		getline(meFile, myText); // font size
		Dialogue *k = new Dialogue(stoi(myText), player, false);
		getline(meFile, myText); // font style
		if (myText == "b") {
			k->setDialogueTextBold();
		}
		else {
			k->setDialogueTextNormal();
		}
		k->getTransform().setPosition(this->getTransform().getPosition());
		k->addSentence(dialogueText);
		k->setBackDropActive(false);
		txtEachLine.emplace_back(k);
	}
	isShowingTxt = false;
	offSetWidth = 3.0f;
	offSetHeight = 2.0f;
	posOffset = glm::vec3(0.0f,0.0f,0.0f);
	//descriptionText->getTransform().setPosition(glm::vec3(5.0f, -2.0f, 0.0f));
	this->player = player;
	//descriptionText->isDialogueActive = true;
	Backdrop = new TexturedObject();
	Backdrop->setTexture("../Resource/Texture/StoryStuff/InteracableObject_DescriotionBox_Blackdrop.png");
	Backdrop->getTransform().setScale(16.0f,9.0f);
	Backdrop->setActive(false);
	Backdrop->setRenderOrder(1);
	objectsList.emplace_back(Backdrop);

	BackdropText = new TexturedObject();
	BackdropText->setTexture("../Resource/Texture/StoryStuff/InteracableObject_DescriotionBox.png");
	BackdropText->getTransform().setScale((offSetWidth * 2.1f), offSetHeight * 2.25f);
	BackdropText->setActive(false);
	BackdropText->setRenderOrder(1);
	objectsList.emplace_back(BackdropText);

	for (int i = 0; i < txtEachLine.size(); i++) {
		objectsList.emplace_back(txtEachLine[i]);
	}
}

void InteractableObject::update(list<DrawableObject*>& objectsList) {
	//This one is for the text to be above the object
	//descriptionText->getTransform().setPosition(glm::vec3(this->getTransform().getPosition().x, this->getTransform().getPosition().y + this->getTransform().getScale().y / 2, 0.0f));

	Backdrop->getTransform().setPosition(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition());
	BackdropText->getTransform().setPosition(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition() + posOffset);
	//descriptionText->getTransform().setPosition(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition());

	float offsetY = 0;
	for (int i = 0; i < txtEachLine.size(); i++) {
		float offsetX = (GameEngine::getInstance()->getRenderer()->getCamera()->getPosition().x - offSetWidth) + ((txtEachLine[i]->getTextScale().x / 2.0f)/120.0f);
		txtEachLine[i]->getTransform().setPosition(glm::vec3(offsetX, (offsetY / 120.0f) + offSetHeight, 0.0f));
		offsetY += txtEachLine[i]->getTextScale().y;
		txtEachLine[i]->update(objectsList);
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

void InteractableObject::insertTextInObjectList(list<DrawableObject*>& objectsList) {
	for (int i = 0; i < txtEachLine.size(); i++) {
		objectsList.emplace_back(txtEachLine[i]);
	}
}

void InteractableObject::render(glm::mat4 globalModelTransform) {
	TexturedObject::render(globalModelTransform);
	//if (Backdrop->getIsActive()) {
	//	Backdrop->render(globalModelTransform);
	//	BackdropText->render(globalModelTransform);
	//}
	//for (int i = 0; i < txtEachLine.size(); i++) {
	//	txtEachLine[i]->render(globalModelTransform);
	//}
}

void InteractableObject::setDescriptionActive(bool b) {
	//if (!isClickable) {
	//	return;
	//}
	isShowingTxt = b;
	Backdrop->setActive(b);
	BackdropText->setActive(b);
	for (int i = 0; i < txtEachLine.size(); i++) {
		txtEachLine[i]->isDialogueActive = b;
	}
}

bool InteractableObject::getDescriptionActive() {
	return isShowingTxt; // FAKE
}

bool InteractableObject::getIsClickable() {
	return isClickable;
}

TexturedObject* InteractableObject::getBackdrop() {
	return Backdrop;
}

TexturedObject* InteractableObject::getBackdropText() {
	return BackdropText;
}

void InteractableObject::setPosOffset(glm::vec3 offset) {
	posOffset = offset;
}