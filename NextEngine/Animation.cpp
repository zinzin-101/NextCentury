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

	timeSinceLastFrame = 0.0f;
	timePerFrame = AnimationData::DEFAULT_TIME_PER_FRAME;
}

void Animation::render(glm::mat4 globalModelTransform, Transform& transform, const glm::vec4& colorOverlay, const float brightness) {
	SquareMeshVbo* squareMesh = dynamic_cast<SquareMeshVbo*> (GameEngine::getInstance()->getRenderer()->getMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();
	GLuint offsetXId = GameEngine::getInstance()->getRenderer()->getOffsetXUniformId();
	GLuint offsetYId = GameEngine::getInstance()->getRenderer()->getOffsetYUniformId();
	GLuint scaleXId = GameEngine::getInstance()->getRenderer()->getScaleXUniformId();
	GLuint scaleYId = GameEngine::getInstance()->getRenderer()->getScaleYUniformId();
	GLint colorOverlayId = GameEngine::getInstance()->getRenderer()->getColorOverlayUniformId();
	GLuint brightnessId = GameEngine::getInstance()->getRenderer()->getBrightnessUniformId();

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
		glUniform4f(colorOverlayId, colorOverlay.x, colorOverlay.y, colorOverlay.z, colorOverlay.w);
		glUniform1f(brightnessId, brightness);
		glBindTexture(GL_TEXTURE_2D, texture);
		squareMesh->render();

	}
}

void Animation::setDimension(int row, int col) {
	this->rowCount = row;
	this->colCount = col;
}

void Animation::setTimePerFrame(float t) {
	timePerFrame = t;
}

void Animation::setFrame(int row, int column) {
	offsetX = (1.0f / this->colCount) * column;
	offsetY = (1.0f / this->rowCount) * row;
}

void Animation::addState(string name, int row, int startCol, int frameCount, bool canLoop) {
	if (states.find(name) != states.end()) {
		cout << "Error state: '" << name << "' already exists" << endl;
		return;
	}
	State newState(name, row, startCol, frameCount, canLoop, timePerFrame);
	states[name] = newState;

	if (states.size() == 1) {
		currentState = &states[name];
	}
}

void Animation::addState(string name, int row, int startCol, int frameCount, bool canLoop, float customTimePerFrame) {
	if (states.find(name) != states.end()) {
		cout << "Error state: '" << name << "' already exists" << endl;
		return;
	}
	State newState(name, row, startCol, frameCount, canLoop, customTimePerFrame);
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
		timeSinceLastFrame = 0.0f;

		setFrame(currentState->row, currentState->currentFrame + currentState->startCol);

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
	timeSinceLastFrame += dt;

	if (timeSinceLastFrame > currentState->timePerFrame) {
		setFrame(currentState->row, currentState->currentFrame + currentState->startCol);
		currentState->currentFrame++;
		timeSinceLastFrame = 0.0f;
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

Animation::State& Animation::getCurrentAnimationStateRef() {
	return *currentState;
}

Animation::State Animation::getAnimationState(string name) {
	if (states.count(name) == 0) {
		std::cout << "State does not exist" << std::endl;
		return State();
	}

	return states[name];
}

Animation::State& Animation::getAnimationStateRef(string name) {
	if (states.count(name) == 0) {
		std::cout << "State does not exist" << std::endl;
		return states[name];
	}

	return states[name];
}

float Animation::getTimeRate() const {
	return timePerFrame;
}