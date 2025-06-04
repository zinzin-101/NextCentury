#pragma once

#include <vector>
#include <functional>
#include <list>
#include <SDL_mixer.h>
#include "Button.h"
#include "DrawableObject.h"
#include "PlayerObject.h"
#include "Audio.h"
#include "GameEngine.h"
#include "SimpleObject.h"
#include "Animation.h"

class UISetting {
public:
    UISetting();
    ~UISetting();

    void initUI(std::list<DrawableObject*>& objectsList);
    void updateUI();
    void handleInput(SDL_Keycode key);
    void hideAllSettings();
	void showAllSettings();

    bool isClosed() const { return justClosed; }
    void resetClosedFlag() { justClosed = false; }

private:
    enum class Page {
        AUDIO,
        CONTROL
    } currentPage = Page::AUDIO;

    std::vector<Button*> buttons;
    int selectedButtonIndex = 0;

    SimpleObject* blackdrop = nullptr;
    TexturedObject* AudioPageTex = nullptr;
    TexturedObject* ControlPageTex = nullptr;

    TexturedObject* keyboardTex = nullptr;
    TexturedObject* mouseTex = nullptr;

    TexturedObject* masterVolumeBar = nullptr;
    TexturedObject* soundEffectBar = nullptr;
    TexturedObject* musicBar = nullptr;

    SimpleObject* masterFill = nullptr;
    SimpleObject* sfxFill = nullptr;
    SimpleObject* musicFill = nullptr;

    TexturedObject* arrowMasterLeft = nullptr;
    TexturedObject* arrowMasterRight = nullptr;
    TexturedObject* arrowSFXLeft = nullptr;
    TexturedObject* arrowSFXRight = nullptr;
    TexturedObject* arrowMusicLeft = nullptr;
    TexturedObject* arrowMusicRight = nullptr;

    TexturedObject* audioText = nullptr;
    TexturedObject* controlText = nullptr;

    Animation* audioTextAnim = nullptr;
    Animation* controlTextAnim = nullptr;

	TexturedObject* f8 = nullptr;
	TexturedObject* f11 = nullptr;

    void updateArrowHighlight();
    void adjustVolume(int buttonIndex);
    std::list<DrawableObject*>* sharedObjectsList = nullptr;

    static constexpr float BAR_WIDTH = 5.0f;
    static constexpr float BAR_HEIGHT = 0.1f;
    static constexpr float BAR_XPOS = 2.0f;
    static constexpr float ARROW_SIZE = 0.15f;
    static constexpr float ARROW_MARGIN = 0.05f;

    static constexpr float MASTER_Y = 1.25f;
    static constexpr float SFX_Y = -1.75f;
    static constexpr float MUSIC_Y = -0.25f;

    float computeFillFraction(int currentVol) {
        return float(currentVol) / float(MIX_MAX_VOLUME);
    }
    bool justClosed = false;
};
