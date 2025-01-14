#include "Button.h"

extern InputManager* inputManager;

Button::Button()
    : DrawableObject("Unnamed Button"), label(""), state(ButtonState::NORMAL), isHovered(false), isPressed(false) {
    glm::vec2 size = this->getTransform().getScale();
    collider = new Collider(this, size.x, size.y);
    collider->setEnableCollision(true);  // Enable collider
}

Button::Button(string name, string label)
    : DrawableObject(name), label(label), state(ButtonState::NORMAL), isHovered(false), isPressed(false) {
    glm::vec2 size = this->getTransform().getScale();
    collider = new Collider(this, size.x, size.y);
    collider->setEnableCollision(true);  // Enable collider
}

string Button::getLabel() const {
    return label;
}

void Button::setLabel(const string& newLabel) {
    label = newLabel;
}

Button::ButtonState Button::getState() const {
    return state;
}

void Button::setState(ButtonState newState) {
    state = newState;
}

void Button::setOnClickCallback(std::function<void()> callback) {
    onClick = callback;
}

void Button::setOnHoverCallback(std::function<void()> callback) {
    onHover = callback;
}

void Button::handleMouseInput(int mouseX, int mouseY, bool isMousePressed) {
    // Convert mouse position to world coordinates for collision detection
    glm::vec2 mousePosition(mouseX, mouseY);

    if (collider->isEnable() && checkCollisionPoint(collider, this->getTransform(), mousePosition)) {
        if (!isHovered) {
            isHovered = true;
            state = ButtonState::HOVERED;

            // Trigger hover callback if set
            if (onHover) {
                onHover();
            }
        }

        if (isMousePressed) {
            isPressed = true;
            state = ButtonState::PRESSED;
        }
        else if (isPressed) {
            // Release button and trigger click if applicable
            isPressed = false;
            state = ButtonState::RELEASED;

            if (onClick) {
                onClick();
            }
        }
    }
    else {
        // Reset button state when not interacted with
        isHovered = false;
        isPressed = false;
        state = ButtonState::NORMAL;
    }
}
