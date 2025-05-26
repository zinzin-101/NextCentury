#include "ChatBubble.h"
#include <fstream>
#include <iostream>
#include "GameEngine.h"

ChatBubble::ChatBubble(string fileName, PlayerObject* player, vector<glm::vec3> talkers, list<DrawableObject*>& objectsList) {
	bubble = new TexturedObject();
	bubble->setTexture("../Resource/Texture/StoryStuff/ChatBubble.png");
	bubble->getTransform().setScale(2.76f, 1.7f); // 2.76, 1.5
	objectsList.emplace_back(bubble);
	bubble->setActive(false);

	eachTalker = talkers;

	offsetX = 0;

	isChatting = true;
	isEnd = false;

	string myText;
	ifstream meFile(fileName);

	if (!meFile.is_open()) {
		cout << "File Not Open Interactable Object." << endl;
		return;
	}

	// First line is Title. Title have fixed font
	// Second line is TalkerIndex.(0->n)
	// Third line is bool isFlip. (0, 1);
	// Forth line is timeAppear for that chat.
	// After the that every line is a line in chatBox. (Locked font);
	// If the line is NEXTCHAT move on to creating a new chat.
	/// DON'T FORGET TO ADD TO OBJECTLIST.

	Chat c;

	bool nextChat = true;
	
	while (getline(meFile, myText)) {
		if (nextChat) {
			Dialogue* tText = new Dialogue(24, player, false);
			tText->setBackDropActive(false);
			tText->addSentence(myText);
			tText->isDialogueActive = false;
			objectsList.emplace_back(tText);

			c.titleText = tText;

			getline(meFile, myText);
			c.talkerIndex = stoi(myText);

			getline(meFile, myText);

			c.isFlip = stoi(myText); // 0 or 1
			//cout << myText << " : " << c.isFlip;

			getline(meFile, myText);
			timeAppearEachChat.push(stof(myText));

			getline(meFile, myText);

			nextChat = false;
		}

		if (myText == "NEXTCHAT") {
			chats.push(c);
			c.dialogueText.clear();
			nextChat = true;
			continue;
		}

		Dialogue* k = new Dialogue(16, player, false);
		k->setBackDropActive(false);
		k->addSentence(myText);
		k->isDialogueActive = false;
		objectsList.emplace_back(k);
		c.dialogueText.emplace_back(k);
	}
}
void ChatBubble::runChat(list<DrawableObject*>& objectsList) {
	
	if (chats.empty()) {
		bubble->setActive(false);
		isEnd = true;
		return;
	}

	bubble->setActive(true);

	glm::vec3 currentTalkerPos = eachTalker.at(chats.front().talkerIndex);
	setCurrentChatPos(glm::vec3(currentTalkerPos.x, currentTalkerPos.y, 0.0f));

	float offsetY = (bubble->getTransform().getPosition().y + 0.225f) * 120.0f;
	for (int i = 0; i < chats.front().dialogueText.size(); i++) {
		float offsetX = (bubble->getTransform().getPosition().x) + ((chats.front().dialogueText[i]->getTextScale().x / 2.0f) / 120.0f) - 1.2f;
		chats.front().dialogueText[i]->getTransform().setPosition(glm::vec3(offsetX, (offsetY / 120.0f), 0.0f));
		offsetY += chats.front().dialogueText[i]->getTextScale().y;
		chats.front().dialogueText[i]->update(objectsList);
	}
	
	chats.front().titleText->update(objectsList);

	//if (isChatting) { SOME HOW UPDATE RUNS JUST STARTED ITSELF
		if (!timeAppearEachChat.empty()) {
			if (timeAppearEachChat.front() < 0.0f) {
				nextChat();
				timeAppearEachChat.pop();
			}
			else {
				timeAppearEachChat.front() -= GameEngine::getInstance()->getTime()->getDeltaTimeRealTime();
			}
		}
		if (!chats.empty()) {
			for (Dialogue* p : chats.front().dialogueText) {
				p->isDialogueActive = true;
			}
			chats.front().titleText->isDialogueActive = true;
		}
	//}
}

void ChatBubble::nextChat() {
	chats.front().titleText->isDialogueActive = false;
	for (Dialogue* p : chats.front().dialogueText) {
		p->isDialogueActive = false;
	}
	chats.pop();
}

void ChatBubble::setCurrentChatPos(glm::vec3 pos) {
	int j;
	if (chats.front().isFlip) {
		j = -1;
	}
	else {
		j = 1;
	}
	if (bubble->getTransform().getScale().x > 0.0f) {
		bubble->getTransform().setScale(glm::vec3(bubble->getTransform().getScale().x * j, bubble->getTransform().getScale().y, 0.0f));
	}
	bubble->getTransform().setPosition(glm::vec3(pos.x + offsetX, pos.y, pos.z));
	chats.front().titleText->getTransform().setPosition(glm::vec3(bubble->getTransform().getPosition().x, bubble->getTransform().getPosition().y + 0.6f, 0.0f));
}

void ChatBubble::activateChat() {
	//cout << "active?" << endl;
	bubble->setActive(true);
	isChatting = true;
}

void ChatBubble::setOffSetX(float x) {
	offsetX = x;
}

bool ChatBubble::hasEnded() {
	return isEnd;
}