#include "LightSource.h"
#include "GameEngine.h"
#include "SquareBorderMesh.h"

LightSource::LightSource(float brightness, float maxDistance): brightness(brightness), maxDistance(maxDistance) {
	this->setName("LightSource");
}

float LightSource::getBrightness() const {
	return brightness;
}

float LightSource::getMaxDistance() const {
	return maxDistance;
}

void LightSource::setBrightness(float brightness) {
	this->brightness = brightness;
}

void LightSource::setMaxDistance(float distance) {
	this->maxDistance = distance;
}

float LightSource::getTextureBrightness(DrawableObject* obj) {
	float distance = glm::length(this->getTransform().getPosition() - obj->getTransform().getPosition());

	if (distance >= maxDistance) {
		return LightSourceData::MINIMUM_BRIGHTNESS;
	}

	float resultBrightness = (1.0f - (distance / maxDistance)) * this->brightness;
	
	if (resultBrightness <= LightSourceData::MINIMUM_BRIGHTNESS) {
		return LightSourceData::MINIMUM_BRIGHTNESS;
	}

	return resultBrightness;
}

float LightSource::normalizeBrightness(float brightness) {
	if (brightness <= LightSourceData::MINIMUM_BRIGHTNESS) {
		return LightSourceData::MINIMUM_BRIGHTNESS;
	}

	if (brightness > 1.0f) {
		return 1.0f;
	}

	return brightness;
}

void LightSource::render(glm::mat4 globalModelTransform) {

	if (!canDrawCollider) {
		return;
	}

	SquareBorderMesh* squareBorderMesh = dynamic_cast<SquareBorderMesh*> (GameEngine::getInstance()->getRenderer()->getMesh(SquareBorderMesh::MESH_NAME));

	GLuint modelMatixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();
	GLuint colorId = GameEngine::getInstance()->getRenderer()->getColorUniformId();

	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (renderModeId == -1) {
		cout << "Error: Can't set renderMode in TexturedObject " << endl;
		return;
	}
	if (colorId == -1) {
		cout << "Error: Can't set color " << endl;
		return;
	}

	vector <glm::mat4> matrixStack;

	//glm::mat4 currentMatrix = this->getTransformMat4();
	glm::mat4 currentMatrix = this->transform.getTransformMat4();
	//glm::mat4 currentMatrix = collider->getGlobalTransform(this->transform).getTransformMat4();

	if (squareBorderMesh != nullptr) {

		//currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, 1, 1, 0);
		glUniform1i(renderModeId, 0);
		squareBorderMesh->render();
	}
}