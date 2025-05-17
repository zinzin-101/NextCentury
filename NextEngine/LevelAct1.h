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
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);

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