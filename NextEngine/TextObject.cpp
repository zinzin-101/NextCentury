#include "pch.h"
#include "TextObject.h"
#include "GameEngine.h"
#include "SquareMeshVbo.h"
#include <sstream>

TextObject::TextObject()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


TextObject::~TextObject()
{

}

void TextObject::render(glm::mat4 globalModelTransform)
{
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (GameEngine::getInstance()->getRenderer()->getMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();

	glBindTexture(GL_TEXTURE_2D, texture);
	//glBindTexture(GL_TEXTURE_2D, texNum);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	//glBindTexture(GL_TEXTURE_2D, texture);
	//cout << "Render\n";


	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (renderModeId == -1) {
		cout << "Error: Can't set renderMode in TexturedObject " << endl;
		return;
	}

	glm::mat4 currentMatrix = this->getTransformMat4();

	glm::mat4 flipMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));
	currentMatrix = flipMatrix * currentMatrix;

	if (squareMesh != nullptr) {

		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(renderModeId, 1);
		glBindTexture(GL_TEXTURE_2D, texture);
		squareMesh->render();
	}
}

void TextObject::update(float deltaTime)
{

}

void TextObject::loadText(string text, SDL_Color textColor, int fontSize)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	TTF_Font* font = TTF_OpenFont("../Resource/Texture/Times New Romance.ttf", fontSize);
	if (font == nullptr) {
		cout << "Error: Unable to open font: " << TTF_GetError() << endl;
		return;
	}

	// Split text by \n
	vector<string> lines;
	stringstream ss(text);
	string line;
	while (getline(ss, line, '\n')) {
		lines.push_back(line);
	}

	// Render each line to a surface and combine vertically
	vector<SDL_Surface*> surfaces;
	int totalHeight = 0;
	int maxWidth = 0;

	for (string& l : lines) {
		SDL_Surface* surface = TTF_RenderText_Blended(font, l.c_str(), textColor);
		if (!surface) continue;
		totalHeight += surface->h;
		if (surface->w > maxWidth) maxWidth = surface->w;
		surfaces.emplace_back(surface);
	}

	// Create a final combined surface
	SDL_Surface* finalSurface = SDL_CreateRGBSurface(0, maxWidth, totalHeight, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	int yOffset = 0;
	for (SDL_Surface* s : surfaces) {
		SDL_Rect destRect = { 0, yOffset, s->w, s->h };
		SDL_BlitSurface(s, NULL, finalSurface, &destRect);
		yOffset += s->h;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, finalSurface->w, finalSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, finalSurface->pixels);
	this->getTransform().setScale(glm::vec3(finalSurface->w, -finalSurface->h, 0));

	// Cleanup
	for (SDL_Surface* s : surfaces) {
		SDL_FreeSurface(s);
	}
	SDL_FreeSurface(finalSurface);
	TTF_CloseFont(font);

	this->text = text;
}

void TextObject::InitSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	// Initialize SDL_ttf
	if (TTF_Init() == -1) {
		std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
		return;
	}

	cout << "TextObject : Initialize SDL successful" << endl;
	return;
}

void TextObject::cleanSDL() {
	TTF_Quit();
	SDL_Quit();
}

string TextObject::getText() {
	return this->text;
}