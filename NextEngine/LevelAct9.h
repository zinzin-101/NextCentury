#pragma once

#include "Level.h"
#include "ObjectHeader.h"
#include "Audio.h"

class LevelAct9 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;

    InteractableObject* door;
    FadeBlack* fb;
    bool isFadingToBlack = false;
    float timefade = 1.0f;
    bool end = false;

    AudioEngine m_audio;
    SoundEffect m_jumpSfx;
    SoundEffect m_deathSfx;
    Music       m_backgroundMusic;
public:
    virtual void levelLoad();
    virtual void levelInit();
    virtual void levelUpdate();
    virtual void levelDraw();
    virtual void levelFree();
    virtual void levelUnload();

    virtual void handleKey(InputManager& input);
};