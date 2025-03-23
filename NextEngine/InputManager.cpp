#include "InputManager.h"
#include "DebugMode.h"

InputManager::InputManager() : mouseX(0), mouseY(0), controller(nullptr) {
    // Initialize SDL controller subsystem
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
    if (SDL_NumJoysticks() > 0) {
        controller = SDL_GameControllerOpen(0); // Open the first controller
    }
}

InputManager::~InputManager() {
    if (controller) {
        SDL_GameControllerClose(controller);
    }
}

void InputManager::updateInput() {
    // Reset key states
    for (auto& pair : keyStates) {
        auto& key = pair.first;
        auto& state = pair.second;
        if (state == KEY_PRESSED) {
            keyStates[key] = KEY_HELD;
        }
        else if (state == KEY_RELEASED) {
            keyStates[key] = KEY_IDLE;
        }
    }

    // Reset mouse button states
    for (auto& pair : mouseButtonStates) {
        auto& button = pair.first;
        auto& state = pair.second;
        if (state == KEY_PRESSED) {
            mouseButtonStates[button] = KEY_HELD;
        }
        else if (state == KEY_RELEASED) {
            mouseButtonStates[button] = KEY_IDLE;
        }
    }

    // Reset controller button states
    for (auto& pair : buttonStates) {
        auto& button = pair.first;
        auto& state = pair.second;
        if (state == KEY_PRESSED) {
            buttonStates[button] = KEY_HELD;
        }
        else if (state == KEY_RELEASED) {
            buttonStates[button] = KEY_IDLE;
        }
    }

    // Poll events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        #ifdef DEBUG_MODE_ON
        // Handle Imgui event
        ImGui_ImplSDL2_ProcessEvent(&event);
        #endif

        // Handle keyboard events
        if (event.type == SDL_KEYDOWN && !event.key.repeat) {
            keyStates[event.key.keysym.sym] = KEY_PRESSED;
        }
        if (event.type == SDL_KEYUP) {
            keyStates[event.key.keysym.sym] = KEY_RELEASED;
        }

        // Handle mouse events
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            mouseButtonStates[event.button.button] = KEY_PRESSED;
        }
        if (event.type == SDL_MOUSEBUTTONUP) {
            mouseButtonStates[event.button.button] = KEY_RELEASED;
        }

        // Handle controller events
        if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            buttonStates[event.cbutton.button] = KEY_PRESSED;
        }
        if (event.type == SDL_CONTROLLERBUTTONUP) {
            buttonStates[event.cbutton.button] = KEY_RELEASED;
        }

        if (event.type == SDL_CONTROLLERAXISMOTION) {
            if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
                analogStick.first = event.caxis.value/32768.0f; // Capture left stick x-axis
            }
            if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                analogStick.second = event.caxis.value / 32768.0f; // Capture left stick y-axis
            }
            
            
        }
    }

    // Update mouse position
    SDL_GetMouseState(&mouseX, &mouseY);

    //std::cout << "Left Stick X: " << analogStick.first << " | Left Stick Y: " << analogStick.second << std::endl;
}

// Key handling methods
bool InputManager::getButtonDown(SDL_Keycode key) {
    return keyStates[key] == KEY_PRESSED;
}

bool InputManager::getButton(SDL_Keycode key) {
    return keyStates[key] == KEY_HELD || keyStates[key] == KEY_PRESSED;
}

bool InputManager::getButtonUp(SDL_Keycode key) {
    return keyStates[key] == KEY_RELEASED;
}

// Mouse handling methods
bool InputManager::getMouseButtonDown(int button) {
    return mouseButtonStates[button] == KEY_PRESSED;
}

bool InputManager::getMouseButton(int button) {
    return mouseButtonStates[button] == KEY_HELD || mouseButtonStates[button] == KEY_PRESSED;
}

bool InputManager::getMouseButtonUp(int button) {
    return mouseButtonStates[button] == KEY_RELEASED;
}

// Controller handling methods
bool InputManager::getControllerButtonDown(Uint8 button) {
    return buttonStates[button] == KEY_PRESSED;
}

bool InputManager::getControllerButton(Uint8 button) {
    return buttonStates[button] == KEY_HELD || buttonStates[button] == KEY_PRESSED;
}

bool InputManager::getControllerButtonUp(Uint8 button) {
    return buttonStates[button] == KEY_RELEASED;
}

float InputManager::getControllerAnalogStickX() {
    return analogStick.first;
}

float InputManager::getControllerAnalogStickY() {
    return analogStick.second;
}


int InputManager::getMouseX() const {
    return mouseX;
}

int InputManager::getMouseY() const {
    return mouseY;
}
