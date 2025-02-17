#pragma once

#include "SquareMeshVbo.h"
#include "DrawableObject.h"
#include "Collider.h"
#include "SimpleObject.h"
#include "TextObject.h"
#include "InputManager.h"
#include "TextObject.h"
#include <functional>
#include <iostream>
#include "CollisionHandler.h"

/// @brief This class provides a button to be used as a UI
class Button : public SimpleObject,public TextObject {
public:
    /// @brief This defines the possible states that a Button can have
    enum class ButtonState {
        NORMAL,
        HOVERED,
        PRESSED,
        RELEASED
    };

private:
    ButtonState state;
    std::string label;
    bool isHovered;
    bool isPressed;
    glm::vec3 color;
    TextObject textObject;  
    TextObject* buttonText;  

    std::function<void()> onClick;
    std::function<void()> onHover;

public:
    Button();
    Button(std::string name, std::string label);

    std::string getLabel() const;
    void setLabel(const std::string& newLabel);

    ButtonState getState() const;
    void setState(ButtonState newState);

    void setOnClickCallback(std::function<void()> callback);
    void setOnHoverCallback(std::function<void()> callback);

    void handleMouseInput(int mouseX, int mouseY, bool isMousePressed);

    void setColor(float r, float g, float b);
    void setColor(glm::vec3 color);

    // Override render() to make Button a non-abstract class
    void render(glm::mat4 globalModelTransform) override;
};
