#pragma once

#include "Level.h"

class LevelCredit : public Level {
private:
    list<DrawableObject*> objectsList;

    AudioEngine m_audio;
    SoundEffect m_jumpSfx;
    SoundEffect m_deathSfx;
    Music       m_backgroundMusic;

	SimpleObject* blackdrop;
	TexturedObject* creditText; 

public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};