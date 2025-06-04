#pragma once
#include "TexturedObject.h"
#include "Collider.h"
#include "TextObject.h"
#include "InputManager.h"
#include "Animation.h"
#include <functional>

class Button : public TexturedObject {
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
    TextObject textObject;
    Collider* collider;

    std::function<void()> onClick;
    std::function<void()> onHover;

public:
    Button();
    Button(const std::string& name, const std::string& texturePath);

    const std::string& getLabel() const;
    void setLabel(const std::string& newLabel);

    ButtonState getState() const;
    void setState(ButtonState newState);

    void setOnClickCallback(std::function<void()> callback);
    void setOnHoverCallback(std::function<void()> callback);

    void setFocused(bool focused);
    bool getFocused() const;
    void handleKeyboardInput(int key, bool isKeyPressed);

    void render(glm::mat4 globalModelTransform) override;
    std::function<void()> getOnClickCallback() const {
        return onClick;
    }
};
