#pragma once
#include "TexturedObject.h"
#include "Dialogue.h"

class InteractableObject : public TexturedObject {
	string description;
	PlayerObject* player;
	bool isClickable;
public:
	InteractableObject(string description, PlayerObject* player);
	void update(list<DrawableObject*>& objectsList);
	void setDescriptionActive(bool b);
	bool getDescriptionActive();
	Dialogue* descriptionText;
};