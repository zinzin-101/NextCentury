#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

/// @brief This class defines a camera object
class Camera {
private:
    glm::vec3 position; 
    glm::vec3 target; 
    glm::vec3 up;   

public:
    Camera();
    Camera(float fov, float aspect, float near, float far) {}
    void setPosition(const glm::vec3& pos);
    void setTarget(const glm::vec3& targetPos);
    glm::vec3 getPosition() const;
    glm::vec3 getTarget() const;  

    glm::mat4 getViewMatrix();

    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
};
