#pragma once
#include "TexturedObject.h"
#include "GLRenderer.h"
#include "SimpleObject.h"
#include "LivingEntity.h"
#include "PlayerObject.h"
#include "Button.h"

class UI : public TexturedObject {
	private:
		SimpleObject *healthBar = nullptr;
		Button* quitButton;
	public:
		UI();
		void initUI(list<DrawableObject*> &objectsList);
		void updateUI(PlayerObject &playerObject, glm::vec3 camPos);
};