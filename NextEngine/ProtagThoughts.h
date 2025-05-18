#pragma once
#include "Dialogue.h"

class ProtagThoughts : public DrawableObject {
	Dialogue* dialogueText;
	bool isActiveDialogue;
	queue<float> lifeTimeEachSentence;
	int fontSize;
	float xActivatePos;
	float keepTime = 0;
	PlayerObject* player;
public:
	ProtagThoughts(string fileName, PlayerObject* player, float x);
	Dialogue* getDialogueObject();
	void update(list<DrawableObject*>& objectsList);
	void render(glm::mat4 globalModelTransform);
	void activateDialogue();
};