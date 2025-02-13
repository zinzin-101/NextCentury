#include "Animation.h"
#include "GameEngine.h"
#include "SquareMeshVbo.h"
#include <stdlib.h>

using namespace std;

Animation::Animation(unsigned int& texture): texture(texture) {
	offsetX = 0;
	offsetY = 0;

	rowCount = 1;
	colCount = 1;

	isPaused = false;

	currentState = nullptr;
}

void Animation::render(glm::mat4 globalModelTransform, Transform& transform) {
	SquareMeshVbo* squareMesh = dynamic_cast<SquareMeshVbo*> (GameEngine::getInstance()->getRenderer()->getMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();
	GLuint offsetXId = GameEngine::getInstance()->getRenderer()->getOffsetXUniformId();
	GLuint offsetYId = GameEngine::getInstance()->getRenderer()->getOffsetYUniformId();
	GLuint scaleXId = GameEngine::getInstance()->getRenderer()->getScaleXUniformId();
	GLuint scaleYId = GameEngine::getInstance()->getRenderer()->getScaleYUniformId();

	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (renderModeId == -1) {
		cout << "Error: Can't set renderMode in TexturedObject " << endl;
		return;
	}
	if (offsetXId == -1) {
		cout << "Error: Can't set offsetXId in TexturedObject " << endl;
		return;
	}

	vector <glm::mat4> matrixStack;

	glm::mat4 currentMatrix = transform.getTransformMat4();

	if (squareMesh != nullptr) {

		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(renderModeId, 2);
		glUniform1f(offsetXId, offsetX);
		glUniform1f(offsetYId, offsetY);
		glUniform1f(scaleXId, 1.0f / this->colCount); // bigger scale zoom out, small scale zoom in
		glUniform1f(scaleYId, 1.0f / this->rowCount);
		glBindTexture(GL_TEXTURE_2D, texture);
		squareMesh->render();

	}
}

void Animation::setDimension(int row, int col) {
	this->rowCount = row;
	this->colCount = col;
}

void Animation::setFrame(int row, int column) {
	offsetX = (1.0f / this->colCount) * column; // MA BRAIN SWITCH ROW AND COLUMN SO I SWITCH EM HERE
	offsetY = (1.0f / this->rowCount) * row;
}

void Animation::addState(string name, int row, int frameCount, bool canLoop) {
	if (states.find(name) != states.end()) {
		cout << "Error state: '" << name << "' already exists" << endl;
		return;
	}
	State newState(name, row, frameCount, canLoop);
	states[name] = newState;

	if (states.size() == 1) {
		currentState = &states[name];
	}
}

void Animation::setState(string name) {
	if (states.find(name) == states.end()) {
		cout << "Error state: " << name << " does not exist" << endl;
		return;
	}

	State* nextState = &states[name];
	if (nextState->name != currentState->name || currentState == nullptr) {
		nextState->currentFrame = 0;
		nextState->isPlaying = true;
		currentState = nextState;
		return;
	}
}

void Animation::updateCurrentState() {
	if (currentState == nullptr) {
		return;
	}

	if (isPaused) {
		setFrame(currentState->row, currentState->currentFrame);
		return;
	}

	if (currentState->currentFrame >= currentState->frameCount) {
		if (currentState->canLoop) {
			currentState->currentFrame = 0;
		}
		else {
			currentState->currentFrame = currentState->frameCount - 1;
			currentState->isPlaying = false;
		}
	}

	float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
	timeRateKeep += dt;

	if (timeRateKeep > timeRate) { // Will need to change to frame I think
		setFrame(currentState->row, currentState->currentFrame);
		currentState->currentFrame++;
		timeRateKeep = 0.0f;
	}
}

void Animation::setRandomFrame() {
	int randomFrame = std::rand() % currentState->frameCount;
	setFrame(currentState->row, randomFrame);
}

void Animation::setPaused(bool value) {
	isPaused = value;
}

int Animation::getCurrentFrame() const {
	return currentState->currentFrame;
}

bool Animation::getIsPlaying() const {
	return currentState->isPlaying;
}

Animation::State Animation::getCurrentAnimationState() const {
	return *currentState;
}