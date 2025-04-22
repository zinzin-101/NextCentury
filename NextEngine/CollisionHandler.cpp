#include "CollisionHandler.h"

constexpr unsigned int COLLISION_RESOLUTION = 4;

bool checkCollision(Collider* col1, Transform& t1, Collider* col2, Transform& t2) {
	Transform globalT1 = col1->getGlobalTransform(t1);
	//globalT1.scales(col1->getWidth(), col1->getHeight());
	glm::vec3 pos1 = globalT1.getPosition();
	float halfWidth1 = abs(globalT1.getScale().x * col1->getWidth() / 2.0f);
	float halfHeight1 = abs(globalT1.getScale().y * col1->getHeight() / 2.0f);
	//unsigned char flag1 = col1->getCollisionFlag();

	Transform globalT2 = col2->getGlobalTransform(t2);
	//globalT2.scales(col2->getWidth(), col2->getHeight());
	glm::vec3 pos2 = globalT2.getPosition();
	float halfWidth2 = abs(globalT2.getScale().x * col2->getWidth() / 2.0f);
	float halfHeight2 = abs(globalT2.getScale().y * col2->getHeight() / 2.0f);
	//unsigned char flag2 = col2->getCollisionFlag();

	bool xAxis = abs(pos1.x - pos2.x) < (halfWidth1 + halfWidth2);
	bool yAxis = abs(pos1.y - pos2.y) < (halfHeight1 + halfHeight2);

	//cout << "check" << endl;

	bool collided = xAxis && yAxis;
	//updateCollisionState(col1, col2, collided);

	return collided;
}

bool checkCollisionRay(RayObject* ray1, Collider* col2, Transform& t2) {
	Transform globalT1 = ray1->getTransform();
	glm::vec3 pos1 = globalT1.getPosition() + ray1->getRayOrigin();
	glm::vec3 endPoint = pos1 + (ray1->getRayDirection() * ray1->getRayLength());

	bool isInside = checkCollisionPoint(col2, t2, pos1) || checkCollisionPoint(col2, t2, endPoint);

	if (isInside) {
		return true;
	}

	float x1 = pos1.x;
	float y1 = pos1.y;
	float x2 = endPoint.x;
	float y2 = endPoint.y;
	
	Transform globalT2 = col2->getGlobalTransform(t2);
	glm::vec3 pos2 = globalT2.getPosition();
	float halfWidth2 = abs(globalT2.getScale().x * col2->getWidth() / 2.0f);
	float halfHeight2 = abs(globalT2.getScale().y * col2->getHeight() / 2.0f);

	glm::vec3 up(0, halfHeight2, 0);
	glm::vec3 right(halfWidth2, 0, 0);

	glm::vec3 points[4] = {
		pos2 - right + up,
		pos2 + right + up,
		pos2 + right - up,
		pos2 - right - up
	};

	for (int i = 0; i < 4; i++) {
		float x3 = points[i].x;
		float y3 = points[i].y;
		float x4 = points[(i + 1) % 4].x;
		float y4 = points[(i + 1) % 4].y;

		// Line intersection formula
		float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
		float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

		if (0.0f <= t && t <= 1.0f && 0.0f <= u && u <= 1.0f) {
			return true;
		}
	}

	return false;
}

bool checkCollisionPoint(Collider* col, Transform& t, glm::vec2 point) {
	Transform globalT = col->getGlobalTransform(t);
	glm::vec3 pos = globalT.getPosition();
	float halfWidth = abs(globalT.getScale().x * col->getWidth() / 2.0f);
	float halfHeight = abs(globalT.getScale().y * col->getHeight() / 2.0f);

	bool xAxis = pos.x - halfWidth <= point.x && point.x <= pos.x + halfWidth;
	bool yAxis = pos.y - halfHeight <= point.y && point.y <= pos.y + halfHeight;

	//cout << "check pos: " << pos.x << " " << pos.y << "point: " << point.x << " " << point.y << endl;

	return xAxis && yAxis;
}

void updateCollisionState(Collider* col1, Collider* col2, bool collided) {
	std::map<Collider*, Collider::CollisionState>& colMap1 = col1->getCollisionMap();

	bool found = colMap1.find(col2) != colMap1.end() && colMap1.at(col2) != Collider::NONE;
	if (collided) {
		if (!found) {
			colMap1[col2] = Collider::ENTER;
			return;
		}

		colMap1[col2] = Collider::STAY;
		return;
	}

	if (found) {
		if (colMap1[col2] == Collider::EXIT) {
			//colMap1[col2] = Collider::NONE;
			std::map<Collider*, Collider::CollisionState>& colMap2 = col2->getCollisionMap();
			colMap1.erase(col2);
			colMap2.erase(col1);

			return;
		}

		colMap1[col2] = Collider::EXIT;
		return;
	}
}

void handleObjectCollision(list<DrawableObject*>& objects) {
	for (list<DrawableObject*>::iterator i = objects.begin(); i != objects.end(); i++) {

		if ((*i)->getColliderComponent() != nullptr) {
			(*i)->getColliderComponent()->setCollisionFlag(0);
		}

		for (list<DrawableObject*>::iterator j = objects.begin(); j != objects.end(); j++) {
			if (i == j) {
				continue;
			}

			DrawableObject* obj1 = *i;
			DrawableObject* obj2 = *j;

			RayObject* ray1 = dynamic_cast<RayObject*>(obj1);
			RayObject* ray2 = dynamic_cast<RayObject*>(obj2);
			if (ray1 != NULL && ray2 != NULL) {
				continue;
			}
			
			Collider* col1 = obj1->getColliderComponent();
			Collider* col2 = obj2->getColliderComponent();

			if (col1 == nullptr || col2 == nullptr) {
				continue;
			}

			if (!col1->isEnable() || !col2->isEnable()) {
				continue;
			}

			Transform t1 = obj1->getTransform();
			Transform t2 = obj2->getTransform();

			bool collided = false;

			Physics* phys1 = obj1->getPhysicsComponent();
			Physics* phys2 = obj2->getPhysicsComponent();

			if (phys1 != nullptr && phys2 != nullptr) {
				continue;
			}

			if (!obj1->getIsActive() || !obj2->getIsActive()) {
				collided = false;
			}
			else if (ray1 != NULL) {
				collided = checkCollisionRay(ray1, col2, t2);
			}
			else if (ray2 != NULL) {
				collided = checkCollisionRay(ray2, col1, t1);
			}
			else if (phys1 != nullptr) {
				Collider tempCol(nullptr);
				tempCol = *col1;
				glm::vec3 dir = t1.getPosition() - phys1->getLastPosition();
				dir /= (float)COLLISION_RESOLUTION;
				for (int k = 0; k < COLLISION_RESOLUTION; k++) {
					collided = checkCollision(&tempCol, t1, col2, t2);
					
					if (collided) {
						obj1->getTransform() = t1;
						break;
					}

					t1.translate(dir);
				}
			}
			else if (phys2 != nullptr) {
				Collider tempCol(nullptr);
				tempCol = *col2;
				glm::vec3 dir = t2.getPosition() - phys2->getLastPosition();
				dir /= (float)COLLISION_RESOLUTION;
				for (int k = 0; k < COLLISION_RESOLUTION; k++) {
					collided = checkCollision(&tempCol, t2, col1, t1);

					if (collided) {
						obj2->getTransform() = t2;
						break;
					}

					t2.translate(dir);
				}
			}
			else {
				collided = checkCollision(col1, t1, col2, t2);
			}

			updateCollisionState(col1, col2, collided);

			if (collided) {
				//system("cls");
				//cout << obj1->getName() << " collided with " << obj2->getName() << endl;

				if (col1->isTrigger() || col2->isTrigger()) {
					//cout << "Trigger collision" << endl;
					/// Do nothing
					continue;
				}

				resolveCollision(obj1, obj2);
			}
			//else {
			//	//system("cls");
			//	//cout << "no collision" << endl;
			//}
		}
	}
}

void resolveCollision(DrawableObject* obj1, DrawableObject* obj2) {
	Physics* phys1 = obj1->getPhysicsComponent();
	Physics* phys2 = obj2->getPhysicsComponent();

	if (phys1 == nullptr) {
		return;
	}

	if (phys1 != nullptr && phys2 != nullptr) {
		return; // currently ignoring collision between two physics objects
	}

	Transform& t1 = obj1->getTransform();
	Transform& t2 = obj2->getTransform();

	Collider* col1 = obj1->getColliderComponent();
	Collider* col2 = obj2->getColliderComponent();

	Transform globalT1 = col1->getGlobalTransform(t1);
	Transform globalT2 = col2->getGlobalTransform(t2);

	glm::vec3 pos1 = globalT1.getPosition();
	glm::vec3 pos2 = globalT2.getPosition();
	glm::vec3 lastPos = obj1->getPhysicsComponent()->getLastPosition();

	glm::vec3 scale1 = globalT1.getScale();
	float halfWidth1 = abs(scale1.x) * col1->getWidth() / 2.0f;
	float halfHeight1 = abs(scale1.y) * col1->getHeight() / 2.0f;

	glm::vec3 scale2 = globalT2.getScale();
	float halfWidth2 = abs(scale2.x) * col2->getWidth() / 2.0f;
	float halfHeight2 = abs(scale2.y) * col2->getHeight() / 2.0f;

	float distX = abs(pos2.x - pos1.x);
	float distY = abs(pos2.y - pos1.y);

	float intersectedX = halfWidth1 + halfWidth2 - distX;
	float intersectedY = halfHeight1 + halfHeight2 - distY;

	glm::vec3 dir = (lastPos + col1->getTransform().getPosition()) - pos2;
	dir.x /= halfWidth1 * 2.0f + halfWidth2 * 2.0f;
	dir.y /= halfHeight1 * 2.0f + halfHeight2 * 2.0f;

	glm::vec3 newPos1 = pos1;
	//glm::vec3 newPos2 = pos2;
	glm::vec2 vel1 = phys1->getVelocity();
	//glm::vec2 vel2 = phys2 != nullptr ? phys2->getVelocity() : glm::vec2();

	COLLISION_FLAG collisionFlag = col1->getCollisionFlag();

	if (abs(dir.y) >= abs(dir.x)) {
		if (pos1.y - halfHeight1 >= pos2.y) {
			newPos1.y += intersectedY;
			//newPos2.y -= intersectedY;
			collisionFlag |= COLLISION_DOWN;
		}
		else if (pos1.y + halfHeight1 <= pos2.y) {
			newPos1.y -= intersectedY;
			//newPos2.y += intersectedY;
			collisionFlag |= COLLISION_UP;
		}
		vel1.y = 0.0f;
		//vel2.y = 0.0f;
	}
	else {
		if (pos1.x - halfWidth1 >= pos2.x) {
			newPos1.x += intersectedX;
			//newPos2.x -= intersectedX;
			collisionFlag |= COLLISION_LEFT;
		}
		else if (pos1.x + halfWidth1 <= pos2.x) {
			newPos1.x -= intersectedX;
			//newPos2.x += intersectedX;
			collisionFlag |= COLLISION_RIGHT;
		}
		vel1.x = 0.0f;
		//vel2.x = 0.0f;
	}

	col1->setCollisionFlag(collisionFlag);

	//if (phys2 != nullptr) {
	//	phys2->setVelocity(vel2);
	//	t2.setPosition(newPos2);
	//	phys2->setLastPosition(pos2);
	//} 
	//else {
	//	phys1->setVelocity(vel1);
	//	t1.setPosition(newPos1);
	//	phys1->setLastPosition(pos1);
	//}

	phys1->setVelocity(vel1);
	//t1.setPosition(newPos1);
	t1.setPosition(newPos1 - col1->getTransform().getPosition());
	phys1->setLastPosition(pos1);
}