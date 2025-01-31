#pragma once
#include "TexturedObject.h"
#include "GLRenderer.h"
#include "SimpleObject.h"
#include "LivingEntity.h"
#include "PlayerObject.h"

class UI : public TexturedObject {
	private:
		SimpleObject *healthBar = nullptr;
	public:
		UI();
		void initUI(list<DrawableObject*> &objectsList);
		void updateUI(PlayerObject &playerObject);
};