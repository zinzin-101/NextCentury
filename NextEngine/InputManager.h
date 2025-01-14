#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>
#include <unordered_map>
#include <iostream>

class InputManager {
public:
    enum KeyState {
        KEY_IDLE,    // Not pressed
        KEY_PRESSED, // Just pressed this frame
        KEY_HELD,    // Held down across multiple frames
        KEY_RELEASED // Just released this frame
    };

    InputManager();
    ~InputManager();

    void updateInput();  // Poll inputs and update the state

    // Unity-like input functions
    bool getButtonDown(SDL_Keycode key);      // Equivalent to Input.GetButtonDown
    bool getButton(SDL_Keycode key);          // Equivalent to Input.GetButton
    bool getButtonUp(SDL_Keycode key);        // Equivalent to Input.GetButtonUp

    bool getMouseButtonDown(int button);      // Equivalent to Input.GetMouseButtonDown
    bool getMouseButton(int button);          // Equivalent to Input.GetMouseButton
    bool getMouseButtonUp(int button);        // Equivalent to Input.GetMouseButtonUp

    bool getControllerButtonDown(Uint8 button); // Equivalent to Input.GetControllerButtonDown
    bool getControllerButton(Uint8 button);     // Equivalent to Input.GetControllerButton
    bool getControllerButtonUp(Uint8 button);   // Equivalent to Input.GetControllerButtonUp

    float getControllerAnalogStickX();
    float getControllerAnalogStickY();

    int getMouseX() const;
    int getMouseY() const;


private:
    std::unordered_map<SDL_Keycode, KeyState> keyStates;   // Map of key states
    std::unordered_map<int, KeyState> mouseButtonStates;   // Mouse button states
    std::unordered_map<Uint8, KeyState> buttonStates;      // Controller button states
    int mouseX, mouseY;                                    // Mouse position
    SDL_GameController* controller;                        // Controller pointer
    std::pair<float, float> analogStick;                   // Analog Stick
};

#endif
