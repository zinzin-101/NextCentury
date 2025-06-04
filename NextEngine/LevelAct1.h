#pragma once

#include "Level.h"
#include "ObjectHeader.h"
#include "Audio.h"

class LevelAct1 : public Level {
private:
    list<DrawableObject*> objectsList;
    list<DrawableObject*> UIobjectsList;
    PlayerObject* player = nullptr;
    IngameUI* UIobject;
    queue<Dialogue*> dialogueList;
    list<InteractableObject*> interactableList;
    InteractableObject* it = nullptr;
    ProtagThoughts* p1 = nullptr;
    ProtagThoughts* p2 = nullptr;
    int interactCount = 0;
    FadeBlack* fb;
    bool isFadingToBlack = false;
    float fadetime = 1.0f;
    ColliderObject* set1Block;

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