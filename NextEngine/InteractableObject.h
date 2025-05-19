#pragma once
#include "TexturedObject.h"
#include "Dialogue.h"
#include <fstream>

using namespace std;

class InteractableObject : public TexturedObject {
	vector<Dialogue*> txtEachLine;
	PlayerObject* player;
	bool isClickable;
	bool isShowingTxt;
	TexturedObject* Backdrop;
	TexturedObject* BackdropText;
	float offSetWidth;
	float offSetHeight;
public:
	InteractableObject(string describe, PlayerObject* player);
	void update(list<DrawableObject*>& objectsList);
	void setDescriptionActive(bool b);
	void insertTextInObjectList(list<DrawableObject*>& objectsList);
	bool getDescriptionActive();
	bool getIsClickable();
	void render(glm::mat4 globalModelTransform);
	void setOffsetWidth(float x);
	void setOffsetHeith(float y);
	TexturedObject* getBackdrop();
	TexturedObject* getBackdropText();
};