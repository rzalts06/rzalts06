#include "HomeScreen.h"

HomeScreen::HomeScreen() : Scene { {0.0f}, nullptr   } {}
HomeScreen::HomeScreen(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

HomeScreen::~HomeScreen() { shutdown(); }

void HomeScreen::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.effect = new Effects(mOrigin, float(1200 * 1.25f), float(900 * 1.25f));
    mGameState.effect->start(FADEIN);
    mGameState.effect->setEffectSpeed(0.3f);

    mGameState.homeScreenMusic = LoadMusicStream("Project5/Audio/UnderTheSea.wav");
    SetMusicVolume(mGameState.homeScreenMusic, 0.50f);
    PlayMusicStream(mGameState.homeScreenMusic);

    mGameState.homeScreen = new Entity(mOrigin,
        {1200.0f, 900.0f},
        "Project5/Textures/menuScene.png", NONE);

    mGameState.directions = new Entity({mOrigin.x - 70.0f, mOrigin.y},
        {500.0f, 500.0f},
        "Project5/Textures/directions.png", NONE);
    mGameState.directions->deactivate();
}

void HomeScreen::update(float deltaTime) {
    UpdateMusicStream(mGameState.homeScreenMusic);
    mGameState.effect->update(deltaTime, &mOrigin);
}

void HomeScreen::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.homeScreen->render();

    DrawText(TextFormat("Press and hold 'I' to see instructions"),
        SCREEN_WIDTH  - 600.0f, 200.0f, 19.0f, BLACK);

    mGameState.directions->render();
    mGameState.effect->render();
}

void HomeScreen::shutdown() {
    delete mGameState.homeScreen;
    delete mGameState.directions;

    delete mGameState.effect;
    mGameState.effect = nullptr;

    UnloadMusicStream(mGameState.homeScreenMusic);
}