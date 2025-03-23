#pragma once
#define DEBUG_MODE_ON // Launch in debug mode

// imgui
#ifdef DEBUG_MODE_ON
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#endif