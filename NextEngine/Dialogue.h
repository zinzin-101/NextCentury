#pragma once

#include "TextObject.h"
#include "SimpleObject.h"
#include "PlayerObject.h"
#include <queue>

class Dialogue : public DrawableObject
{
private:
	float activeRange;
	int fontsize;
	bool isActiveByPosition;
	TextObject text;
	bool isBackdropActive;
	
	TexturedObject Backdrop;
	PlayerObject* player;

public:
	queue<string> sentences;
	bool isDialogueActive;
	bool isEnd;
	Dialogue(int fontSize, PlayerObject *player, bool isActiveByPosition);
	glm::vec3 getTextScale();
	void update(list<DrawableObject*>& objectsList);
	void render(glm::mat4 globalModelTransform);
	void nextSentence();
	void addSentence(string s);
	void setSentences(queue<string> s);
	void endDialogue();
	void checkActiveRange();
	void setBackDropActive(bool b);
	void setDialogueTextBold();
	void setDialogueTextNormal();
};