#pragma once

#include "DrawableObject.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <SDL_ttf.h>
#include <SDL.h>

class TextObject : virtual public DrawableObject
{
private:
	unsigned int texture;
	string text;
	int fontStyle;

public:
	TextObject();
	~TextObject();
	void render(glm::mat4 globalModelTransform);
	void update(float deltaTime);
	void loadText(string text, SDL_Color textColor, int fontSize);
	static void InitSDL();
	static void cleanSDL();
	string getText();
	void setFontStyleNormal();
	void setFontStyleBold();
};