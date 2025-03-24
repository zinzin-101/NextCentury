#include "TexturedObject.h"
#include "GameEngine.h"
#include "SquareMeshVbo.h"


TexturedObject::TexturedObject(): animation(nullptr), colorOverlay(glm::vec4()) {}

TexturedObject::TexturedObject(string name) : animation(nullptr), colorOverlay(glm::vec4()) {
	DrawableObject::setName(name);
}


TexturedObject::~TexturedObject() {
	if (animation != nullptr) {
		delete animation;
	}
}

void TexturedObject::setTexture(string path) {
	texture = GameEngine::getInstance()->getRenderer()->LoadTexture(path);
}

void TexturedObject::render(glm::mat4 globalModelTransform) {
	if (animation != nullptr) {
		animation->render(globalModelTransform, this->transform, colorOverlay, renderBrightness);
		return;
	}

	SquareMeshVbo* squareMesh = dynamic_cast<SquareMeshVbo*> (GameEngine::getInstance()->getRenderer()->getMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();
	/*GLuint offsetXId = GameEngine::getInstance()->getRenderer()->getOffsetXUniformId();
	GLuint offsetYId = GameEngine::getInstance()->getRenderer()->getOffsetYUniformId();*/
	GLuint scaleXId = GameEngine::getInstance()->getRenderer()->getScaleXUniformId();
	GLuint scaleYId = GameEngine::getInstance()->getRenderer()->getScaleYUniformId();
	GLint colorOverlayId = GameEngine::getInstance()->getRenderer()->getColorOverlayUniformId();
	GLfloat brightnessId = GameEngine::getInstance()->getRenderer()->getBrightnessUniformId();

	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (renderModeId == -1) {
		cout << "Error: Can't set renderMode in TexturedObject " << endl;
		return;
	}
	/*if (offsetXId == -1) {
		cout << "Error: Can't set offsetXId in TexturedObject " << endl;
		return;
	}*/

	vector <glm::mat4> matrixStack;

	glm::mat4 currentMatrix = this->getTransformMat4();

	if (squareMesh != nullptr) {

		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(renderModeId, 1);
		glUniform4f(colorOverlayId, colorOverlay.x, colorOverlay.y, colorOverlay.z, colorOverlay.w);
		glUniform1f(brightnessId, renderBrightness);
		glBindTexture(GL_TEXTURE_2D, texture);
		squareMesh->render();

	}
}

void TexturedObject::initAnimation(int rowCount, int colCount) {
	animation = new Animation(texture);
	animation->setDimension(rowCount, colCount);
}

Animation* TexturedObject::getAnimationComponent() {
	return animation != nullptr ? animation : nullptr;
}

bool TexturedObject::isAnimated() {
	return animation != nullptr;
}

//void TexturedObject::addAnimation(int rowCount, int colCount) {
//	animated = true;
//	this->rowCount = rowCount;
//	this->colCount = colCount;
//}
//
//void TexturedObject::setFrame(float row, float column) {
//	offsetX = (1.0f / this->rowCount) * row;
//	offsetY = (1.0f / this->colCount) * column;
//}