#pragma once
#include "Collider.h"
#include "DrawableObject.h"
#include "RayObject.h"
#include <iostream>
#include <math.h>
#include <list>

using namespace std;

bool checkCollision(Collider* col1, Transform& t1, Collider* col2, Transform& t2);
bool checkCollisionRay(RayObject* ray1, Collider* col2, Transform& t2);
bool checkCollisionPoint(Collider* col, Transform& t, glm::vec2 point);
void updateCollisionState(Collider* col1, Collider* col2, bool collided);
void handleObjectCollision(list<DrawableObject*>& objects);
void resolveCollision(DrawableObject* obj1, DrawableObject* obj2);

template <class T>
void swapVariable(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

template <class T>
void swapVariable(T* a, T* b) {
	T* temp = a;
	a = b;
	b = temp;
}