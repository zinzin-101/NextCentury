#pragma once
#include "TexturedObject.h"
#include "Dialogue.h"
#include <fstream>

using namespace std;

class InteractableObject : public TexturedObject {
	vector<Dialogue> txtEachLine;
	PlayerObject* player;
	bool isClickable;
	bool isShowingTxt;
	TexturedObject Backdrop;
public:
	InteractableObject(string describe, PlayerObject* player);
	void update(list<DrawableObject*>& objectsList);
	void setDescriptionActive(bool b);
	bool getDescriptionActive();
	bool getIsClickable();
	void render(glm::mat4 globalModelTransform);
	Dialogue* descriptionText;
};