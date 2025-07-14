#pragma once

#include "Level.h"
#include "ObjectHeader.h"
#include "UI.h"

class LevelTutorial : public Level {
public:
    LevelTutorial();
    virtual ~LevelTutorial();

    virtual void levelLoad() override;
    virtual void levelInit() override;
    virtual void levelUpdate() override;
    virtual void levelDraw() override;
    virtual void levelFree() override;
    virtual void levelUnload() override;
    virtual void handleKey(InputManager& input) override;

private:
    std::list<DrawableObject*> objectsList;
    UI* UIobject = nullptr;
    TexturedObject* tutorialPage = nullptr;
	SimpleObject* bg = nullptr;

    enum class FadeState { FadingIn, Visible, FadingOut, Done };
    
    FadeState fadeState;
    float fadeTimer;
    const float fadeDuration = 5.0f;   
    const float holdDuration = 2.0f;    
};
