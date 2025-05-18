#pragma once
#include "Dialogue.h"

class ProtagThoughts : public DrawableObject { // THIS CLASS IS INTENDED TO BE USE IN THE LEVEL.H AND .CPP
	Dialogue* dialogueText;
	bool isActiveDialogue;
	queue<float> lifeTimeEachSentence;
	int fontSize;
	float keepTime = 0;
	PlayerObject* player;
public:
	ProtagThoughts(string fileName, PlayerObject* player);
	Dialogue* getDialogueObject();
	void update(list<DrawableObject*>& objectsList);
	void render(glm::mat4 globalModelTransform);
	void activateDialogue();
};