#include "Loser.h"

Loser::Loser() : Scene { {0.0f}, nullptr   } {}
Loser::Loser(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

Loser::~Loser() { shutdown(); }

void Loser::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.bg = LoadMusicStream("Project4_AI/Audio/Nobody'sPerfect.wav");
    SetMusicVolume(mGameState.bg, 0.40f);
    PlayMusicStream(mGameState.bg);

    mGameState.loser = new P4Entity(
        mOrigin,
        {433.0f * 1.6f, 577.0f * 1.5f},
        "Project4_AI/Textures/Loss.png", NONE);

    mGameState.hannah = new P4Entity(
        mOrigin,
        {196.0f * 0.3f, 344.0f * 0.3f},
        "Project4_AI/Textures/singleHannah.png", NONE);
}

void Loser::update(float deltaTime) {
    UpdateMusicStream(mGameState.bg);
}

void Loser::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.hannah->render();
    mGameState.loser->render();

    std::string resetMsg = "Press R to restart";
    DrawText(resetMsg.c_str(), mOrigin.x - 150.0f, 860, 30, WHITE);

}

void Loser::shutdown() {
    delete mGameState.loser;
    delete mGameState.hannah;

    UnloadMusicStream(mGameState.bg);
}