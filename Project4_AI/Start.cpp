#include "Start.h"

Start::Start() : Scene { {0.0f}, nullptr   } {}
Start::Start(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

Start::~Start() { shutdown(); }

void Start::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.bg = LoadMusicStream("Project4_AI/Audio/BestOfBothWorlds.wav");
    SetMusicVolume(mGameState.bg, 0.40f);
    PlayMusicStream(mGameState.bg);

    float sizeRatio = 0.75f;

    mGameState.hannah = new P4Entity(
        {900.0f, 200.0f},
        {196.0f * 0.3f, 344.0f * 0.3f},
        "Project4_AI/Textures/singleHannah.png", NONE);

    mGameState.singleStage = new P4Entity(
        {900.0f, 270.0f},
        {426.0f * 0.3f, 226.0f * 0.3f},
        "Project4_AI/Textures/singleStage.png", NONE);

    mGameState.ATLogo = new P4Entity({mOrigin.x, mOrigin.y - 50.0f},
        {1536.0f * 0.5f, 1024.0f * 0.5f},
        "Project4_AI/Textures/ATLogo.png", NONE);

    mGameState.directions = new P4Entity(
        {mGameState.ATLogo->getPosition().x, mGameState.ATLogo->getPosition().y + 350.0f},
        {2048.0f * 0.25f, 1365.0f * 0.25f},
        "Project4_AI/Textures/directions.png", NONE);

}

void Start::update(float deltaTime) {
    UpdateMusicStream(mGameState.bg);
}

void Start::render() {
    BeginDrawing();

    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.singleStage->render();
    mGameState.hannah->render();
    mGameState.ATLogo->render();
    mGameState.directions->render();
}

void Start::shutdown() {
    delete mGameState.hannah;
    delete mGameState.singleStage;
    delete mGameState.ATLogo;
    delete mGameState.directions;

    UnloadMusicStream(mGameState.bg);
}