#pragma once
#include "LivingEntity.h"
#include "Dialogue.h"

struct Chat {
	Dialogue* titleText;
	vector<Dialogue*> dialogueText;
	int talkerIndex;
	bool isFlip;
};

class ChatBubble {
	queue<Chat> chats;
	TexturedObject* bubble;
	vector<glm::vec3> eachTalker;
	float offsetX;
	queue<float> timeAppearEachChat;
	bool isChatting;
	bool isEnd;
public:
	ChatBubble(string file, PlayerObject* player, vector<glm::vec3> talkers, list<DrawableObject*>& objectsList);
	void runChat(list<DrawableObject*>& objectsList);
	void nextChat();
	void setCurrentChatPos(glm::vec3 pos); // SET the bubble first then translate the other objects, THIS can be run at update all the time with pos being currentChatPos.
	void activateChat();
	void setOffSetX(float x);
	bool hasEnded();
	void skipSentence();
};