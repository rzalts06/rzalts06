#include "level3.h"
#include "Start.h"
#include "Winner.h"
#include "Loser.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1200,
              SCREEN_HEIGHT = 900,
              FPS           = 120,
              NUMBER_OF_LEVELS = 6,
              NUMBER_OF_LIVES   = 3;

constexpr Vector2 ORIGIN           = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f,
                LIVE_DIMENSION = 50.0f,
                LOSING_LIVES_BUFFER = 2.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f,
      END_GAME_THRESHOLD = 800.0f,
      gLiveBufferTime = 0.0f;

int   livesLeft = 3;

bool gLost = false;

Camera2D gCamera = { 0 };

Scene* gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

Start* gStart = nullptr;
Level1* gLevel1 = nullptr;
Level2* gLevel2 = nullptr;
Level3* gLevel3 = nullptr;
Winner* gWinner = nullptr;
Loser* gLoser = nullptr;

P4Entity* lives = nullptr;
P4Entity* emptyLive = nullptr;
P4Entity* cheeseCounter = nullptr;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{
    gCurrentScene = scene;
    gCurrentScene->initialise();
    if (gCurrentScene->getState().hannah != nullptr) gCamera.target = gCurrentScene->getState().hannah->getPosition();
    else gCamera.target = gCurrentScene->getState().ATLogo->getPosition();
}

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 4 -  AI");
    InitAudioDevice();

    gStart = new Start(ORIGIN, "#7C507A");
    gLevel1 = new Level1(ORIGIN, "#8A5A88");
    gLevel2 = new Level2(ORIGIN, "#9A6A98");
    gLevel3 = new Level3(ORIGIN, "#A878A6");
    gWinner= new Winner(ORIGIN, "#6E466C");
    gLoser = new Loser(ORIGIN, "#603C5E");

    gLevels.push_back(gStart);
    gLevels.push_back(gLevel1);
    gLevels.push_back(gLevel2);
    gLevels.push_back(gLevel3);
    gLevels.push_back(gWinner);
    gLevels.push_back(gLoser);

    switchToScene(gLevels[0]);

    lives = new P4Entity[NUMBER_OF_LIVES];
    emptyLive = new P4Entity[NUMBER_OF_LIVES];

    float rightMostX = SCREEN_WIDTH - (NUMBER_OF_LIVES * LIVE_DIMENSION);

    for (int i = 0; i < NUMBER_OF_LIVES; i++) {
        lives[i].setTexture("Project4_AI/Textures/MusicLife.png");
        lives[i].setEntityType(BLOCK);
        lives[i].setScale({LIVE_DIMENSION, LIVE_DIMENSION});
        lives[i].setColliderDimensions({LIVE_DIMENSION, LIVE_DIMENSION});
        lives[i].setPosition({
            rightMostX + i * LIVE_DIMENSION,
            30.0f
        });

        emptyLive[i].setTexture("Project4_AI/Textures/musicOutline.png");
        emptyLive[i].setEntityType(BLOCK);
        emptyLive[i].setScale({LIVE_DIMENSION, LIVE_DIMENSION});
        emptyLive[i].setColliderDimensions({LIVE_DIMENSION, LIVE_DIMENSION});
        emptyLive[i].setPosition({lives[i].getPosition()});
    }

    cheeseCounter = new P4Entity(
    {35.0f, 35.0f},
    {225.0f * 0.2f, 225.0f * 0.2f},
    "Project4_AI/Textures/cheeseCounter.png", NONE);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}


void processInput() {
    gCurrentScene->getState().hannah->resetMovement();

    if      (IsKeyDown(KEY_A)) gCurrentScene->getState().hannah->moveLeft();
    else if (IsKeyDown(KEY_D)) gCurrentScene->getState().hannah->moveRight();

    if (IsKeyDown(KEY_S)) gCurrentScene->getState().hannah->moveDown();

    if (IsKeyPressed(KEY_SPACE)) gCurrentScene->getState().hannah->jump();

    if (IsKeyPressed(KEY_R)) {
        switchToScene(gLevels[0]);

        livesLeft = 3;

        gLevel1->setCollectedCheese(0);
        gLevel2->setCollectedCheese(0);
        gLevel3->setCollectedCheese(0);

        gLost = false;
    }

    if (IsKeyPressed(KEY_ENTER)) switchToScene(gLevels[1]);
    if (IsKeyPressed(KEY_ONE)) switchToScene(gLevels[1]);
    if (IsKeyPressed(KEY_TWO)) switchToScene(gLevels[2]);
    if (IsKeyPressed(KEY_THREE)) switchToScene(gLevels[3]);
    if (IsKeyPressed(KEY_W)) switchToScene(gLevels[4]);
    if (IsKeyPressed(KEY_L)) switchToScene(gLevels[5]);

    if (GetLength(gCurrentScene->getState().hannah->getMovement()) > 1.0f) gCurrentScene->getState().hannah->normaliseMovement();
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);

        if (gLiveBufferTime > 0.0f) {
            gLiveBufferTime -= FIXED_TIMESTEP;
        }

        if (gLiveBufferTime < 0.0f) gLiveBufferTime = 0.0f;

        if (gCurrentScene->getState().paps != nullptr || gCurrentScene->getState().selener != nullptr || gCurrentScene->getState().jakey != nullptr) {
            if (gLiveBufferTime <= 0.0 && gCurrentScene->getState().hannah->collideWith(gCurrentScene->getState().paps)) {
                PlaySound(gCurrentScene->getState().cameraShutters);
                if (livesLeft > 0) --livesLeft;
                gLiveBufferTime = LOSING_LIVES_BUFFER;
            }

            if (gLiveBufferTime <= 0.0 && gCurrentScene->getState().hannah->collideWith(gCurrentScene->getState().selener)) {
                PlaySound(gCurrentScene->getState().SGHMFight);
                if (livesLeft > 0) --livesLeft;
                gLiveBufferTime = LOSING_LIVES_BUFFER;
            }

            if (gLiveBufferTime <= 0.0 && gCurrentScene->getState().hannah->collideWith(gCurrentScene->getState().jakey)) {
                PlaySound(gCurrentScene->getState().kissCheer);
                if (livesLeft > 0) --livesLeft;
                gLiveBufferTime = LOSING_LIVES_BUFFER;
            }

            if (livesLeft <= 0 && !gLost) {
                gLost = true;
                PlaySound(gCurrentScene->getState().sadOOO);
                switchToScene(gLevels[5]);
            }
        }

        deltaTime -= FIXED_TIMESTEP;

        if (gCurrentScene != gStart && gCurrentScene != gWinner && gCurrentScene != gLoser) {
            Vector2 currentPlayerPosition = { gCurrentScene->getState().hannah->getPosition().x, ORIGIN.y };
            panCamera(&gCamera, &currentPlayerPosition);
        }

    }
    if (gCurrentScene->getState().hannah->getPosition().y > END_GAME_THRESHOLD) { //gCurrentScene->getState().hannah != nullptr &&
        switchToScene(gLevel1);
        gLevel1->setCollectedCheese(0);
        gLevel2->setCollectedCheese(0);
        gLevel3->setCollectedCheese(0);
    }
}

void render() {
    BeginDrawing();

    if (gCurrentScene == gStart || gCurrentScene == gWinner || gCurrentScene == gLoser) {
        gCurrentScene->render();
    }

    else {
        BeginMode2D(gCamera);
        gCurrentScene->render();
        EndMode2D();
    }

    for (int i = 0; i < livesLeft; i++) lives[i].render();
    for (int i = 0; i < NUMBER_OF_LIVES; i++) emptyLive[i].render();
    cheeseCounter->render();

    if (gCurrentScene == gWinner) {
        int collected = 9;
        std::string collectedCheese = std::to_string(collected);
        DrawText(collectedCheese.c_str(), 75, 25, 30, WHITE);
    }
    else {
        int collected = gCurrentScene->getCollectedCheese();
        std::string collectedCheese = std::to_string(collected);
        DrawText(collectedCheese.c_str(), 75, 25, 30, WHITE);
    }


    EndDrawing();
}

void shutdown()
{
    delete gLevel1;
    delete gLevel2;
    delete gLevel3;
    delete gStart;
    delete gLoser;
    delete gWinner;

    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;
    delete[] lives;
    delete[] emptyLive;
    delete cheeseCounter;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID > 0)
        {
            int id = gCurrentScene->getState().nextSceneID;
            switchToScene(gLevels[id]);
        }

        render();
    }

    shutdown();

    return 0;
}