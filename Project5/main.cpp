#include "cs3113.h"
#include "Entity.h"
#include "WinScene.h"

/**
* Author: Rachelle Zaltsman
* Assignment: Under the Sea
* Date due: April 24, 2026, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

// Global Constants
constexpr int SCREEN_WIDTH  = 1200,
              SCREEN_HEIGHT = 900,
              FPS           = 120,
              COLLECTABLES_PER_LEVEL = 3,
              TOTAL_COLLECTABLES = 9,
              NUMBER_OF_LEVELS = 7;

constexpr char    BG_COLOUR[] = "#B5E5F8";
constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

// Global Variables
AppStatus gAppStatus   = RUNNING;

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f,
      gCollisionBuffer = 0.0f;

Scene* gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

ShaderProgram gShader;

Music bg;
Sound ohNo;

HomeScreen* gHome = nullptr;
Level1* gLevel1 = nullptr;
Level2* gLevel2 = nullptr;
Level3* gLevel3 = nullptr;
LoseScene1* gLose1 = nullptr;
LoseScene2* gLose2 = nullptr;
WinScreen* gWin = nullptr;

Entity* allItems[TOTAL_COLLECTABLES] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
bool isallItems[TOTAL_COLLECTABLES] = {false, false, false, false, false, false, false, false, false};
int lastCollected = -1;

// Function Declarations
void switchToScene(Scene *scene);
void resetLevel(Scene* level, int levelNumber);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{
    gCurrentScene = scene;
    gCurrentScene->initialise();

    if (gCurrentScene->getState().ariel != nullptr)
        gCurrentScene->getState().gCamera.target = gCurrentScene->getState().ariel->getPosition();

    else gCurrentScene->getState().gCamera.target = ORIGIN;
}

void resetLevel(Scene *level, int levelNumber) {
    if (level == nullptr) return;

    switchToScene(level);

    gLevel1->setCurrEnergy(gLevel1->getMaxEnergy());
    gLevel2->setCurrEnergy(gLevel2->getMaxEnergy());
    gLevel3->setCurrEnergy(gLevel3->getMaxEnergy());

    for (int i = 0; i < COLLECTABLES_PER_LEVEL; i++) {
        level->getState().isCollected[i] = false;

        if (level->getState().collidables[i] != nullptr) level->getState().collidables[i]->activate();
    }

    for (int i = levelNumber * COLLECTABLES_PER_LEVEL; i < levelNumber * COLLECTABLES_PER_LEVEL + COLLECTABLES_PER_LEVEL; i++) {
        isallItems[i] = false;
        allItems[i]->deactivate();
    }
}


void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 5: Under the Sea");
    InitAudioDevice();

    bg =  LoadMusicStream("Project5/Audio/partOfYourWorld.wav");
    SetMusicVolume(bg, 0.20f);
    PlayMusicStream(bg);

    ohNo = LoadSound("Project5/Audio/ohNo.wav");
    SetSoundVolume(ohNo, 0.2f);

    gShader.load("Project5/vertex.glsl", "Project5/fragment.glsl");

    gHome = new HomeScreen(ORIGIN, BG_COLOUR);
    gLevels.push_back(gHome);

    gLevel1 = new Level1(ORIGIN, BG_COLOUR);
    gLevels.push_back(gLevel1);

    gLevel2 = new Level2(ORIGIN, BG_COLOUR);
    gLevels.push_back(gLevel2);

    gLevel3 = new Level3(ORIGIN, BG_COLOUR);
    gLevels.push_back(gLevel3);

    gLose1 = new LoseScene1(ORIGIN, BG_COLOUR);
    gLevels.push_back(gLose1);

    gLose2 = new LoseScene2(ORIGIN, BG_COLOUR);
    gLevels.push_back(gLose2);

    gWin = new WinScreen(ORIGIN, BG_COLOUR);
    gLevels.push_back(gWin);

    allItems[0] = new Entity({0.0f, 0.0f},
        {50.0f, 50.0f},
        "Project5/Textures/fork.png", NONE);
    allItems[1] = new Entity({0.0f, 0.0f},
        {50.0f, 50.0f},
        "Project5/Textures/voice_shell.png", NONE);
    allItems[2] = new Entity({0.0f, 0.0f},
        {50.0f, 50.0f},
        "Project5/Textures/flower.png", NONE);
    allItems[3] = new Entity({0.0f, 0.0f},
        {50.0f, 50.0f},
        "Project5/Textures/shell.png", NONE);
    allItems[4] = new Entity({0.0f, 0.0f},
        {50.0f, 50.0f},
        "Project5/Textures/pipe.png", NONE);
    allItems[5] = new Entity({0.0f, 0.0f},
        {50.0f, 50.0f},
        "Project5/Textures/corkScrew.png", NONE);
    allItems[6] = new Entity({0.0f, 0.0f},
        {50.0f, 50.0f},
        "Project5/Textures/musicBox.png", NONE);
    allItems[7] = new Entity({0.0f, 0.0f},
        {50.0f, 50.0f},
        "Project5/Textures/trident.png", NONE);
    allItems[8] = new Entity({0.0f, 0.0f},
        {50.0f, 50.0f},
        "Project5/Textures/locket.png", NONE);

    for (int i = 0; i < TOTAL_COLLECTABLES; i++) {
        allItems[i]->deactivate();
    }

    switchToScene(gHome);

    SetTargetFPS(FPS);
}


void processInput() {
    if (gCurrentScene->getState().ariel != nullptr) {
        gCurrentScene->getState().ariel->resetMovement();

        if      (IsKeyDown(KEY_A)) gCurrentScene->getState().ariel->moveLeft();
        else if (IsKeyDown(KEY_D)) gCurrentScene->getState().ariel->moveRight();

        if (IsKeyDown(KEY_S)) gCurrentScene->getState().ariel->moveDown();
        else if (IsKeyDown(KEY_W)) gCurrentScene->getState().ariel->moveUp();

        if (GetLength(gCurrentScene->getState().ariel->getMovement()) > 1.0f) gCurrentScene->getState().ariel->normaliseMovement();

    }

    if (IsKeyPressed(KEY_R)) {
        resetLevel(gLevel1, 0);
        resetLevel(gLevel2, 1);
        resetLevel(gLevel3, 2);

        switchToScene(gHome);
    }

    if (IsKeyDown(KEY_I)) gHome->getState().directions->activate();
    else gHome->getState().directions->deactivate();

    if (gCurrentScene == gHome) {
        if (IsKeyPressed(KEY_ENTER)) switchToScene(gLevel1);
    }
    if (IsKeyPressed(KEY_ONE)) switchToScene(gLevel1);
    if (IsKeyPressed(KEY_TWO)) switchToScene(gLevel2);
    if (IsKeyPressed(KEY_THREE)) switchToScene(gLevel3);
    if (IsKeyPressed(KEY_H)) switchToScene(gHome);
    if (IsKeyPressed(KEY_DOWN)) switchToScene(gLose1);
    if (IsKeyPressed(KEY_LEFT)) switchToScene(gLose2);
    if (IsKeyPressed(KEY_UP)) switchToScene(gWin);

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

    while (deltaTime >= FIXED_TIMESTEP) {
        gCurrentScene->update(FIXED_TIMESTEP);

        if (gCurrentScene->getState().ariel == nullptr) {
            deltaTime -= FIXED_TIMESTEP;
            continue;
        }

        if (gCollisionBuffer > 0.0f) {
            gCollisionBuffer -= FIXED_TIMESTEP;
            if (gCollisionBuffer < 0.0f) gCollisionBuffer = 0.0f;
        }

        int levelCollectables = 0;
        if (gCurrentScene == gLevel2) levelCollectables = 3;
        else if (gCurrentScene == gLevel3) levelCollectables = 6;

        for (int i = 0; i < COLLECTABLES_PER_LEVEL; i++) {
            int j = levelCollectables + i;
            if (gCurrentScene->getState().collidables[i]->isActive() == false &&
                gCurrentScene->getState().isCollected[i] == false) {
                gCurrentScene->getState().isCollected[i] = true;
                isallItems[j] = true;
                allItems[j]->activate();
                lastCollected = j;
                gCollisionBuffer = 0.0f;
            }
        }

        // if colliding with net or ursala most recent item get released back and if it is an item from a previous level
        // you redo that level
        bool currentlyColliding = (gCurrentScene->getState().ursala != nullptr &&
            gCurrentScene->getState().ariel->collideWith(gCurrentScene->getState().ursala) == true) ||
            (gCurrentScene->getState().net != nullptr &&
                gCurrentScene->getState().ariel->collideWith(gCurrentScene->getState().net) == true);

        if (currentlyColliding == true &&
            gCollisionBuffer <= 0.0f &&
            lastCollected != -1) {
            int itemNumber = lastCollected % COLLECTABLES_PER_LEVEL;
            int levelNumber = lastCollected / COLLECTABLES_PER_LEVEL;

            Scene* newLevel = nullptr;
            if (levelNumber == 0) newLevel = gLevel1;
            else if (levelNumber == 1) newLevel = gLevel2;
            else if (levelNumber == 2) newLevel = gLevel3;

            isallItems[lastCollected] = false;
            allItems[lastCollected]->deactivate();
            gCollisionBuffer = 3.0f;
            PlaySound(ohNo);

            if (newLevel != nullptr) {
                if (newLevel != gCurrentScene) resetLevel(newLevel, levelNumber);
                else {
                    newLevel->getState().isCollected[itemNumber] = false;
                    if (newLevel->getState().collidables[itemNumber] != nullptr)
                        newLevel->getState().collidables[itemNumber]->activate();
                }
            }
            lastCollected = -1;
            for (int i = TOTAL_COLLECTABLES - 1; i >= 0; --i) {
                if (isallItems[i] == true) {
                    lastCollected = i;
                    break;
                }
            }
            gCollisionBuffer = 2.0f;
        }
        else if (currentlyColliding == true &&
            gCollisionBuffer <= 0.0f &&
            lastCollected <= 0) {
            switchToScene(gLose2);
        }

        // Shader Distances for Eric, Collectables, and Caves and Collisions for  Ursala and Net
        if (gCurrentScene->getState().ursala != nullptr &&
            gCurrentScene->getState().ariel->collideWith(gCurrentScene->getState().ursala) == true) {
            gShader.setInt("ursalaCollide", 1);
        }
        else gShader.setInt("ursalaCollide", 0);

        if (gCurrentScene->getState().net != nullptr &&
            gCurrentScene->getState().ariel->collideWith(gCurrentScene->getState().net) == true) {
            gShader.setInt("netCollide", 1);
        }
        else gShader.setInt("netCollide", 0);

        if (gCurrentScene->getState().eric != nullptr) {
            float ericDis = (gCurrentScene->getState().eric->getPosition().x -
                                gCurrentScene->getState().ariel->getPosition().x);
            gShader.setFloat("ericDis", ericDis);
        }
        else gShader.setFloat("ericDis", -1.0f);

        if (gCurrentScene->getState().cave != nullptr) {
            float caveDis = (gCurrentScene->getState().cave->getPosition().x -
                                gCurrentScene->getState().ariel->getPosition().x);
            gShader.setFloat("caveDis", caveDis);
        }
        else gShader.setFloat("caveDis", -1.0f);

        float itemDis = -1.0f;
        for (int i = 0; i < COLLECTABLES_PER_LEVEL; i++) {
            Entity* item = gCurrentScene->getState().collidables[i];

            if (item != nullptr && item->isActive() == true) {
                float dis = (item->getPosition().x -
                                gCurrentScene->getState().ariel->getPosition().x);
                if (dis >= 0.0f && (itemDis < 0.0f || dis < itemDis)) itemDis = dis;
            }
        }
        gShader.setFloat("itemDis", itemDis);

        UpdateMusicStream(bg);

        deltaTime -= FIXED_TIMESTEP;
    }
    gTimeAccumulator = deltaTime;
}

void render() {
    BeginDrawing();

    if (gCurrentScene == gLevel1 || gCurrentScene == gLevel2 || gCurrentScene == gLevel3) {
        BeginMode2D(gCurrentScene->getState().gCamera);

        gShader.begin();
        gCurrentScene->render();
        gShader.end();

        EndMode2D();

        if (gCurrentScene->getState().effect != nullptr) {
            gCurrentScene->getState().effect->render();
        }

        for (int i = 0; i < TOTAL_COLLECTABLES; i++) {
            if (isallItems[i] == true && allItems[i] != nullptr) {
                allItems[i]->setPosition({30.0f + i * 50.0f, 30.0f});
                allItems[i]->render();
            }
        }

        DrawRectangle(SCREEN_WIDTH - 200.0f - 30.0f, 30.0f, 200.0f, 25.0f, BLACK);
        DrawText(TextFormat("Swim Stamina"),
        SCREEN_WIDTH  - 350.0f, 30.0f, 19.0f, BLACK);
        if (gCurrentScene == gLevel1) {
            DrawRectangle(SCREEN_WIDTH - 200.0f - 30.0f, 30.0f, (gLevel1->getCurrEnergy() / gLevel1->getMaxEnergy()) * 200.0f, 25.0f, BLUE);
        }
        else if (gCurrentScene == gLevel2) {
            DrawRectangle(SCREEN_WIDTH - 200.0f - 30.0f, 30.0f, (gLevel2->getCurrEnergy() / gLevel2->getMaxEnergy()) * 200.0f, 25.0f, BLUE);
        }
        else if (gCurrentScene == gLevel3) {
            DrawRectangle(SCREEN_WIDTH - 200.0f - 30.0f, 30.0f, (gLevel3->getCurrEnergy() / gLevel3->getMaxEnergy()) * 200.0f, 25.0f, BLUE);
        }
    }

    else gCurrentScene->render();

    EndDrawing();
}

void shutdown()
{
    for (int i = 0; i < TOTAL_COLLECTABLES; i++) delete allItems[i];
    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    UnloadSound(ohNo);
    UnloadMusicStream(bg);
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