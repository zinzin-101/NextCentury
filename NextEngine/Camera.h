#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "DrawableObject.h"

/// @brief This class defines a camera object
class Camera {
private:
    glm::vec3 position; 
    DrawableObject* target;
    glm::vec3 up;
    float deadZoneX;
    float deadZoneY;
    glm::vec3 offset;
    //bool targetIsSet;

public:
    bool shake = false;

    Camera();
    Camera(float fov, float aspect, float near, float far) {}
    void setPosition(const glm::vec3& pos);
    void setTarget(DrawableObject* target);
    void followTarget();
    void setOffset(glm::vec3 offset);
    glm::vec3 getPosition() const;
    DrawableObject* getTarget() const;

    glm::mat4 getViewMatrix();

    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
};
