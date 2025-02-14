#include "RayObject.h"
#include "GameEngine.h"

RayObject::RayObject(glm::vec3 pos, glm::vec3 dir, float length): origin(pos), length(length) {
	this->addColliderComponent();
	this->getColliderComponent()->setTrigger(true);
	dir = glm::normalize(dir);
	this->dir = dir;
}

void RayObject::setRayOrigin(glm::vec3 pos){
	this->origin = pos;
}

void RayObject::setRayDirection(glm::vec3 dir){
	this->dir = dir;
}

void RayObject::setRayLength(float length) {
	this->length = length;
}

glm::vec3 RayObject::getRayOrigin() const{
	return origin;
}

glm::vec3 RayObject::getRayDirection() const {
	return dir;
}

float RayObject::getRayLength() const {
	return length;
}

void RayObject::render(glm::mat4 globalModelTransform) {
	if (!canDrawCollider) {
		return;
	}

	GLuint modelMatixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint colorId = GameEngine::getInstance()->getRenderer()->getColorUniformId();
	GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();

	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (colorId == -1) {
		cout << "Error: Can't set color " << endl;
		return;
	}
	if (renderModeId == -1) {
		cout << "Error: Can't set renderMode in TexturedObject " << endl;
		return;
	}
	//vector <glm::mat4> matrixStack;

	glm::mat4 currentMatrix = this->getTransformMat4();

	float endX = this->dir.x * length;
	float endY = this->dir.y * length;

	currentMatrix = globalModelTransform * currentMatrix;
	glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
	glUniform3f(colorId, 0, 1, 1);
	glUniform1i(renderModeId, 0);
	
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(endX, endY);
	glEnd();
}

void RayObject::onCollisionEnter(Collider* collider) {

}

void RayObject::onCollisionStay(Collider* collider) {

}

void RayObject::onCollisionExit(Collider* collider) {

}

void RayObject::onTriggerEnter(Collider* collider) {

}

void RayObject::onTriggerStay(Collider* collider) {

}

void RayObject::onTriggerExit(Collider* collider) {

}