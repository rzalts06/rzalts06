#include "Winner.h"

Winner::Winner() : Scene { {0.0f}, nullptr   } {}
Winner::Winner(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

Winner::~Winner() { shutdown(); }

void Winner::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.bg = LoadMusicStream("Project4_AI/Audio/You'llAlwaysFindYourWayBackHome.wav");
    SetMusicVolume(mGameState.bg, 0.40f);
    PlayMusicStream(mGameState.bg);
    
    mGameState.winner = new P4Entity(
        mOrigin,
        {408.0f * 1.5f, 612.0f * 1.4f},
        "Project4_AI/Textures/Win.png", NONE);

    mGameState.hannah = new P4Entity(
        mOrigin,
        {196.0f * 0.3f, 344.0f * 0.3f},
        "Project4_AI/Textures/singleHannah.png", NONE);
}

void Winner::update(float deltaTime) {
    UpdateMusicStream(mGameState.bg);
}

void Winner::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.hannah->render();
    mGameState.winner->render();

    std::string resetMsg = "Press R to restart";
    DrawText(resetMsg.c_str(), mOrigin.x - 150.0f, 860, 30, WHITE);

}

void Winner::shutdown() {
    delete mGameState.winner;
    delete mGameState.hannah;

    UnloadMusicStream(mGameState.bg);
}