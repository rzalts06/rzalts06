#include "P3cs3113.h"
#include "P3Entity.h"
using namespace std;

/**
* Author: Rachelle Zaltsman
* Assignment: Lunar Lander
* Date due: 03/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


// Global Constants
enum Level {L0, L1, L2, L3};

constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 980,
              FPS           = 120,
              NUMBER_OF_BEDS_AT_A_TIME = 3;

constexpr char    BG_COLOUR[] = "#B5E5F8";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float ACCELERATION_OF_GRAVITY = 0.5f,
                FIXED_TIMESTEP          = 1.0f / 60.0f,
                END_GAME_THRESHOLD      = 800.0f;

// Global Variables
Level gLevel = L0;
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f,
      sizeRatio  = 0.3f;


bool isGravityOn = false;
bool isGameOver = false;

Entity *gBlueBed = nullptr;
Entity *gGrayBed = nullptr;
Entity *gCubeBed = nullptr;
Entity *gFrogBed = nullptr;
Entity *gPinkBed = nullptr;
Entity *gScratchPost = nullptr;
Entity *gMovingBed = nullptr;
Entity *gAura = nullptr;
Entity *gCat = nullptr;

Entity *gWin = nullptr;
Entity *gLose = nullptr;
Entity *gInstructions = nullptr;

Entity *g100Score = nullptr;
Entity *g5000Score = nullptr;
Entity *g0Score = nullptr;

Music bridgerton360;


void setLevel(Level l);
void resetGame();
// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 3: Lunar Lander");
    InitAudioDevice();

    std::map<Direction, std::vector<int>> animationAtlas = {
        {DOWN,  { 0, 1, 2, 3}},
        {LEFT,  { 4, 5, 6, 7 }},
        {RIGHT,  { 8, 9, 10, 11 }},
        {PURR, { 12, 13, 14, 15 }},
        {ANGRY, { 16, 17, 18, 19 }}
    };

    gCat = new Entity({SCREEN_WIDTH / 2.0f, 20.0f},
        {250.0f * sizeRatio, 250.0f * sizeRatio},
        "Project3_LunarLander/Textures/cat_sprite.png", ATLAS, { 5, 4 },
        animationAtlas);

    gGrayBed = new Entity({ORIGIN.x - 300.0f, SCREEN_HEIGHT - 70.0f},
        {288.0f * sizeRatio, 294.0f * sizeRatio}, "Project3_LunarLander/Textures/grayBed.png");

    gFrogBed = new Entity({ ORIGIN.x - 400.0f , SCREEN_HEIGHT - 490.0f},
        {314.0f * sizeRatio, 338.0f * sizeRatio}, "Project3_LunarLander/Textures/frogBed.png");

    gBlueBed = new Entity({ORIGIN.x - 130.0f, SCREEN_HEIGHT - 300.0f},
        {362.0f * 0.25f, 202.0f * 0.25f}, "Project3_LunarLander/Textures/blue_bed.png");

    gCubeBed = new Entity({ORIGIN.x + 100.0f, SCREEN_HEIGHT - 200.0f},
        {288.0f * sizeRatio, 286.0f * sizeRatio}, "Project3_LunarLander/Textures/cubeBed.png");

    gPinkBed = new Entity({ORIGIN.x + 300.0f, ORIGIN.y + 100.0f},
        {362.0f * sizeRatio, 202.0f * sizeRatio}, "Project3_LunarLander/Textures/pink_bed.png");

    gScratchPost = new Entity({ORIGIN.x + 450.0f, ORIGIN.y + 400.0f},
        {302.0f * sizeRatio, 364.0f * sizeRatio}, "Project3_LunarLander/Textures/scratchPost.png");

    gMovingBed = new Entity({ORIGIN.x + 450.0f, ORIGIN.y - 200.0f},
        {542.0f * 0.1f, 460.0f * 0.1f}, "Project3_LunarLander/Textures/moving_platform.png");

    gAura = new Entity[NUMBER_OF_BEDS_AT_A_TIME];

    gLose = new Entity({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f}, {492.0f * 0.67f, 473.0f *  0.67f},
        "Project3_LunarLander/Textures/lose.png");

    gWin = new Entity({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f}, {492.0f *  0.67f, 473.0f *  0.67f},
        "Project3_LunarLander/Textures/win.png");

    gInstructions = new Entity({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f}, {500.0f *  0.67f, 500.0f *  0.67f},
        "Project3_LunarLander/Textures/instructions.png");

    g100Score = new Entity({SCREEN_WIDTH / 2.0f, 40.0f}, {535.0f * sizeRatio , 438.0f * sizeRatio },
        "Project3_LunarLander/Textures/100.png");

    g5000Score = new Entity({SCREEN_WIDTH / 2.0f, 40.0f}, {347.0f * sizeRatio , 175.0f * sizeRatio },
    "Project3_LunarLander/Textures/5000.png");

    g0Score = new Entity({SCREEN_WIDTH / 2.0f, 40.0f}, {128.0f * sizeRatio  , 238.0f * sizeRatio },
    "Project3_LunarLander/Textures/0.png");

    bridgerton360 = LoadMusicStream("Project3_LunarLander/360BridgertonCover.wav");
    SetMusicVolume(bridgerton360, 0.50f);
    PlayMusicStream(bridgerton360);

    SetTargetFPS(FPS);
}

void processInput()
{
    if (isGameOver == false) {
        gCat->resetMovement();

        if (IsKeyPressed(KEY_ONE)) setLevel(L1);
        else if (IsKeyPressed(KEY_TWO)) setLevel(L2);
        else if (IsKeyPressed(KEY_THREE)) setLevel(L3);
        
        if (IsKeyPressed(KEY_SPACE) && !isGravityOn) {
            isGravityOn = true;
            gCat->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
        }

        if (IsKeyDown(KEY_A) && gCat->getFuel() > 0.0f) {
            gCat->moveLeft();
            gCat->subFuel(0.1);
            gCat->setAcceleration({100.0f, gCat->getAcceleration().y});
        }
        else if (IsKeyDown(KEY_D) && gCat->getFuel() > 0.0f) {
            gCat->moveRight();
            gCat->subFuel(0.1);
            gCat->setAcceleration({-100.0f, gCat->getAcceleration().y});
        }

        if (IsKeyDown(KEY_S)) {
            gCat->moveDown();
            gCat->subFuel(0.2);
        }

        if (GetLength(gCat->getMovement()) > 1.0f)
            gCat->normaliseMovement();
    }

    if (IsKeyPressed(KEY_P)) resetGame();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
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
        if (!isGameOver) {
            if (gLevel == L1) {
                if (gAura[0].isActive()) gAura[0].setPosition({ gMovingBed->getPosition().x , gMovingBed->getPosition().y + 20.0f });
                if (gAura[1].isActive()) gAura[1].setPosition({ gBlueBed->getPosition().x , gBlueBed->getPosition().y + 15.0f });
                if (gAura[2].isActive()) gAura[2].setPosition({ gScratchPost->getPosition().x , gScratchPost->getPosition().y + 45.0f });

                gCat->update(FIXED_TIMESTEP, gBlueBed, 1, true,  100);
                gCat->update(FIXED_TIMESTEP, gScratchPost, 1, true,  100);
                gCat->update(FIXED_TIMESTEP, gMovingBed, 1, true,  5000);
                gCat->update(FIXED_TIMESTEP, gCubeBed, 1, false,  0);
                gCat->update(FIXED_TIMESTEP, gFrogBed, 1, false,  0);
                gCat->update(FIXED_TIMESTEP, gGrayBed, 1, false, 0);
                gCat->update(FIXED_TIMESTEP, gPinkBed, 1, false, 0);

            }

            if (gLevel == L2) {
                if (gAura[0].isActive()) gAura[0].setPosition({ gFrogBed->getPosition().x , gFrogBed->getPosition().y + 40.0f });
                if (gAura[1].isActive()) gAura[1].setPosition({ gGrayBed->getPosition().x , gGrayBed->getPosition().y + 35.0f });
                if (gAura[2].isActive()) gAura[2].setPosition({ gCubeBed->getPosition().x , gCubeBed->getPosition().y + 30.0f });

                gCat->update(FIXED_TIMESTEP, gCubeBed, 1, true, 100);
                gCat->update(FIXED_TIMESTEP, gFrogBed, 1, true,  5000);
                gCat->update(FIXED_TIMESTEP, gGrayBed, 1, true, 100);
                gCat->update(FIXED_TIMESTEP, gPinkBed, 1, false, 0);
                gCat->update(FIXED_TIMESTEP, gBlueBed, 1, false,  0);
                gCat->update(FIXED_TIMESTEP, gScratchPost, 1, false, 0);
                gCat->update(FIXED_TIMESTEP, gMovingBed, 1, false,  0);
            }

            if (gLevel == L3) {
                if (gAura[0].isActive()) gAura[0].setPosition({ gMovingBed->getPosition().x , gMovingBed->getPosition().y + 20.0f });
                if (gAura[1].isActive()) gAura[1].setPosition({ gGrayBed->getPosition().x , gGrayBed->getPosition().y + 35.0f });
                if (gAura[2].isActive()) gAura[2].setPosition({ gPinkBed->getPosition().x , gPinkBed->getPosition().y + 20.0f });

                gCat->update(FIXED_TIMESTEP, gPinkBed, 1, true, 5000);
                gCat->update(FIXED_TIMESTEP, gMovingBed, 1, true, 100);
                gCat->update(FIXED_TIMESTEP, gGrayBed, 1, true, 100);
                gCat->update(FIXED_TIMESTEP, gCubeBed, 1, false,  0);
                gCat->update(FIXED_TIMESTEP, gFrogBed, 1, false,  0);
                gCat->update(FIXED_TIMESTEP, gBlueBed, 1, false,  0);
                gCat->update(FIXED_TIMESTEP, gScratchPost, 1, false,  0);
            }
        }

        if (gCat->getPosition().x >= SCREEN_WIDTH - gCat->getScale().x / 2.0f) {
            gCat->setPosition({SCREEN_WIDTH - gCat->getScale().x / 2.0f, gCat->getPosition().y});
            gCat->setDirection(ANGRY);
            isGameOver = true;
        }
        else if (gCat->getPosition().x - gCat->getScale().x / 2.0f <= 0.0f) {
            gCat->setPosition({ gCat->getScale().x / 2.0f, gCat->getPosition().y});
            gCat->setDirection(ANGRY);
            isGameOver = true;
        }

        if (gCat->getPosition().y >= SCREEN_HEIGHT - gCat->getScale().y / 2.0f) {
            gCat->setPosition({gCat->getPosition().x, SCREEN_HEIGHT - gCat->getScale().x / 2.0f});
            gCat->setDirection(ANGRY);
            isGameOver = true;
        }

        if (gCat->getFuel() <= 0.0f) {
            gCat->setDirection(ANGRY);
            gCat->setAcceleration({0.0f, gCat->getAcceleration().y});

            isGameOver = true;

            gCat->setMovement({0.0f, 0.0f});
            gCat->setAcceleration({0.0f, 0.0f});
            gMovingBed->setMovement({0.0f, 0.0f});
        }

        gMovingBed->setSpeed(80);
        if (gMovingBed->getPosition().x >= ORIGIN.x + 450.0f) gMovingBed->setMovement({ -1.0f , 0.0f });
        else if (gMovingBed->getPosition().x <= ORIGIN.x - 450.0f) gMovingBed->setMovement({ 1.0f , 0.0f });

        for (int i = 0; i < NUMBER_OF_BEDS_AT_A_TIME; ++i) {
            gAura[i].update(FIXED_TIMESTEP, nullptr, 0, false,  0);
        }

        gBlueBed->update(FIXED_TIMESTEP, nullptr, 0, false,  0);
        gCubeBed->update(FIXED_TIMESTEP, nullptr,0, false, 0);
        gFrogBed->update(FIXED_TIMESTEP, nullptr,0, false,  0);
        gGrayBed->update(FIXED_TIMESTEP, nullptr,0, false, 0);
        gMovingBed->update(FIXED_TIMESTEP, nullptr,0, false,  0);
        gPinkBed->update(FIXED_TIMESTEP, nullptr, 0, false, 0);
        gScratchPost->update(FIXED_TIMESTEP, nullptr, 0, false,  0);

        if ((gCat->getDirection() == PURR) || (gCat->getDirection() == ANGRY)) isGameOver = true;

        if (isGameOver) {
            gCat->setMovement({0.0f, 0.0f});
            gCat->setAcceleration({0.0f, 0.0f});
            gCat->resetMovement();

            gMovingBed->setMovement({0.0f, 0.0f});

            isGravityOn = false;

            gCat->update(FIXED_TIMESTEP, nullptr, 0, false, 0);
        }

        if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
            if ((gCat->getAcceleration().x > 0.0f) && (gCat->getPosition().x < SCREEN_WIDTH / 2.0f)) {
                if (gCat->getPosition().x >= SCREEN_WIDTH / 2.0f - 2.0f) {
                    gCat->setPosition({SCREEN_WIDTH / 2.0f, gCat->getPosition().y});
                    gCat->setAcceleration({0.0f, gCat->getAcceleration().y});
                }
            }

            else if ((gCat->getAcceleration().x < 0.0f) && (gCat->getPosition().x > SCREEN_WIDTH / 2.0f)) {
                if (gCat->getPosition().x <= SCREEN_WIDTH / 2.0f + 2.0f) {
                    gCat->setPosition({SCREEN_WIDTH / 2.0f, gCat->getPosition().y});
                    gCat->setAcceleration({0.0f, gCat->getAcceleration().y});
                }
            }
        }

        if (gCat->getFuel() <= 0.0f) gCat->setFuel(0.0f);

        UpdateMusicStream(bridgerton360);

        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    gCat->render();

    for (int i = 0; i < NUMBER_OF_BEDS_AT_A_TIME; ++i) gAura[i].render();
    gBlueBed->render();
    gCubeBed->render();
    gFrogBed->render();
    gGrayBed->render();
    gMovingBed->render();
    gPinkBed->render();
    gScratchPost->render();

    DrawText(TextFormat("Purr Power: %.2f",
        gCat->getFuel()), SCREEN_WIDTH  - 270.0f, 20.0f, 16.0f, BLACK);
    DrawText(TextFormat("Nap Coordinates: %.0f p , %.0f p",
        gCat->getPosition().x, gCat->getPosition().y), SCREEN_WIDTH - 270.0f, 50.0f, 16.0f, BLACK);

    if (gLevel == L0) {
        gInstructions->render();
    }

    if(isGameOver == true) {
        if (gCat->getDirection() == PURR) gWin->render();
        else if (gCat->getDirection() == ANGRY) gLose->render();
    }

    if ((gCat->getScore() == 100) && (gCat->getDirection() == PURR)) {
        g0Score->deactivate();
        g100Score->activate();
        g100Score->render();
    }
    else if ((gCat->getScore() == 5000) && (gCat->getDirection() == PURR) ) {
        g0Score->deactivate();
        g5000Score->activate();
        g5000Score->render();
    }
    else {
        g0Score->activate();
        g0Score->render();
    }

    EndDrawing();
}

void shutdown() {
    UnloadMusicStream(bridgerton360);
    CloseAudioDevice();

    CloseWindow();
}

int main(void) {
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}

void setLevel(Level l) {
    gLevel = l;
    gCat->setFuel(50.0f);
    if (gLevel == L1) {
        for (int i = 0; i < NUMBER_OF_BEDS_AT_A_TIME; ++i) {
            gAura[i].activate();
            gAura[i].setTexture("Project3_LunarLander/Textures/aura.png");
            gAura[i].setScale({376.0f * sizeRatio , 162.0f * sizeRatio});
            gAura[i].setColliderDimensions({376.0f * sizeRatio,  162.0f * sizeRatio});
        }
    }

    else if (gLevel == L2) {
        for (int i = 0; i < NUMBER_OF_BEDS_AT_A_TIME; ++i) {
            gAura[i].activate();
            gAura[i].setTexture("Project3_LunarLander/Textures/aura.png");
            gAura[i].setScale({ 376.0f * sizeRatio, 162.0f * sizeRatio });
            gAura[i].setColliderDimensions({376.0f * sizeRatio, 162.0f * sizeRatio});
        }
    }

    else if (gLevel == L3) {
        for (int i = 0; i < NUMBER_OF_BEDS_AT_A_TIME; ++i) {
            gAura[i].activate();
            gAura[i].setTexture("Project3_LunarLander/Textures/aura.png");
            gAura[i].setScale({376.0f * sizeRatio, 162.0f * sizeRatio });
            gAura[i].setColliderDimensions({376.0f * sizeRatio, 162.0f * sizeRatio});
        }
    }
    else {
        for (int i = 0; i < NUMBER_OF_BEDS_AT_A_TIME; ++i) {
            gAura[i].deactivate();
            gAura[i].setTexture("Project3_LunarLander/Textures/aura.png");
            gAura[i].setScale({162.0f * sizeRatio, 376.0f * sizeRatio});
            gAura[i].setColliderDimensions({162.0f * sizeRatio, 376.0f * sizeRatio});
        }
    }
}

void resetGame() {
    isGameOver = false;
    isGravityOn = false;

    gCat->setFuel(50.0f);
    gCat->setAcceleration({0.0f, 0.0f});
    gCat->setMovement({0.0f, 0.0f});
    gCat->setDirection(DOWN);
    gCat->setPosition({SCREEN_WIDTH / 2.0f, 20.0f});
    gCat->setScale({250.0f * sizeRatio, 250.0f * sizeRatio});
    gCat->resetMovement();

    gMovingBed->setSpeed(80);
    if (gMovingBed->getPosition().x >= ORIGIN.x + 450.0f) gMovingBed->setMovement({ -1.0f , 0.0f });
    else if (gMovingBed->getPosition().x <= ORIGIN.x - 450.0f) gMovingBed->setMovement({ 1.0f , 0.0f });
    else gMovingBed->setMovement({ 1.0f , 0.0f });
    g100Score->deactivate();
    g5000Score->deactivate();
}