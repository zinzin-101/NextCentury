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

class Button : public SimpleObject {
public:
   
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
    bool isFocused;
    glm::vec3 color;
    TextObject textObject;

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

    void setFocused(bool focused);
    bool getFocused() const;
    void handleKeyboardInput(int key, bool isKeyPressed);

    void setColor(float r, float g, float b);
    void setColor(glm::vec3 color);

    void render(glm::mat4 globalModelTransform) override;
};
