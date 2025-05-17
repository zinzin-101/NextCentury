#include "pch.h"
#include "LevelAct1.h"
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

#define EFFECT_PATH   "../Resource/Audio/SoundEffect/"
#define MUSIC_PATH    "../Resource/Audio/Music/"

void LevelAct1::levelLoad() {
    // load meshes
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);

    // loading screen...
    addLoadingScreen(objectsList);
    levelDraw();
    removeLoadingScreen(objectsList);
}

void LevelAct1::levelInit() {
    // Initialize audio engine with effect and music folders
    m_audio.init(EFFECT_PATH, MUSIC_PATH);

    // Load sound effects and music by file names
    m_jumpSfx = m_audio.loadSoundEffect("../Resource/Audio/SoundEffect/Rolling.wav");
    m_deathSfx = m_audio.loadSoundEffect("../Resource/Audio/SoundEffect/Rolling.wav");

    // UI setup
    UIobject = new IngameUI();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);

    // create parallax layers
    float pictureWidth = 1920.0f;
    float pictureHeight = 360.0f;
    auto addLayer = [&](const std::string& file, float depth, float yOffset = 0.0f) {
        ParallaxObject* layer = new ParallaxObject(0.0f, yOffset, depth, false, player, true, pictureWidth, pictureHeight);
        layer->setTexture(file);
        objectsList.emplace_back(layer);
        };

    addLayer("../Resource/Texture/Act1/City_P01_Sky.png", 100.0f);
    addLayer("../Resource/Texture/Act1/City_P02_City1.png", 50.0f);
    addLayer("../Resource/Texture/Act1/City_P03_City2.png", 35.0f);
    addLayer("../Resource/Texture/Act1/City_P04_City3.png", 30.0f, -0.5f);
    addLayer("../Resource/Texture/Act1/City_P05_NewsBoardandBins.png", 20.0f, -0.5f);
    addLayer("../Resource/Texture/Act1/City_P09_Lightpole.png", 15.0f, -0.5f);
    addLayer("../Resource/Texture/Act1/City_P10_Car.png", 10.0f, -0.5f);
    addLayer("../Resource/Texture/Act1/City_P13_Fog.png", 60.0f, 1.0f);
    addLayer("../Resource/Texture/Act1/City_P11_Ground.png", 0.0f, -0.5f);

    // Import transforms
    Level::importTransformData(objectsList, "alpha1", false);

    // Player setup
    player = new PlayerObject();
    player->getTransform().setScale(4.166f, 2.5f);
    player->getColliderComponent()->getTransform().translate(0.0f, -0.44f);
    player->getColliderComponent()->setDimension(0.25f, 0.65f);
    objectsList.emplace_back(player);

    // Camera
    auto camera = GameEngine::getInstance()->getRenderer()->getCamera();
    camera->setTarget(player);
    GameEngine::getInstance()->getRenderer()->setToggleViewport(false);

    // Parallax player binding
    for (auto* obj : objectsList) {
        if (auto* pObj = dynamic_cast<ParallaxObject*>(obj)) pObj->setPlayer(player);
    }

    startObjects(objectsList);
    player->getDamageCollider()->setFollowOffset({ 1.0f, -0.2f, 0 });

    // Add foreground pole
    ParallaxObject* pole = new ParallaxObject(0.0f, 0.0f, 0.0f, false, player, true, pictureWidth, pictureHeight);
    pole->setTexture("../Resource/Texture/Act1/City_P12_FGPole.png");
    objectsList.emplace_back(pole);

    // Camera limits and freeze
    camera->setDeadLimitBool(true);
    camera->setDeadLimitMinMax(-5.0f, 60.0f);
    camera->setOffset({ 0.0f, -0.5f, 0.0f });
    GameEngine::getInstance()->getRenderer()->setToggleViewport(true);
    GameEngine::getInstance()->freezeGameForSeconds(0.5f);
}

void LevelAct1::levelUpdate() {
    updateObjects(objectsList);
    GameEngine::getInstance()->getRenderer()->updateCamera({});

    // Death logic with sound
    for (auto itr = objectsList.begin(); itr != objectsList.end(); ) {
        if (auto* enemy = dynamic_cast<EnemyObject*>(*itr)) {
            if (enemy->getHealth() <= 0) {
                m_deathSfx.play();                 
                DrawableObject::destroyObject(enemy);
                itr = objectsList.erase(itr);
                continue;
            }
        }
        ++itr;
    }

    GameEngine::getInstance()->getRenderer()->updateCamera({});
}

void LevelAct1::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
#ifdef DEBUG_MODE_ON
    drawImGui(objectsList);
#endif
}

void LevelAct1::levelFree() {
    // Stop music and destroy audio
    m_audio.stopMusic();
    m_audio.destroy();

    for (auto* obj : objectsList) delete obj;
    objectsList.clear();
    delete UIobject;
}

void LevelAct1::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    GameEngine::getInstance()->getRenderer()->setClearColor(0.1f, 0.1f, 0.1f);
}

void LevelAct1::handleKey(InputManager& input) {
    // debugging zoom
    if (input.getButton(SDLK_z)) GameEngine::getInstance()->getRenderer()->increaseZoomRatio(0.1f);
    if (input.getButton(SDLK_x)) GameEngine::getInstance()->getRenderer()->decreaseZoomRatio(0.1f);

    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    if (input.getButtonDown(SDLK_SPACE)) {
		GameEngine::getInstance()->playSoundEffect("Rolling.wav");
    }
    /// Process key ///
   // add key that requires hold duration here
    processHeldKey(input, SDLK_k);
    processHeldMouse(input, SDL_BUTTON_LEFT);

    processHeldKey(input, SDLK_u);
    processHeldMouse(input, SDL_BUTTON_MIDDLE);

    // add key that requires buffering here
    processKeyBuffer(input, SDLK_SPACE);
    processKeyBuffer(input, SDLK_LSHIFT);

    // handle event here
    if (input.getButton(SDLK_a) && !input.getButton(SDLK_d)) player->move(glm::vec2(-1, 0));
    if (input.getButton(SDLK_d) && !input.getButton(SDLK_a)) player->move(glm::vec2(1, 0));
    if (input.getButtonDown(SDLK_j)) player->parryAttack();
    if (input.getMouseButtonDown(SDL_BUTTON_RIGHT)) player->parryAttack();

    /// Use processed key here ///
    if (keyHeldDuration[SDLK_k] < PlayerStat::DURATION_TO_START_HEAVY_ATTACK) {
        if (input.getButtonUp(SDLK_k)) {
            player->normalAttack();
        }
    }
    else {
        if (input.getButtonUp(SDLK_k)) {
            player->heavyAttack();
        }
        else if (input.getButton(SDLK_k)) {
            player->startHeavyAttack();
        }
    }

    if (mouseHeldDuration[SDL_BUTTON_LEFT] < PlayerStat::DURATION_TO_START_HEAVY_ATTACK) {
        if (input.getMouseButtonUp(SDL_BUTTON_LEFT)) {
            player->normalAttack();
        }
    }
    else {
        if (input.getMouseButtonUp(SDL_BUTTON_LEFT)) {
            player->heavyAttack();
        }
        else if (input.getMouseButton(SDL_BUTTON_LEFT)) {
            player->startHeavyAttack();
        }
    }

    if (input.getButtonUp(SDLK_u)) {
        player->rangeAttack(objectsList);
    }
    else if (input.getButton(SDLK_u)) {
        player->startRangeAttack(dt);
    }

    if (input.getMouseButtonUp(SDL_BUTTON_MIDDLE)) {
        player->rangeAttack(objectsList);
    }
    else if (input.getMouseButton(SDL_BUTTON_MIDDLE)) {
        player->startRangeAttack(dt);
    }

    if ((isKeyInBuffer(SDLK_LSHIFT) || (isKeyInBuffer(SDLK_SPACE))) && player->getCanMove()) {
        clearKeyBuffer(SDLK_SPACE);
        clearKeyBuffer(SDLK_LSHIFT);

        if (input.getButton(SDLK_a)) {
            player->dodge(-1.0f);
        }
        else if (input.getButton(SDLK_d)) {
            player->dodge(1.0f);
        }
        else {
            player->dodge();
        }
    }
}