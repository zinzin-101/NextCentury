#include "UISetting.h"
#include "SDL.h"
#include <iostream>

UISetting::UISetting() {
    buttons.resize(6, nullptr);
}

UISetting::~UISetting() {
}

void UISetting::initUI(std::list<DrawableObject*>& objectsList) {
    sharedObjectsList = &objectsList;

    blackdrop = new SimpleObject("SettingBlackdrop");
    blackdrop->setColor({ 0.0f, 0.0f, 0.0f });
    blackdrop->getTransform().setScale({ 20.0f, 20.0f, 0.0f });
    objectsList.push_back(blackdrop);

    AudioPageTex = new TexturedObject("AudioPageBG");
    AudioPageTex->setTexture("../Resource/Texture/UI/Setting/AudioPage.png");
    AudioPageTex->getTransform().setScale({ 16.0f, 9.0f, 0.0f });
    AudioPageTex->getTransform().setPosition({ 0.0f, 0.0f, 0.0f });
    objectsList.push_back(AudioPageTex);

    audioText = new TexturedObject("AudioText");
    audioText->setTexture("../Resource/Texture/UI/Setting/Audio.png");
    audioText->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    audioText->getTransform().setPosition({ -3.0f, 3.1f, 0.0f });
    objectsList.push_back(audioText);

    audioText->initAnimation(1, 3);
    audioTextAnim = audioText->getAnimationComponent();
    audioTextAnim->setPaused(true);
    audioTextAnim->setFrame(0, 0);


    controlText = new TexturedObject("ControlText");
    controlText->setTexture("../Resource/Texture/UI/Setting/Control.png");
    controlText->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    controlText->getTransform().setPosition({ 3.0f, 3.1f, 0.0f });
    objectsList.push_back(controlText);

    controlText->initAnimation(1, 3);
    controlTextAnim = controlText->getAnimationComponent();
    controlTextAnim->setPaused(true);
    controlTextAnim->setFrame(0, 0);


    keyboardTex = new TexturedObject("KeyboardSprite");
    keyboardTex->setTexture("../Resource/Texture/UI/Setting/Keyboard.png");
    keyboardTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });  
    keyboardTex->getTransform().setPosition({ -1.0f, 0.0f, 0.0f });
    objectsList.push_back(keyboardTex);

    mouseTex = new TexturedObject("MouseSprite");
    mouseTex->setTexture("../Resource/Texture/UI/Setting/Mouse.png");
    mouseTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });     
    mouseTex->getTransform().setPosition({ -1.0f, 0.0f, 0.0f });
    objectsList.push_back(mouseTex);

    ControlPageTex = new TexturedObject("ControlPageBG");
    ControlPageTex->setTexture("../Resource/Texture/UI/Setting/ControlPage.png");
    ControlPageTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f }); 
    ControlPageTex->getTransform().setPosition({ 0.0f, 0.0f, 0.0f });
    objectsList.push_back(ControlPageTex);

    mouseTex = new TexturedObject("KeyboardSprite");
    mouseTex->setTexture("../Resource/Texture/UI/Setting/Mouse.png");
    mouseTex->getTransform().setScale({ 1.0f * 7.0f, (78.0f / 119.0f) * 7.0f, 0.0f });
    mouseTex->getTransform().setPosition({ -1.0f, 0.0f, 0.0f });
    mouseTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    objectsList.push_back(mouseTex);

    ControlPageTex = new TexturedObject("ControlPageBG");
    ControlPageTex->setTexture("../Resource/Texture/UI/Setting/ControlPage.png");
    ControlPageTex->getTransform().setScale({16.0f, 9.0f, 0.0f});
    ControlPageTex->getTransform().setPosition({0.0f, 0.0f, 0.0f});
    ControlPageTex->getTransform().setScale({0.0f, 0.0f, 0.0f});
    objectsList.push_back(ControlPageTex);

    const float barW = BAR_WIDTH;
    const float barH = BAR_HEIGHT;
    const float barX = BAR_XPOS;

    masterVolumeBar = new TexturedObject("MasterVolumeBar");
    masterVolumeBar->setTexture("../Resource/Texture/UI/Setting/AudioBar.png");
    masterVolumeBar->getTransform().setScale({barW, barH, 0.0f});
    masterVolumeBar->getTransform().setPosition({barX, MASTER_Y, 0.0f});
    objectsList.push_back(masterVolumeBar);

    masterFill = new SimpleObject("MasterFill");
    masterFill->setColor({ 1.0f, 1.0f, 1.0f });
    masterFill->getTransform().setScale({barW, barH, 0.0f});
    masterFill->getTransform().setPosition({barX, MASTER_Y, 0.0f});
    objectsList.push_back(masterFill);

    soundEffectBar = new TexturedObject("SFXVolumeBar");
    soundEffectBar->setTexture("../Resource/Texture/UI/Setting/AudioBar.png");
    soundEffectBar->getTransform().setScale({barW, barH, 0.0f});
    soundEffectBar->getTransform().setPosition({barX, SFX_Y, 0.0f});
    objectsList.push_back(soundEffectBar);

    sfxFill = new SimpleObject("SFXFill");
    sfxFill->setColor({ 1.0f, 1.0f, 1.0f });
    sfxFill->getTransform().setScale({barW, barH, 0.0f});
    sfxFill->getTransform().setPosition({barX, SFX_Y, 0.0f});
    objectsList.push_back(sfxFill);

    musicBar = new TexturedObject("MusicVolumeBar");
    musicBar->setTexture("../Resource/Texture/UI/Setting/AudioBar.png");
    musicBar->getTransform().setScale({barW, barH, 0.0f});
    musicBar->getTransform().setPosition({barX, MUSIC_Y, 0.0f});
    objectsList.push_back(musicBar);

    musicFill = new SimpleObject("MusicFill");
    musicFill->setColor({ 1.0f, 1.0f, 1.0f });
    musicFill->getTransform().setScale({barW, barH, 0.0f});
    musicFill->getTransform().setPosition({barX, MUSIC_Y, 0.0f});
    objectsList.push_back(musicFill);

    const float arrowW = ARROW_SIZE;
    const float halfBar = barW / 2.0f;   
    const float halfArr = arrowW / 2.0f; 
    const float margin = ARROW_MARGIN;   

    float leftX  = barX - halfBar - halfArr - margin; 
    float rightX = barX + halfBar + halfArr + margin; 
    static constexpr int DELTA = 10;

    arrowMasterLeft = new TexturedObject("MasterLeft");
    arrowMasterLeft->setTexture("../Resource/Texture/UI/Setting/ArrowLeft.png");
    arrowMasterLeft->getTransform().setScale({arrowW, arrowW, 0.0f});
    arrowMasterLeft->getTransform().setPosition({leftX, MASTER_Y, 0.0f});
    objectsList.push_back(arrowMasterLeft);

    buttons[0] = new Button("MasterLeftButton", "");
    buttons[0]->getTransform().setScale({arrowW, arrowW, 0.0f});
    buttons[0]->getTransform().setPosition({leftX, MASTER_Y, 0.0f});
    buttons[0]->setOnClickCallback([]() {
        auto& audio = GameEngine::getInstance()->getAudioEngine();
        audio.adjustSoundEffectVolume(-DELTA);
        audio.adjustMusicVolume(-DELTA);
        audio.printVolume();
    });
    objectsList.push_back(buttons[0]);

    arrowMasterRight = new TexturedObject("MasterRight");
    arrowMasterRight->setTexture("../Resource/Texture/UI/Setting/ArrowRight.png");
    arrowMasterRight->getTransform().setScale({arrowW, arrowW, 0.0f});
    arrowMasterRight->getTransform().setPosition({rightX, MASTER_Y, 0.0f});
    objectsList.push_back(arrowMasterRight);

    buttons[1] = new Button("MasterRightButton", "");
    buttons[1]->getTransform().setScale({arrowW, arrowW, 0.0f});
    buttons[1]->getTransform().setPosition({rightX, MASTER_Y, 0.0f});
    buttons[1]->setOnClickCallback([]() {
        auto& audio = GameEngine::getInstance()->getAudioEngine();
        audio.adjustSoundEffectVolume(+DELTA);
        audio.adjustMusicVolume(+DELTA);
        audio.printVolume();
    });
    objectsList.push_back(buttons[1]);

    arrowSFXLeft = new TexturedObject("SFXLeft");
    arrowSFXLeft->setTexture("../Resource/Texture/UI/Setting/ArrowLeft.png");
    arrowSFXLeft->getTransform().setScale({arrowW, arrowW, 0.0f});
    arrowSFXLeft->getTransform().setPosition({leftX, SFX_Y, 0.0f});
    objectsList.push_back(arrowSFXLeft);

    buttons[4] = new Button("SFXLeftButton", "");
    buttons[4]->getTransform().setScale({arrowW, arrowW, 0.0f});
    buttons[4]->getTransform().setPosition({leftX, SFX_Y, 0.0f});
    buttons[4]->setOnClickCallback([]() {
        auto& audio = GameEngine::getInstance()->getAudioEngine();
        audio.adjustSoundEffectVolume(-DELTA);
        audio.printVolume();
    });
    objectsList.push_back(buttons[4]);

    arrowSFXRight = new TexturedObject("SFXRight");
    arrowSFXRight->setTexture("../Resource/Texture/UI/Setting/ArrowRight.png");
    arrowSFXRight->getTransform().setScale({arrowW, arrowW, 0.0f});
    arrowSFXRight->getTransform().setPosition({rightX, SFX_Y, 0.0f});
    objectsList.push_back(arrowSFXRight);

    buttons[5] = new Button("SFXRightButton", "");
    buttons[5]->getTransform().setScale({arrowW, arrowW, 0.0f});
    buttons[5]->getTransform().setPosition({rightX, SFX_Y, 0.0f});
    buttons[5]->setOnClickCallback([]() {
        auto& audio = GameEngine::getInstance()->getAudioEngine();
        audio.adjustSoundEffectVolume(+DELTA);
        audio.printVolume();
    });
    objectsList.push_back(buttons[5]);

    arrowMusicLeft = new TexturedObject("MusicLeft");
    arrowMusicLeft->setTexture("../Resource/Texture/UI/Setting/ArrowLeft.png");
    arrowMusicLeft->getTransform().setScale({arrowW, arrowW, 0.0f});
    arrowMusicLeft->getTransform().setPosition({leftX, MUSIC_Y, 0.0f});
    objectsList.push_back(arrowMusicLeft);

    buttons[2] = new Button("MusicLeftButton", "");
    buttons[2]->getTransform().setScale({arrowW, arrowW, 0.0f});
    buttons[2]->getTransform().setPosition({leftX, MUSIC_Y, 0.0f});
    buttons[2]->setOnClickCallback([]() {
        auto& audio = GameEngine::getInstance()->getAudioEngine();
        audio.adjustMusicVolume(-DELTA);
        audio.printVolume();
    });
    objectsList.push_back(buttons[2]);

    arrowMusicRight = new TexturedObject("MusicRight");
    arrowMusicRight->setTexture("../Resource/Texture/UI/Setting/ArrowRight.png");
    arrowMusicRight->getTransform().setScale({arrowW, arrowW, 0.0f});
    arrowMusicRight->getTransform().setPosition({rightX, MUSIC_Y, 0.0f});
    objectsList.push_back(arrowMusicRight);

    buttons[3] = new Button("MusicRightButton", "");
    buttons[3]->getTransform().setScale({arrowW, arrowW, 0.0f});
    buttons[3]->getTransform().setPosition({rightX, MUSIC_Y, 0.0f});
    buttons[3]->setOnClickCallback([]() {
        auto& audio = GameEngine::getInstance()->getAudioEngine();
        audio.adjustMusicVolume(+DELTA);
        audio.printVolume();
    });
    objectsList.push_back(buttons[3]);

    selectedButtonIndex = 0;
    buttons[0]->setFocused(true);
    updateArrowHighlight();
}

void UISetting::updateArrowHighlight() {
    for (int i = 0; i < 6; ++i) {
        buttons[i]->setFocused(false);
    }
    if (selectedButtonIndex >= 0 && selectedButtonIndex < 6) {
        buttons[selectedButtonIndex]->setFocused(true);
    }
}

void UISetting::adjustVolume(int buttonIndex) {
    if (buttonIndex >= 0 && buttonIndex < 6) {
        buttons[buttonIndex]->getOnClickCallback()();
    }
}

void UISetting::updateUI() {
    if (currentPage == Page::AUDIO) {

        blackdrop->getTransform().setScale({ 20.0f, 20.0f, 0.0f });
        AudioPageTex->getTransform().setScale({ 16.0f, 9.0f, 0.0f });
        ControlPageTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        keyboardTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        mouseTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

        audioText->getTransform().setScale({ 4.0f, 1.0f, 0.0f });    
        controlText->getTransform().setScale({ 4.0f, 1.0f, 0.0f });

        if (audioTextAnim) {
            audioTextAnim->updateCurrentState();
        }

        const float zeroScale[3] = { 0.0f, 0.0f, 0.0f };
        const float normalScale[3] = { ARROW_SIZE, ARROW_SIZE, 0.0f };

        auto hideOrShow = [&](TexturedObject* arrow, int idx) {
            if (selectedButtonIndex == idx) {
                arrow->getTransform().setScale({ normalScale[0], normalScale[1], normalScale[2] });
            }
            else {
                arrow->getTransform().setScale({ zeroScale[0], zeroScale[1], zeroScale[2] });
            }
            };

        hideOrShow(arrowMasterLeft, 0);
        hideOrShow(arrowMasterRight, 1);
        hideOrShow(arrowMusicLeft, 2);
        hideOrShow(arrowMusicRight, 3);
        hideOrShow(arrowSFXLeft, 4);
        hideOrShow(arrowSFXRight, 5);

        masterVolumeBar->getTransform().setScale({ BAR_WIDTH, BAR_HEIGHT, 0.0f });
        soundEffectBar->getTransform().setScale({ BAR_WIDTH, BAR_HEIGHT, 0.0f });
        musicBar->getTransform().setScale({ BAR_WIDTH, BAR_HEIGHT, 0.0f });

        int currentSfxVol = Mix_Volume(-1, -1);
        int currentMusicVol = Mix_VolumeMusic(-1);
        int currentMasterVol = std::max(currentSfxVol, currentMusicVol);

        const float fullWidth = BAR_WIDTH;
        const float halfBar = fullWidth / 2.0f;
        const float barLeftX = BAR_XPOS - halfBar;

        {
            float masterFrac = computeFillFraction(currentMasterVol);
            float masterW = fullWidth * masterFrac;
            float masterCenterX = barLeftX + (masterW * 0.5f);
            masterFill->getTransform().setScale({ masterW, BAR_HEIGHT, 0.0f });
            masterFill->getTransform().setPosition({ masterCenterX, MASTER_Y, 0.0f });
        }

        {
            float sfxFrac = computeFillFraction(currentSfxVol);
            float sfxW = fullWidth * sfxFrac;
            float sfxCenterX = barLeftX + (sfxW * 0.5f);
            sfxFill->getTransform().setScale({ sfxW, BAR_HEIGHT, 0.0f });
            sfxFill->getTransform().setPosition({ sfxCenterX, SFX_Y, 0.0f });
        }

        {
            float musicFrac = computeFillFraction(currentMusicVol);
            float musicW = fullWidth * musicFrac;
            float musicCenterX = barLeftX + (musicW * 0.5f);
            musicFill->getTransform().setScale({ musicW, BAR_HEIGHT, 0.0f });
            musicFill->getTransform().setPosition({ musicCenterX, MUSIC_Y, 0.0f });
        }

        updateArrowHighlight();
    }
    else {
        blackdrop->getTransform().setScale({ 20.0f, 20.0f, 0.0f });

        AudioPageTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        ControlPageTex->getTransform().setScale({ 16.0f, 9.0f, 0.0f });

        keyboardTex->getTransform().setScale({ 6.0f, (100.0f / 231.0f) * 6.0f, 0.0f });
        keyboardTex->getTransform().setPosition({ -1.1f, -0.275f, 0.0f });

        mouseTex->getTransform().setScale({ (78.0f / 119.0f) * 3.5f, 3.5f, 0.0f });
        mouseTex->getTransform().setPosition({ 3.85f, 0.0f, 0.0f });

        audioText->getTransform().setScale({ 4.0f, 1.0f, 0.0f });
        controlText->getTransform().setScale({ 4.0f, 1.0f, 0.0f });
        if (controlTextAnim) {
            controlTextAnim->updateCurrentState();
        }

        masterVolumeBar->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        soundEffectBar->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        musicBar->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

        arrowMasterLeft->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        arrowMasterRight->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        arrowSFXLeft->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        arrowSFXRight->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        arrowMusicLeft->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        arrowMusicRight->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

        masterFill->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        sfxFill->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        musicFill->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    }
}


void UISetting::handleInput(SDL_Keycode key) {
    switch (key) {
    case SDLK_TAB:
        currentPage = (currentPage == Page::AUDIO) ? Page::CONTROL : Page::AUDIO;
        if (currentPage == Page::AUDIO) updateArrowHighlight();
        break;

    case SDLK_w:
    case SDLK_UP:
        if (currentPage == Page::AUDIO) {
            buttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex - 1 + 6) % 6;
            updateArrowHighlight();
        }
        break;

    case SDLK_s:
    case SDLK_DOWN:
        if (currentPage == Page::AUDIO) {
            buttons[selectedButtonIndex]->setFocused(false);
            selectedButtonIndex = (selectedButtonIndex + 1) % 6;
            updateArrowHighlight();
        }
        break;

    case SDLK_a:
    case SDLK_LEFT:
        if (currentPage == Page::AUDIO && (selectedButtonIndex % 2 == 0)) {
            adjustVolume(selectedButtonIndex);
        }
        break;

    case SDLK_d:
    case SDLK_RIGHT:
        if (currentPage == Page::AUDIO && (selectedButtonIndex % 2 == 1)) {
            adjustVolume(selectedButtonIndex);
        }
        break;

    case SDLK_RETURN:
    case SDLK_SPACE:
        if (currentPage == Page::AUDIO) {
            adjustVolume(selectedButtonIndex);
        }
        break;

    case SDLK_q:
        if (blackdrop) blackdrop->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

        if (AudioPageTex)    AudioPageTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (ControlPageTex)  ControlPageTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (keyboardTex)     keyboardTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (mouseTex)        mouseTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

        if (masterVolumeBar) masterVolumeBar->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (soundEffectBar)  soundEffectBar->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (musicBar)        musicBar->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

        if (arrowMasterLeft)  arrowMasterLeft->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (arrowMasterRight) arrowMasterRight->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (arrowSFXLeft)     arrowSFXLeft->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (arrowSFXRight)    arrowSFXRight->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (arrowMusicLeft)   arrowMusicLeft->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (arrowMusicRight)  arrowMusicRight->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

        if (masterFill) masterFill->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (sfxFill)    sfxFill->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (musicFill)  musicFill->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

        if (audioText)   audioText->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
        if (controlText) controlText->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

        for (auto* b : buttons) {
            if (b) {
                b->setFocused(false);
                b->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
            }
        }
        selectedButtonIndex = 0;
        currentPage = Page::AUDIO;
        break;
    }
}

void UISetting::hideAllSettings() {
    if (blackdrop)       blackdrop->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (AudioPageTex)    AudioPageTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (ControlPageTex)  ControlPageTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (keyboardTex)     keyboardTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (mouseTex)        mouseTex->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

    if (masterVolumeBar) masterVolumeBar->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (soundEffectBar)  soundEffectBar->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (musicBar)        musicBar->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

    if (arrowMasterLeft)  arrowMasterLeft->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (arrowMasterRight) arrowMasterRight->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (arrowSFXLeft)     arrowSFXLeft->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (arrowSFXRight)    arrowSFXRight->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (arrowMusicLeft)   arrowMusicLeft->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (arrowMusicRight)  arrowMusicRight->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

    if (masterFill) masterFill->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (sfxFill)    sfxFill->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (musicFill)  musicFill->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

    if (audioText)   audioText->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    if (controlText) controlText->getTransform().setScale({ 0.0f, 0.0f, 0.0f });

    for (auto* b : buttons) {
        if (b) b->getTransform().setScale({ 0.0f, 0.0f, 0.0f });
    }
}