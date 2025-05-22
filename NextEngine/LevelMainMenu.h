#pragma once

#include "Level.h"
#include "ObjectHeader.h"
#include "UI.h"

class LevelMainMenu : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    UI* UIobject;
    queue<Dialogue*> dialogueList;
    list<InteractableObject*> interactableList;


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