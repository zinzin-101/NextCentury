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
    bool isUsingDeadLimit;
    float deadLimitPosMin;
    float deadLimitPosMax;
    glm::vec3 offset;
    //bool targetIsSet;

    float shakeDurationRemaining;
    bool isShaking;

public:
    Camera();
    Camera(float fov, float aspect, float near, float far) {}
    void setDeadLimitBool(bool isDeadLimit);
    void setDeadLimitMinMax(float min, float max);
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

    void startShake(float duration);

    void updateCamera();
    
};
