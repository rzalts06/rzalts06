#include "LoseScene2.h"

LoseScene2::LoseScene2() : Scene { {0.0f}, nullptr   } {}
LoseScene2::LoseScene2(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

LoseScene2::~LoseScene2() { shutdown(); }

void LoseScene2::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.effect = new Effects(mOrigin, float(1200 * 1.25f), float(900 * 1.25f));
    mGameState.effect->start(FADEIN);
    mGameState.effect->setEffectSpeed(0.60f);

    mGameState.loseMusic = LoadMusicStream("Project5/Audio/loseSound.wav");
    SetMusicVolume(mGameState.loseMusic, 0.20f);
    PlayMusicStream(mGameState.loseMusic);

    mGameState.loseScreen2 = new Entity(mOrigin,
        {1200.0f, 1000.0f},
        "Project5/Textures/loseScene2.png", NONE);
}

void LoseScene2::update(float deltaTime) {
    UpdateMusicStream(mGameState.loseMusic);
    mGameState.effect->update(deltaTime, &mOrigin);
}

void LoseScene2::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.loseScreen2->render();
    mGameState.effect->render();
}

void LoseScene2::shutdown() {
    UnloadMusicStream(mGameState.loseMusic);
    delete mGameState.loseScreen2;

    delete mGameState.effect;
    mGameState.effect = nullptr;
}