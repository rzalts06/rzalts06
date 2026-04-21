#include "LoseScene1.h"

LoseScene1::LoseScene1() : Scene { {0.0f}, nullptr   } {}
LoseScene1::LoseScene1(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

LoseScene1::~LoseScene1() { shutdown(); }

void LoseScene1::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.effect = new Effects(mOrigin, float(1200 * 1.25f), float(900 * 1.25f));
    mGameState.effect->start(FADEIN);
    mGameState.effect->setEffectSpeed(0.60f);

    mGameState.loseMusic = LoadMusicStream("Project5/Audio/loseSound.wav");
    SetMusicVolume(mGameState.loseMusic, 0.20f);
    PlayMusicStream(mGameState.loseMusic);

    mGameState.loseScreen1 = new Entity(mOrigin,
        {1200.0f, 1000.0f},
        "Project5/Textures/loseScene.png", NONE);
}

void LoseScene1::update(float deltaTime) {
    UpdateMusicStream(mGameState.loseMusic);
    mGameState.effect->update(deltaTime, &mOrigin);
}

void LoseScene1::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.loseScreen1->render();
    mGameState.effect->render();
}

void LoseScene1::shutdown() {
    UnloadMusicStream(mGameState.loseMusic);
    delete mGameState.loseScreen1;

    delete mGameState.effect;
    mGameState.effect = nullptr;
}