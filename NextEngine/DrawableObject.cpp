#include "DrawableObject.h"
#include "TexturedObject.h"
#include "GameEngine.h"
#include "RayObject.h"
#include "SquareBorderMesh.h"
#include "SquareMeshVbo.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>

using namespace std;

Transform& DrawableObject::getTransform(){
	/*glm::mat4 transform = glm::mat4(1.0);
	transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0));
	transform = glm::scale(transform, glm::vec3(size.x, size.y, 1));
	return transform;*/
	
	return transform;
}

glm::mat4 DrawableObject::getTransformMat4() const {
	return transform.getTransformMat4();
}

DrawableObject::DrawableObject(){
	/*pos = glm::vec3(0.0, 0.0, 0.0);
	size = glm::vec3(1.0, 1.0, 1.0);*/

	this->name = "Object";
	this->transform = Transform();
	physics = nullptr;
	collider = nullptr;

	canDrawCollider = false;

	isActive = true;
	isMarkedForDelete = false;
	
	renderBrightness = 1.0f;
	renderOpacity = 1.0f;
}

DrawableObject::DrawableObject(string name) {
	/*pos = glm::vec3(0.0, 0.0, 0.0);
	size = glm::vec3(1.0, 1.0, 1.0);*/

	this->name = name;
	this->transform = Transform();
	physics = nullptr;
	collider = nullptr;

	canDrawCollider = false;

	isActive = true;
	isMarkedForDelete = false;

	renderBrightness = 1.0f;
}

DrawableObject::~DrawableObject() {
	if (physics != nullptr) {
		delete physics;
		physics = nullptr;
	}

	if (collider != nullptr) {
		delete collider;
		collider = nullptr;
	}
}

string DrawableObject::getName() const {
	return this->name;
}

void DrawableObject::setName(string name) {
	this->name = name;
}

Physics* DrawableObject::getPhysicsComponent() const {
	if (this->physics == nullptr) {
		//cout << "Physics component does not exist" << endl;
		return nullptr;
	}

	return this->physics;
}
Collider* DrawableObject::getColliderComponent() const {
	if (this->collider == nullptr) {
		//cout << "Collider component does not exist" << endl;
		return nullptr;
	}

	return this->collider;
}

void DrawableObject::addPhysicsComponent() {
	if (this->physics != nullptr) {
		cout << "Physics component already exists" << endl;
		return;
	}

	this->physics = new Physics(this->transform);
}

void DrawableObject::addPhysicsComponent(Physics& physics) {
	if (this->physics != nullptr) {
		cout << "Physics component already exists" << endl;
		return;
	}

	this->physics = new Physics(this->transform);
	*(this->physics) = physics;
}

void DrawableObject::addColliderComponent() {
	if (this->collider != nullptr) {
		cout << "Collider component already exists" << endl;
		return;
	}

	this->collider = new Collider(this);
}

void DrawableObject::addColliderComponent(Collider& collider) {
	if (this->collider != nullptr) {
		cout << "Collider component already exists" << endl;
		return;
	}

	this->collider = new Collider(this);
	this->collider->setTransform(collider.getTransform());
	this->collider->setWidth(collider.getWidth());
	this->collider->setHeight(collider.getHeight());
}

/// For implementation in derived class ///
void DrawableObject::onCollisionEnter(Collider* collider){}
void DrawableObject::onCollisionStay(Collider* collider){}
void DrawableObject::onCollisionExit(Collider* collider){}
void DrawableObject::onTriggerEnter(Collider* collider){}
void DrawableObject::onTriggerStay(Collider* collider){}
void DrawableObject::onTriggerExit(Collider* collider){}

void DrawableObject::processCollider() {
	if (collider == nullptr) {
		return;
	}

	map<Collider*, Collider::CollisionState>& colliderMap = collider->getCollisionMap();

	for (pair<Collider*, Collider::CollisionState> pair : colliderMap) {
		Collider* col = pair.first;

		Collider::CollisionState state = pair.second;
		switch (state) {
			case Collider::ENTER:
				col->isTrigger() ? onTriggerEnter(col) : onCollisionEnter(col);
				break;
			case Collider::STAY:
				col->isTrigger() ? onTriggerStay(col) : onCollisionStay(col);
				break;
			case Collider::EXIT:
				col->isTrigger() ? onTriggerExit(col) : onCollisionExit(col);
				break;
		}
	}
}

//void DrawableObject::updateBehavior() {}

void DrawableObject::start(list<DrawableObject*>& objectsList) {

}

void DrawableObject::update(list<DrawableObject*>& objectsList) {
	processCollider();
	
	if (physics != nullptr) {
		physics->update(this->transform);
	}

	TexturedObject* texturedObj = dynamic_cast<TexturedObject*>(this);
	if (texturedObj != NULL) {
		Animation* anim = texturedObj->getAnimationComponent();
		if (anim != nullptr) {
			anim->updateCurrentState();
		}
	}
}

void DrawableObject::setDrawCollider(bool value) {
	canDrawCollider = value;
}

void DrawableObject::drawCollider() {
	if (collider == nullptr) {
		return;
	}

	if (!canDrawCollider) {
		return;
	}

	RayObject* ray = dynamic_cast<RayObject*>(this);

	if (ray != NULL) {
		return;
	}

	SquareBorderMesh* squareBorderMesh = dynamic_cast<SquareBorderMesh*> (GameEngine::getInstance()->getRenderer()->getMesh(SquareBorderMesh::MESH_NAME));

	GLuint modelMatixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();
	GLuint colorId = GameEngine::getInstance()->getRenderer()->getColorUniformId();
	GLuint brightnessId = GameEngine::getInstance()->getRenderer()->getBrightnessUniformId();


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
	Transform colTransform = collider->getGlobalTransform(this->transform);
	colTransform.scales(collider->getWidth(), collider->getHeight());
	glm::mat4 currentMatrix = colTransform.getTransformMat4();
	//glm::mat4 currentMatrix = collider->getGlobalTransform(this->transform).getTransformMat4();

	if (squareBorderMesh != nullptr) {

		//currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, 0, 1, 0);
		glUniform1f(brightnessId, 1);
		glUniform1i(renderModeId, 0);
		squareBorderMesh->render();
	}
}

void DrawableObject::setActive(bool value) {
	isActive = value;

	if (collider != nullptr && !isActive) {
		std::map<Collider*, Collider::CollisionState>& colMap = collider->getCollisionMap();
		for (std::pair<Collider*, Collider::CollisionState> pair : colMap) {
			colMap[pair.first] = Collider::EXIT;
		}
	}
}

bool DrawableObject::getIsActive() const {
	return isActive;
}

bool DrawableObject::getCanDrawCollider() const {
	return canDrawCollider;
}

bool DrawableObject::getMarkedForDelete() const {
	return isMarkedForDelete;
}

void DrawableObject::setRenderBrightness(float brightness) {
	this->renderBrightness = brightness;
}

void DrawableObject::setRenderOpacity(float opacity) {
	this->renderOpacity = opacity;
}

void DrawableObject::destroyObject(DrawableObject* obj) {
	obj->isMarkedForDelete = true;
	obj->setActive(false);
}