#define _CRT_SECURE_NO_WARNINGS
// OpenGL headers
#include <GL/glew.h>

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include "GameEngine.h"
#include "InputManager.h"
#include "TextObject.h"


#ifdef DEBUG_NO_EDITOR
GLint WIDTH = 1600, HEIGHT = 900;
Uint32 windowScreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
#else
	#ifndef DEBUG_MODE_ON
	GLint WIDTH = 1920, HEIGHT = 1080;
	Uint32 windowScreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	#else
		#ifdef START_FULLSCREEN
		GLint WIDTH = 1920, HEIGHT = 1080;
		Uint32 windowScreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE;
		#else		
		GLint WIDTH = 1600, HEIGHT = 900;
		Uint32 windowScreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
		#endif
	#endif
#endif

/// Forcing the program to use dedicated GPU
extern "C" {
	//high performance
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; //nvidia
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; //amd

	//integrated
	//__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000000; //nvidia
	//__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0; //amd
}

using namespace std;
bool quit;
const unsigned int FPS = 60;
const unsigned int MIN_FPS = 24;
const int frameDelay = 1000 / FPS;
const int maxFrameDelay = 1000 / MIN_FPS;
uint64_t frameCount = 0;
uint64_t startTick;
uint64_t deltaTick;
//float test = 0.0f;

const glm::vec2 RESOLUTION_PRESETS[4] = {
	glm::vec2(1920, 1080),
	glm::vec2(1600, 900),
	glm::vec2(1280, 720),
	glm::vec2(960, 540)
};
unsigned int resolutionIndex = 0;

SDL_Window* window;
SDL_GLContext glContext;
SDL_Event sdlEvent;

InputManager inputManager;
GameEngine * engine = nullptr;

static const std::filesystem::path SAVE_PATH = std::filesystem::path(std::getenv("LOCALAPPDATA")) / "NextCentury" / "data";

int main(int argc, char *argv[]) {
	quit = false;

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Initialize video subsystem
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		// Display error message
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}

	// Create window
	window = SDL_CreateWindow("NextEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, windowScreenFlag);
	if (window == NULL) {
		// Display error message
		cout << "Window could not be created! SDL_Error" << SDL_GetError() << endl;
		return false;
	}
	
	// Create OpenGL context
	glContext = SDL_GL_CreateContext(window);

	if (glContext == NULL) {
		// Display error message
		cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << endl;
		return false;
	}
	
	// Initialize glew
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		cout << "Error initializing GLEW! " << glewGetErrorString(glewError) << endl;
		return false;
	}
	if (SDL_GL_SetSwapInterval(1) < 0) {
		cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << endl;
	}

	std::filesystem::create_directories(SAVE_PATH);
	std::filesystem::path resolutionPath = SAVE_PATH / "ResolutionSave.dat";

	std::ifstream initResolution(resolutionPath);
	if (initResolution) {
		std::string line;
		std::getline(initResolution, line);
		resolutionIndex = std::stoi(line);

		std::getline(initResolution, line);
		bool isFullScreen = static_cast<bool>(std::stoi(line));

		if (isFullScreen) {
			windowScreenFlag |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			SDL_SetWindowFullscreen(window, windowScreenFlag);
		}

		glm::vec2 currentResolution = RESOLUTION_PRESETS[resolutionIndex];
		WIDTH = currentResolution.x;
		HEIGHT = currentResolution.y;
		SDL_SetWindowSize(window, WIDTH, HEIGHT);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	
		initResolution.close();
	}
	else {
		std::cout << "Failed to read resolution save or is first time launching" << std::endl;
	}

	engine = GameEngine::getInstance();
	engine->init(WIDTH, HEIGHT);
	engine->setSDLWindow(window);
	engine->setGlContext(&glContext);

	//Init SDL for text
	TextObject::InitSDL();

	#ifdef DEBUG_MODE_ON
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

	//// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	const char* glsl_version = "#version 330";
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init(glsl_version);

	#endif

	// Game loop
	while (engine->getStateController()->gameStateCurr != GameState::GS_QUIT) {

		if (engine->getStateController()->gameStateCurr != GS_RESTART) {
			engine->getStateController()->loadLevel();
			SDL_GL_SwapWindow(window);
		}
		else {
			engine->getStateController()->gameStateNext =
			engine->getStateController()->gameStateCurr = engine->getStateController()->gameStatePrev;
		}

		// Initialize the gamestate
		engine->getStateController()->initLevel();

		while (engine->getStateController()->gameStateCurr == engine->getStateController()->gameStateNext) {

			// toggle fullscreen
			if (inputManager.getButtonDown(SDLK_F11)) {
				windowScreenFlag ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
				SDL_SetWindowFullscreen(window, windowScreenFlag);

				//if (!(windowScreenFlag & SDL_WINDOW_FULLSCREEN_DESKTOP)) {
				//	SDL_DisplayMode dm;
				//	SDL_GetCurrentDisplayMode(0, &dm);
				//	int width = (int)((float)dm.w * 0.9f);
				//	int height = (int)((float)dm.h * 0.9f);
				//	SDL_SetWindowSize(window, width, height);
				//	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				//}
			}

			if (inputManager.getButtonDown(SDLK_F8) && (!(windowScreenFlag & SDL_WINDOW_FULLSCREEN_DESKTOP))) {
				resolutionIndex = ++resolutionIndex % 4;
				glm::vec2 currentResolution = RESOLUTION_PRESETS[resolutionIndex];
				WIDTH = currentResolution.x;
				HEIGHT = currentResolution.y;
				SDL_SetWindowSize(window, WIDTH, HEIGHT);
				SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			}

			SDL_GetWindowSize(window, &WIDTH, &HEIGHT);
			engine->getRenderer()->setGLViewport(WIDTH, HEIGHT);

			startTick = SDL_GetTicks();

			inputManager.updateInput(); // Update input states

			//quit
			/*if (inputManager.getButtonDown(SDLK_ESCAPE)) {
				engine->getStateController()->gameStateNext = GameState::GS_QUIT;
			}*/

			/// Handle input in each level
			engine->getStateController()->handleKeyLevel(inputManager);

			// Handle mouse button clicks
			if (inputManager.getMouseButtonDown(SDL_BUTTON_LEFT)) {
				int mouseX = inputManager.getMouseX();
				int mouseY = inputManager.getMouseY();
				engine->getStateController()->handleMouseLevel(0, mouseX, mouseY);  // Handle mouse click at (x, y)
			}

			if (inputManager.getControllerButtonDown(SDL_CONTROLLER_BUTTON_START)) {
				engine->getStateController()->gameStateNext = GameState::GS_QUIT;
			}

			if ((inputManager.getControllerAnalogStickX() > 0.05) || (inputManager.getControllerAnalogStickX() < -0.05)) {
				engine->getStateController()->handleAnalogLevel(0, inputManager.getControllerAnalogStickX());
			}


			if ((inputManager.getControllerAnalogStickY() > 0.05) || (inputManager.getControllerAnalogStickY() < -0.05)) {
				engine->getStateController()->handleAnalogLevel(1, inputManager.getControllerAnalogStickY());
			}

			deltaTick = SDL_GetTicks() - startTick;
			if (deltaTick < maxFrameDelay) {
				engine->getTime()->updateTick(SDL_GetTicks());
			}
			else {
				engine->getTime()->updateTickCounterOnly(SDL_GetTicks());
			}

			engine->getStateController()->updateLevel();

			engine->getStateController()->drawLevel();
			SDL_GL_SwapWindow(window);

			/// Limiting the FPS to 60
			if (deltaTick < frameDelay) {
				SDL_Delay(frameDelay - deltaTick);
			}

			//cout << "Next " << GameData::GetInstance()->gGameStateNext << endl;
			//cout << "FPS: " << 1.0f / engine->getTime()->getDeltaTimeRealTime() << endl;

			engine->updateEngineComponent();
		}

		engine->getStateController()->freelevel();

		if (engine->getStateController()->gameStateNext != GS_RESTART) {
			engine->getStateController()->unloadLevel();
		}

		engine->getStateController()->gameStatePrev = engine->getStateController()->gameStateCurr;
		engine->getStateController()->gameStateCurr = engine->getStateController()->gameStateNext;	
	}

	std::ofstream resolutionSave(resolutionPath);

	if (resolutionSave) {
		resolutionSave << resolutionIndex << "\n";
		resolutionSave << static_cast<bool>((windowScreenFlag & SDL_WINDOW_FULLSCREEN_DESKTOP));
		resolutionSave.close();
	}
	else {
		std::cout << "Failed to save resolution" << std::endl;
	}

	//clean text SDL
	TextObject::cleanSDL();

	#ifdef DEBUG_MODE_ON
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	#endif

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}


