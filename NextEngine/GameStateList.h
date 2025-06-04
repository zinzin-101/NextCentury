#pragma once

enum GameState {
	// list of all game states 
	GS_LEVEL1 = 0,
	GS_LEVEL2,
	GS_LEVEL3,
	GS_MAINMENU,
	GS_ACT1,
	GS_ACT2,
	GS_ACT3,
	GS_ACT4,
	GS_ACT5,
	GS_ACT6,
	GS_ACT7,
	GS_ACT8,
	GS_CREDIT,
	

	// special game state. Do not change
	GS_RESTART,
	GS_QUIT,
	GS_NONE
};