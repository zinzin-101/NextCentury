#include "ProtagThoughts.h"
#include "GameEngine.h"
#include <fstream>

void ProtagThoughts::readFile(string fileName) {
	string myText;
	ifstream meFile(fileName);

	if (!meFile.is_open()) {
		cout << "File Not Open Interactable Object." << endl;
		return;
	}

	while (getline(meFile, myText)) {
		dialogueText->addSentence(myText);
		getline(meFile, myText); // time
		lifeTimeEachSentence.push(stof(myText));
	}
}

ProtagThoughts::ProtagThoughts(string fileName, PlayerObject* player) {
	isActiveDialogue = false;
	fontSize = 40;
	dialogueText = new Dialogue(fontSize, player, false);
	//dialogueText->setBackDropActive(false);
	this->player = player;
	readFile(fileName);
	keepTime = lifeTimeEachSentence.front();
}

ProtagThoughts::ProtagThoughts(string fileName, PlayerObject* player, int fSize) {
	isActiveDialogue = false;
	fontSize = fSize;
	dialogueText = new Dialogue(fontSize, player, false);
	//dialogueText->setBackDropActive(false);
	this->player = player;
	readFile(fileName);
	keepTime = lifeTimeEachSentence.front();
}
Dialogue* ProtagThoughts::getDialogueObject() {
	return dialogueText;
}
void ProtagThoughts::activateDialogue() { // can only be active once
	isActiveDialogue = true;
	if (!lifeTimeEachSentence.empty()) {
		dialogueText->isDialogueActive = true;
	}
}

void ProtagThoughts::reActivateDialogue(string txtFile) {
	readFile(txtFile);
	keepTime = lifeTimeEachSentence.front();
	dialogueText->isEnd = false;
	activateDialogue();
}

void ProtagThoughts::stopDialogue() {
	dialogueText->isDialogueActive = false;
	dialogueText->isEnd = true;
}

void ProtagThoughts::update(list<DrawableObject*>& objectsList) {
	dialogueText->getTransform().setPosition(glm::vec3(GameEngine::getInstance()->getRenderer()->getCamera()->getPosition().x, -4.0f, 0.0f));
	dialogueText->update(objectsList);
	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	//if (abs(player->getTransform().getPosition().x - xActivatePos) < 0.2f) {
	//	if (!isActiveDialogue) {
	//		activateDialogue();
	//	}
	//}
	if (isActiveDialogue) {
		if (!lifeTimeEachSentence.empty()) {
			if (keepTime <= 0) {
				lifeTimeEachSentence.pop();
				if (!lifeTimeEachSentence.empty()) {
					keepTime = lifeTimeEachSentence.front();
				}
				dialogueText->nextSentence();
			}
			else {
				keepTime -= dt;
			}
		}
	}
	if (lifeTimeEachSentence.empty()) {
		dialogueText->isDialogueActive = false;
	}
}

void ProtagThoughts::render(glm::mat4 globalModelTransform) {
	//if (isActiveDialogue) {
	//	dialogueText->render(globalModelTransform);
	//}
	dialogueText->render(globalModelTransform);
}