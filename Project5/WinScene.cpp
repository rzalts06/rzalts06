#include "WinScene.h"

WinScreen::WinScreen() : Scene { {0.0f}, nullptr   } {}
WinScreen::WinScreen(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

WinScreen::~WinScreen() { shutdown(); }

void WinScreen::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.effect = new Effects(mOrigin, float(1200 * 1.25f), float(900 * 1.25f));
    mGameState.effect->start(FADEIN);
    mGameState.effect->setEffectSpeed(0.60f);

    mGameState.winMusic = LoadMusicStream("Project5/Audio/winSound.wav");
    SetMusicVolume(mGameState.winMusic, 0.10f);
    PlayMusicStream(mGameState.winMusic);

    mGameState.winScreen = new Entity(mOrigin,
        {1200.0f, 1000.0f},
        "Project5/Textures/winScene.png", NONE);
}

void WinScreen::update(float deltaTime) {
    UpdateMusicStream(mGameState.winMusic);
    mGameState.effect->update(deltaTime, &mOrigin);
}

void WinScreen::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.winScreen->render();
    mGameState.effect->render();
}

void WinScreen::shutdown() {
    UnloadMusicStream(mGameState.winMusic);
    delete mGameState.winScreen;

    delete mGameState.effect;
    mGameState.effect = nullptr;
}