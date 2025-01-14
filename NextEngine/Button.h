#pragma once

#include "DrawableObject.h"
#include "Collider.h"
#include "InputManager.h"  
#include <functional>
#include <iostream>
#include "CollisionHandler.h" 

/// @brief This class provides a button to be used as a UI
class Button : public DrawableObject {
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
    string label;
    bool isHovered;
    bool isPressed;

    Collider* collider;

    std::function<void()> onClick;
    std::function<void()> onHover;

public:
    Button();
    Button(string name, string label);

    string getLabel() const;
    void setLabel(const string& newLabel);

    ButtonState getState() const;
    void setState(ButtonState newState);

    void setOnClickCallback(std::function<void()> callback);
    void setOnHoverCallback(std::function<void()> callback);

    void handleMouseInput(int mouseX, int mouseY, bool isMousePressed);

    // Override render() to make Button a non-abstract class
    //void render(glm::mat4 globalModelTransform) override;
};

