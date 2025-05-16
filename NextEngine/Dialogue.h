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
	
	TexturedObject Backdrop;
	PlayerObject* player;

public:
	queue<string> sentences;
	bool isDialogueActive;
	Dialogue(int fontSize, PlayerObject *player, bool isActiveByPosition);
	void update(list<DrawableObject*>& objectsList);
	void render(glm::mat4 globalModelTransform);
	void nextSentence();
	void addSentence(string s);
	void setSentences(queue<string> s);
	void endDialogue();
	void checkActiveRange();

};