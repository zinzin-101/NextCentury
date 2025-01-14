
#include "SimpleObject.h"
#include "LivingEntity.h"
#include "GameEngine.h"
#include "SquareMeshVbo.h"


SimpleObject::SimpleObject() {
	color = glm::vec3(0.0, 0.0, 0.0);
}

SimpleObject::SimpleObject(string name) {
	color = glm::vec3(0.0, 0.0, 0.0);
	DrawableObject::setName(name);
}

SimpleObject::~SimpleObject() {}

void SimpleObject::onCollisionStay(Collider* collider) {
	DrawableObject* obj = collider->getObject();
	LivingEntity* entity = dynamic_cast<LivingEntity*>(obj);
	std::cout << "marker colliding with " << obj->getName() << std::endl;
	/*if (entity != NULL) {
		std::cout << "marker colliding with " << obj->getName() << std::endl;
	} */
}

void SimpleObject::onCollisionExit(Collider* collider) {
	DrawableObject* obj = collider->getObject();
	LivingEntity* entity = dynamic_cast<LivingEntity*>(obj);
	std::cout << "marker collision exit from " << obj->getName() << std::endl;
	/*if (entity != NULL) {
		std::cout << "marker collision exit from " << obj->getName() << std::endl;
	}*/
}

void SimpleObject::onTriggerStay(Collider* collider) {
	DrawableObject* obj = collider->getObject();
	LivingEntity* entity = dynamic_cast<LivingEntity*>(obj);
	std::cout << "marker triggering with " << obj->getName() << std::endl;
	/*if (entity != NULL) {
		std::cout << "marker colliding with " << obj->getName() << std::endl;
	} */
}

void SimpleObject::onTriggerExit(Collider* collider) {
	DrawableObject* obj = collider->getObject();
	LivingEntity* entity = dynamic_cast<LivingEntity*>(obj);
	std::cout << "marker trigger exit from " << obj->getName() << std::endl;
	/*if (entity != NULL) {
		std::cout << "marker collision exit from " << obj->getName() << std::endl;
	}*/
}

void SimpleObject::setColor(float r, float g, float b) {
	color = glm::vec3(r, g, b);
}

void SimpleObject::render(glm::mat4 globalModelTransform) {
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (GameEngine::getInstance()->getRenderer()->getMesh(SquareMeshVbo::MESH_NAME));

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

	if (squareMesh != nullptr) {

		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, color.x, color.y, color.z);
		glUniform1i(renderModeId, 0);
		squareMesh->render();

	}
}

