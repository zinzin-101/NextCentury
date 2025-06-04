#include "Button.h"
#include "GameEngine.h"
#include "SquareMeshVbo.h"

Button::Button()
    : TexturedObject("UnnamedButton")
    , state(ButtonState::NORMAL)
    , isHovered(false)
    , isPressed(false)
    , isFocused(false)
{
    // no texture by default
    auto scale = getTransform().getScale();
    collider = new Collider(this, scale.x, scale.y);
    collider->setEnableCollision(true);
    textObject.loadText("", { 255,255,255,255 }, 50);
}

Button::Button(const std::string& name, const std::string& texturePath)
    : TexturedObject(name)
    , state(ButtonState::NORMAL)
    , isHovered(false)
    , isPressed(false)
    , isFocused(false)
{
    setTexture(texturePath);

    initAnimation(1, 3);
    Animation* anim = getAnimationComponent();
    anim->addState("idle", 0, 0, 1, false);
    anim->addState("hovered", 0, 1, 1, false);
    anim->addState("pressed", 0, 2, 1, false);
    anim->setState("idle");

    auto scale = getTransform().getScale();
    collider = new Collider(this, scale.x, scale.y);
    collider->setEnableCollision(true);

    textObject.loadText("", { 255,255,255,255 }, 50);
}

const std::string& Button::getLabel() const {
    return label;
}

void Button::setLabel(const std::string& newLabel) {
    label = newLabel;
    textObject.loadText(label, { 255,255,255,255 }, 50);
}

Button::ButtonState Button::getState() const {
    return state;
}

void Button::setState(ButtonState newState) {
    state = newState;
    Animation* anim = getAnimationComponent();
    switch (state) {
    case ButtonState::NORMAL:  anim->setState("idle");    break;
    case ButtonState::HOVERED: anim->setState("hovered"); break;
    default:                   anim->setState("pressed"); break;
    }
}
void Button::setOnClickCallback(std::function<void()> callback) {
    onClick = std::move(callback);
}

void Button::setOnHoverCallback(std::function<void()> callback) {
    onHover = std::move(callback);
}

void Button::setFocused(bool focused) {
    isFocused = focused;
    setState(focused ? ButtonState::HOVERED : ButtonState::NORMAL);
}

bool Button::getFocused() const {
    return isFocused;
}

void Button::handleKeyboardInput(int key, bool isKeyPressed) {
    if (!isFocused) return;
    if (key == SDLK_RETURN || key == SDLK_SPACE) {
        if (isKeyPressed && !isPressed) {
            isPressed = true;
            setState(ButtonState::PRESSED);
            GameEngine::getInstance()->getAudioEngine().playSoundEffectByName("click.wav");
        }
        else if (!isKeyPressed) {
            isPressed = false;
            setState(ButtonState::RELEASED);
            if (onClick) onClick();
        }
    }
}

void Button::render(glm::mat4 globalModelTransform) {
    Animation* anim = getAnimationComponent();
    anim->updateCurrentState();
    TexturedObject::render(globalModelTransform);
    if (!label.empty()) textObject.render(globalModelTransform);
}
