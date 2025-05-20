#pragma once
#include "Dialogue.h"
#include "DrawableObject.h"

class GotItemText{
	Dialogue* text;
	float appearTime;
	bool isAppeared;
public:
	GotItemText(string itemText, PlayerObject* player, list<DrawableObject*>& objectsList);
	void update(list<DrawableObject*>& objectsList);
	void activateAppear();
};