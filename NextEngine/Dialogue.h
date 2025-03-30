#pragma once

#include "TextObject.h"
#include "SimpleObject.h"

class Dialogue : public DrawableObject
{
private:
	float activeRange;
	int fontsize;
	bool isActive;
	TextObject text;
	list<string> sentences;
	SimpleObject placeHolderBG;

public:
	Dialogue(int fontSize);
	void update(list<DrawableObject*>& objectsList);
	void render(glm::mat4 globalModelTransform);
	void nextSentence();
	void addSentence(string s);
	void setSentences(list<string> s);
	void checkActiveRange(glm::vec3 playerPos);
};