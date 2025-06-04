#pragma once
#include "Dialogue.h"
#include "DrawableObject.h"

class GotItemText{
	Dialogue* text;
	float appearTime;
	bool isAppeared;
	float appearPosX;
	float appearPosY;
	bool isReturning;
public:
	GotItemText(string itemText, PlayerObject* player, list<DrawableObject*>& objectsList);
	void update(list<DrawableObject*>& objectsList);
	void activateAppear();
	void setAppearPos(float x, float y);
	void setAppearTime(float t);
	bool getIsReturning();
};