#include "Button.h"
#include "GameEngine.h"

Button::Button()
    : SimpleObject("Unnamed Button"), label(""), state(ButtonState::NORMAL), isHovered(false),
    isPressed(false), isFocused(false), color(glm::vec3(1.0f, 1.0f, 1.0f)) {
    glm::vec2 size = this->getTransform().getScale();
    this->addColliderComponent();
    collider->setEnableCollision(true);

    // Initialize text object
    textObject.loadText(label, { 255, 255, 255, 255 }, 24);
}

Button::Button(std::string name, std::string label)
    : SimpleObject(name), label(label), state(ButtonState::NORMAL), isHovered(false),
    isPressed(false), isFocused(false), color(glm::vec3(1.0f, 1.0f, 1.0f)) {
    glm::vec2 size = this->getTransform().getScale();
    collider = new Collider(this, size.x, size.y);
    collider->setEnableCollision(true);

    textObject.loadText(label, { 255, 255, 255, 255 }, 24);
}

std::string Button::getLabel() const {
    return label;
}

void Button::setLabel(const std::string& newLabel) {
    label = newLabel;
    textObject.loadText(label, { 255, 255, 255, 255 }, 24);
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

void Button::setFocused(bool focused) {
    isFocused = focused;
}

bool Button::getFocused() const {
    return isFocused;
}

void Button::handleKeyboardInput(SDL_Keycode key, bool isKeyPressed) {
    if (!isFocused) return;  

    if (key == SDLK_RETURN || key == SDLK_SPACE) {
        if (isKeyPressed && !isPressed) {
            isPressed = true;
            state = ButtonState::PRESSED;
        }
        else if (!isKeyPressed) {
            isPressed = false;
            state = ButtonState::RELEASED;
            if (onClick) onClick();  
        }
    }
}

void Button::setColor(float r, float g, float b) {
    color = glm::vec3(r, g, b);
}

void Button::setColor(glm::vec3 color) {
    this->color = color;
}

void Button::render(glm::mat4 globalModelTransform) {
    SquareMeshVbo* squareMesh = dynamic_cast<SquareMeshVbo*>(GameEngine::getInstance()->getRenderer()->getMesh(SquareMeshVbo::MESH_NAME));

    GLuint modelMatrixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
    GLuint colorId = GameEngine::getInstance()->getRenderer()->getColorUniformId();
    GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();

    glm::mat4 currentMatrix = this->getTransformMat4();
    currentMatrix = globalModelTransform * currentMatrix;

    if (squareMesh != nullptr) {
        glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));

        // Change color based on state
        glm::vec3 renderColor = color;
        if (isFocused) {
            renderColor = glm::vec3(1.0f, 1.0f, 0.0f); 
        }

        switch (state) {
        case ButtonState::NORMAL:
            glUniform3f(colorId, renderColor.r, renderColor.g, renderColor.b);
            break;
        case ButtonState::HOVERED:
            glUniform3f(colorId, renderColor.r * 0.8f, renderColor.g * 0.8f, renderColor.b * 0.8f);
            break;
        case ButtonState::PRESSED:
            glUniform3f(colorId, renderColor.r * 0.5f, renderColor.g * 0.5f, renderColor.b * 0.5f);
            break;
        case ButtonState::RELEASED:
            glUniform3f(colorId, renderColor.r * 1.2f, renderColor.g * 1.2f, renderColor.b * 1.2f);
            break;
        }

        glUniform1i(renderModeId, 0);
        squareMesh->render();
    }

    // Render text
    //textObject.render(globalModelTransform);
}
