#include "SonicWave.h"

SonicWave::SonicWave() {
	initiated = false;
	currentPosition = glm::vec3();
	groundMarker = nullptr;

	/// set texture and init animation ///
	//+
}

void SonicWave::start(std::list<DrawableObject*>& objectsList) {
	groundMarker = new ColliderObject();
	groundMarker->setName("SonicMarker");
	groundMarker->setDrawColliderColor(glm::vec3(1, 0, 0));
	groundMarker->setDrawCollider(true);
	objectsList.emplace_back(groundMarker);
}

void SonicWave::onCollisionEnter(Collider* collider) {

}
void SonicWave::trigger(glm::vec3 position) {

}

void SonicWave::update(std::list<DrawableObject*>& objectsList) {
	DrawableObject::update(objectsList);
	
	//+
}