#include "Button.h"
#include "GameEngine.h"


Button::Button()
    : SimpleObject("Unnamed Button"), label(""), state(ButtonState::NORMAL), isHovered(false), isPressed(false), color(glm::vec3(1.0f, 1.0f, 1.0f)) {
    glm::vec2 size = this->getTransform().getScale();
    this->addColliderComponent();
    collider->setEnableCollision(true);
    color = glm::vec3(1, 1, 1);

    // Initialize text object
    textObject.loadText(label, { 255, 255, 255, 255 }, 24);
}

Button::Button(std::string name, std::string label)
    : SimpleObject(name), label(label), state(ButtonState::NORMAL), isHovered(false), isPressed(false), color(glm::vec3(1.0f, 1.0f, 1.0f)) {
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

void Button::handleMouseInput(int mouseX, int mouseY,bool isMousePressed) {
    glm::vec2 mousePosition(mouseX, mouseY);
    bool inside = collider->isEnable() && checkCollisionPoint(collider, this->getTransform(), mousePosition);

    if (inside) {
        if (!isHovered) {
            isHovered = true;
            state = ButtonState::HOVERED;
            if (onHover) onHover();
            cout << "hover" << endl;
        }

        if (isMousePressed) {
            isPressed = true;
            state = ButtonState::PRESSED;
            cout << "press" << endl;
        }
        else if (isPressed) {
            isPressed = false;
            state = ButtonState::RELEASED;
            if (onClick) onClick();
            cout << "release" << endl;
        }
    }
    else {
        isHovered = false;
        isPressed = false;
        state = ButtonState::NORMAL;
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
    buttonText = new TextObject();
    // Render text (assuming you have a TextObject inside the Button class)
    if (buttonText != nullptr) {
        buttonText->render(globalModelTransform);
    }
    GLuint modelMatrixId = GameEngine::getInstance()->getRenderer()->getModelMatrixAttrId();
    GLuint colorId = GameEngine::getInstance()->getRenderer()->getColorUniformId();
    GLuint renderModeId = GameEngine::getInstance()->getRenderer()->getModeUniformId();

    if (modelMatrixId == -1 || colorId == -1 || renderModeId == -1) {
        std::cout << "Error: Can't set shader attributes" << std::endl;
        return;
    }

    glm::mat4 currentMatrix = this->getTransformMat4();
    currentMatrix = globalModelTransform * currentMatrix;

    if (squareMesh != nullptr) {
        glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));

        // Change color based on state
        switch (state) {
        case ButtonState::NORMAL:
            glUniform3f(colorId, color.r, color.g, color.b);
            break;
        case ButtonState::HOVERED:
            glUniform3f(colorId, color.r * 0.8f, color.g * 0.8f, color.b * 0.8f);
            break;
        case ButtonState::PRESSED:
            glUniform3f(colorId, color.r * 0.5f, color.g * 0.5f, color.b * 0.5f);
            break;
        case ButtonState::RELEASED:
            glUniform3f(colorId, color.r * 1.2f, color.g * 1.2f, color.b * 1.2f);
            break;
        }

        glUniform1i(renderModeId, 0);
        squareMesh->render();
    }

    // Render text
   // glm::mat4 textMatrix = glm::translate(currentMatrix, glm::vec3(0, 0, 0.1f)); // Slight offset to render on top
    //textObject.render(textMatrix);
}
