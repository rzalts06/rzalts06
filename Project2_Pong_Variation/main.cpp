#include "Project2_Pong_Variation/cs3113.h"
#include <math.h>


// Enums
enum Score { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
enum GameStatus {START, GAME_OVER, SINGLEP};

// Global Constants
constexpr int SCREEN_WIDTH  = 800,
              SCREEN_HEIGHT = 500,
              FPS           = 60,
              PUSHER_SPEED  = 200,
              PUCK_SPEED    = 250,
              MAX_PUCKS = 9,
              MAX_SCORE = 5;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0 },

                  PUSHER_BASE_SIZE = { 50.0f, 50.0f },
                  PUSHER1_INIT_POS  = { SCREEN_WIDTH / 2.0f + 350.0f, SCREEN_HEIGHT / 2.0f },
                  PUSHER2_INIT_POS  = { SCREEN_WIDTH / 2.0f - 350.0f, SCREEN_HEIGHT / 2.0f },

                  PUCK_BASE_SIZE   = { 35.0f, 35.0f },
                  PUCK_INIT_POS    = {PUSHER1_INIT_POS.x - 35.0f, PUSHER1_INIT_POS.y},

                  PLAYER_ONE_SCORE_SIZE = {100.0f, 150.0f},
                  PLAYER_ONE_SCORE_POS = {SCREEN_WIDTH / 2.0f - 30.0f, 30.0f},
                  PLAYER_TWO_SCORE_SIZE = {100.0f, 150.0f},
                  PLAYER_TWO_SCORE_POS = {SCREEN_WIDTH / 2.0f + 40.0f, 30.0f};

// Global Variables
AppStatus gAppStatus = RUNNING;
GameStatus gGameStatus = START;
float     gAngle         = 0.0f,
          gPreviousTicks = 0.0f;

int gKeyNumPucks = 1;

Vector2 gTablePos = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 30.0f},
        gTableMovement = {0.0f, 0.0f},
        gTableScale = {SCREEN_WIDTH * 1.25f, SCREEN_HEIGHT * 1.75f},

        gP1ScorePos = {PLAYER_ONE_SCORE_POS},
        gP1ScoreMovement = {0.0f, 0.0f},
        gP1ScoreScale = PLAYER_ONE_SCORE_SIZE,

        gP2ScorePos = {PLAYER_TWO_SCORE_POS},
        gP2ScoreMovement = {0.0f, 0.0f},
        gP2ScoreScale = PLAYER_TWO_SCORE_SIZE,

        gMeme1Pos = {SCREEN_WIDTH / 2.0f - 200.0f, SCREEN_HEIGHT / 2.0f},
        gMeme1Scale = {250.0f, 250.0f},

        gMeme2Pos = {SCREEN_WIDTH / 2.0f + 200.0, SCREEN_HEIGHT / 2.0f},
        gMeme2Scale = {250.0f, 250.0f},

        gPusher1Pos = PUSHER1_INIT_POS,
        gPusher1Movement = { 0.0f, 0.0f },
        gPusher1Scale    = PUSHER_BASE_SIZE,

        gPusher2Pos = PUSHER2_INIT_POS,
        gPusher2Movement = { 0.0f, 0.0f },
        gPusher2Scale    = PUSHER_BASE_SIZE,

        gPuckPos[9],
        gPuckMovement[9],
        gPuckScale[9],
        gHalfPuck[9],

        gHalfPusher = {gPusher1Scale.x / 2.0f, gPusher1Scale.y / 2.0f};

constexpr char HOCKEY_PUCK[] = "Project2_Pong_Variation/Textures/air_hockey_puck.png",
               HOCKEY_PUSHER[] = "Project2_Pong_Variation/Textures/air_hockey_pusher.png",
               HOCKEY_TABLE[] = "Project2_Pong_Variation/Textures/air_hockey_table.png",
               SCORE_0[] = "Project2_Pong_Variation/Textures/score_0.png",
               SCORE_1[] = "Project2_Pong_Variation/Textures/score_1.png",
               SCORE_2[] = "Project2_Pong_Variation/Textures/score_2.png",
               SCORE_3[] = "Project2_Pong_Variation/Textures/score_3.png",
               SCORE_4[] = "Project2_Pong_Variation/Textures/score_4.png",
               SCORE_5[] = "Project2_Pong_Variation/Textures/score_5.png",
               SCORE_6[] = "Project2_Pong_Variation/Textures/score_6.png",
               SCORE_7[] = "Project2_Pong_Variation/Textures/score_7.png",
               SCORE_8[] = "Project2_Pong_Variation/Textures/score_8.png",
               SCORE_9[] = "Project2_Pong_Variation/Textures/score_9.png",
               WINNER_MEME[] = "Project2_Pong_Variation/Textures/WinnerMeme.jpg",
               LOSER_MEME[] = "Project2_Pong_Variation/Textures/GameOverMeme.jpg";

Texture2D gPuck[9];
Texture2D gPusher;
Texture2D gTable;

Texture2D gScores[10];

Texture2D player1Score;
Texture2D player2Score;

Texture2D gWinner;
Texture2D gLoser;

Score p1Score = ZERO;
Score p2Score = ZERO;

// Function Declarations
void initialize();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2 *postionA,  const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB);
void renderObject(const Texture2D *texture, const Vector2 *position, const Vector2 *scale);
void renderOpponent(const Texture2D *texture, const Vector2 *position, const Vector2 *scale);
Score goalScored(Score score);
bool hasWon(Score score);
void memeGenerator(Score p1, Score p2);
void resetGame();

// Function Definitions
void initialize() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong Variation");

    for (int num = 0; num < MAX_PUCKS; ++num) {
        gPuck[num] = LoadTexture(HOCKEY_PUCK);

        gPuckPos[num] = {PUCK_INIT_POS.x, PUCK_INIT_POS.y + 15.0f * num};
        gPuckMovement[num] = { 0.0f, 0.0f };
        gPuckScale[num]   = PUCK_BASE_SIZE;

        gHalfPuck[num] = {gPuckScale[num].x / 2.0f, gPuckScale[num].y / 2.0f};
    }

    gPusher = LoadTexture(HOCKEY_PUSHER);
    gTable = LoadTexture(HOCKEY_TABLE);

    gScores[0] = LoadTexture(SCORE_0);
    gScores[1] = LoadTexture(SCORE_1);
    gScores[2] = LoadTexture(SCORE_2);
    gScores[3] = LoadTexture(SCORE_3);
    gScores[4] = LoadTexture(SCORE_4);
    gScores[5] = LoadTexture(SCORE_5);
    gScores[6] = LoadTexture(SCORE_6);
    gScores[7] = LoadTexture(SCORE_7);
    gScores[8] = LoadTexture(SCORE_8);
    gScores[9] = LoadTexture(SCORE_9);

    player1Score = gScores[p1Score];
    player2Score = gScores[p2Score];

    gLoser = LoadTexture(LOSER_MEME);
    gWinner = LoadTexture(WINNER_MEME);

    SetTargetFPS(FPS);
}

void processInput() {
    gPusher1Movement = {0.0f, 0.0f};

    if (IsKeyPressed(KEY_ONE)) gKeyNumPucks = 1;
    if (IsKeyPressed(KEY_TWO)) gKeyNumPucks = 2;
    if (IsKeyPressed(KEY_THREE)) gKeyNumPucks = 3;
    if (IsKeyPressed(KEY_FOUR)) gKeyNumPucks = 4;
    if (IsKeyPressed(KEY_FIVE)) gKeyNumPucks = 5;
    if (IsKeyPressed(KEY_SIX)) gKeyNumPucks = 6;
    if (IsKeyPressed(KEY_SEVEN)) gKeyNumPucks = 7;
    if (IsKeyPressed(KEY_EIGHT)) gKeyNumPucks = 8;
    if (IsKeyPressed(KEY_NINE)) gKeyNumPucks = 9;

    if (IsKeyPressed(KEY_P)) resetGame();

    if (IsKeyPressed(KEY_M)) gGameStatus = SINGLEP;

    if (IsKeyPressed(KEY_G)) gGameStatus = START;

    if (gGameStatus == START) {
        gPusher2Movement = {0.0f, 0.0f};

        if (IsKeyDown(KEY_W)) gPusher2Movement.y = -1;
        else if (IsKeyDown(KEY_S)) gPusher2Movement.y = 1;
        if (IsKeyDown(KEY_A)) gPusher2Movement.x = -1;
        else if (IsKeyDown(KEY_D)) gPusher2Movement.x = 1;

        if (GetLength(&gPusher2Movement) > 1.0f) Normalise(&gPusher2Movement);

        if (IsKeyDown(KEY_UP)) gPusher1Movement.y = -1;
        else if (IsKeyDown(KEY_DOWN)) gPusher1Movement.y = 1;
        if (IsKeyDown(KEY_LEFT)) gPusher1Movement.x = -1;
        else if (IsKeyDown(KEY_RIGHT)) gPusher1Movement.x = 1;

        if (GetLength(&gPusher1Movement) > 1.0f) Normalise(&gPusher1Movement);
    }

    if (gGameStatus == SINGLEP) {
        gPusher2Movement.x = 0.0f;

        if (gPusher2Movement.y == 0.0f) gPusher2Movement.y = 1.0f;

        if (IsKeyDown(KEY_UP)) gPusher1Movement.y = -1;
        else if (IsKeyDown(KEY_DOWN)) gPusher1Movement.y = 1;
        if (IsKeyDown(KEY_LEFT)) gPusher1Movement.x = -1;
        else if (IsKeyDown(KEY_RIGHT)) gPusher1Movement.x = 1;

        if (GetLength(&gPusher1Movement) > 1.0f) Normalise(&gPusher1Movement);
    }

    if (WindowShouldClose() || IsKeyPressed(KEY_Q)) gAppStatus = TERMINATED;
}

void update() {
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    if (gGameStatus == SINGLEP) {
        if (gPusher2Pos.y <= 0.0f + gHalfPusher.y) {
            gPusher2Movement.y = 1.0f;
        }
        else if (gPusher2Pos.y >= SCREEN_HEIGHT - gHalfPusher.y) {
            gPusher2Movement.y = -1.0f;
        }
    }

    gPusher1Pos = {
        gPusher1Pos.x + PUSHER_SPEED * gPusher1Movement.x * deltaTime,
        gPusher1Pos.y + PUSHER_SPEED * gPusher1Movement.y * deltaTime
    };

    gPusher2Pos = {
        gPusher2Pos.x + PUSHER_SPEED * gPusher2Movement.x * deltaTime,
        gPusher2Pos.y + PUSHER_SPEED * gPusher2Movement.y * deltaTime
    };

    // ADDITION OF MANY PUCKS
    for (int num = 0; num < gKeyNumPucks; ++num) {
        gPuckPos[num] = {
            gPuckPos[num].x + PUCK_SPEED * gPuckMovement[num].x * deltaTime,
            gPuckPos[num].y + PUCK_SPEED * gPuckMovement[num].y * deltaTime};

        if (isColliding(&gPusher1Pos, &gPusher1Scale,&gPuckPos[num], &gPuckScale[num]) == true) {
            gPuckMovement[num].x += -1.0f;
            gPuckMovement[num].y += gPusher1Movement.y * 0.4f;
        }

        if (isColliding(&gPusher2Pos, &gPusher2Scale,&gPuckPos[num], &gPuckScale[num]) == true) {
            gPuckMovement[num].x += 1.0f;
            gPuckMovement[num].y += gPusher2Movement.y * 0.4f;
        }

        // SCORING
        if (gPuckPos[num].x - gHalfPuck[num].x <=0.0f) {
            gPuckPos[num] = {gPusher2Pos.x + 40.0f, gPusher2Pos.y};
            gPuckMovement[num] = {1.0f, 1.0f};

            p2Score = goalScored(p2Score);
            player2Score = gScores[p2Score];
        }

        if (gPuckPos[num].x + gHalfPuck[num].x >= SCREEN_WIDTH) {
            gPuckPos[num] = {gPusher1Pos.x - 40.0f, gPusher1Pos.y};
            gPuckMovement[num] = {-1.0f, -1.0f};

            p1Score = goalScored(p1Score);
            player1Score = gScores[p1Score];
        }

        if (hasWon(p1Score) || hasWon(p2Score)) {
            gPuckMovement[num] = {0.0f, 0.0f};
            gPuckPos[num] = ORIGIN;
            gGameStatus = GAME_OVER;
        }

        // PUCK WALLS
        if (gPuckPos[num].y - gHalfPuck[num].y <= 0.0f) {
            gPuckPos[num].y = gHalfPuck[num].y;
            gPuckMovement[num].y *= -1.0f;
        }

        if (gPuckPos[num].y + gHalfPuck[num].y >= SCREEN_HEIGHT) {
            gPuckPos[num].y = SCREEN_HEIGHT - gHalfPuck[num].y;
            gPuckMovement[num].y *= -1.0f;
        }
    }

    // PUSHER 1 WALLS
    if (gPusher1Pos.x - gHalfPusher.x <= 0.0f) {
        gPusher1Pos.x = gHalfPusher.x;
        gPusher1Movement.x *= -1.0f;
    }

    if (gPusher1Pos.x + gHalfPusher.x >= SCREEN_WIDTH) {
        gPusher1Pos.x = SCREEN_WIDTH - gHalfPusher.x;
        gPusher1Movement.x *= -1.0f;
    }

    if (gPusher1Pos.y - gHalfPusher.y <= 0.0f) {
        gPusher1Pos.y = gHalfPusher.y;
        gPusher1Movement.y *= -1.0f;
    }

    if (gPusher1Pos.y + gHalfPusher.y >= SCREEN_HEIGHT) {
        gPusher1Pos.y = SCREEN_HEIGHT - gHalfPusher.y;
        gPusher1Movement.y *= -1.0f;
    }

    // PUSHER 2 WALLS
    if (gPusher2Pos.x - gHalfPusher.x <= 0.0f) {
        gPusher2Pos.x = gHalfPusher.x;
        gPusher2Movement.x *= -1.0f;
    }

    if (gPusher2Pos.x + gHalfPusher.x >= SCREEN_WIDTH) {
        gPusher2Pos.x = SCREEN_WIDTH - gHalfPusher.x;
        gPusher2Movement.x *= -1.0f;
    }

    if (gPusher2Pos.y - gHalfPusher.y <= 0.0f) {
        gPusher2Pos.y = gHalfPusher.y;
        gPusher2Movement.y *= -1.0f;
    }

    if (gPusher2Pos.y + gHalfPusher.y >= SCREEN_HEIGHT) {
        gPusher2Pos.y = SCREEN_HEIGHT - gHalfPusher.y;
        gPusher2Movement.y *= -1.0f;
    }
}

void render() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    renderObject(&gTable, &gTablePos, &gTableScale);
    renderObject(&player1Score, &gP1ScorePos, &gP1ScoreScale);
    renderObject(&player2Score, &gP2ScorePos, &gP2ScoreScale);
    renderObject(&gPusher, &gPusher1Pos, &gPusher1Scale);
    renderOpponent(&gPusher, &gPusher2Pos, &gPusher2Scale);

    for (int p = 0; p < gKeyNumPucks; ++p) {
        renderObject(&gPuck[p], &gPuckPos[p], &gPuckScale[p]);
    }

    if (gGameStatus == GAME_OVER) {
        memeGenerator(p1Score, p2Score);
    }

    EndDrawing();
}

void shutdown() {
    CloseWindow();

    UnloadTexture(gLoser);
    UnloadTexture(gWinner);
    UnloadTexture(gPusher);
    UnloadTexture(gTable);

    UnloadTexture(gPuck[0]);
    UnloadTexture(gPuck[1]);
    UnloadTexture(gPuck[2]);
    UnloadTexture(gPuck[3]);
    UnloadTexture(gPuck[4]);
    UnloadTexture(gPuck[5]);
    UnloadTexture(gPuck[6]);
    UnloadTexture(gPuck[7]);
    UnloadTexture(gPuck[8]);

    UnloadTexture(gScores[0]);
    UnloadTexture(gScores[1]);
    UnloadTexture(gScores[2]);
    UnloadTexture(gScores[3]);
    UnloadTexture(gScores[4]);
    UnloadTexture(gScores[5]);
    UnloadTexture(gScores[6]);
    UnloadTexture(gScores[7]);
    UnloadTexture(gScores[8]);
    UnloadTexture(gScores[9]);
}

bool isColliding(const Vector2 *postionA,  const Vector2 *scaleA,
                 const Vector2 *positionB, const Vector2 *scaleB) {
    float xDistance = fabs(postionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(postionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

void renderObject(const Texture2D *texture, const Vector2 *position, const Vector2 *scale) {
    Rectangle textureArea = {
        0.0f, 0.0f,
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    DrawTexturePro(
        *texture,
        textureArea, destinationArea, originOffset,
        gAngle, WHITE
    );
}

void renderOpponent(const Texture2D *texture, const Vector2 *position, const Vector2 *scale) {
    Rectangle textureArea = {
        0.0f, 0.0f,
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    DrawTexturePro(
        *texture,
        textureArea, destinationArea, originOffset,
        gAngle, BLUE
    );
}



Score goalScored(Score score) {
    if (score < NINE) return (Score)(score + 1);
    return NINE;
}

bool hasWon(Score score) {
    return score == NINE;
}

void memeGenerator(Score p1, Score p2) {
    gPusher1Movement = {0.0f, 0.0f};
    gPusher2Movement = {0.0f, 0.0f};
    if ((p1 == NINE) && (p2 < NINE) && (gGameStatus == GAME_OVER)) {
        renderObject(&gWinner, &gMeme1Pos, &gMeme1Scale);
        renderObject(&gLoser, &gMeme2Pos, &gMeme2Scale);
    }
    else if ((p2 == NINE) && (p1 < NINE) && (gGameStatus == GAME_OVER)) {
        renderObject(&gLoser, &gMeme1Pos, &gMeme1Scale);
        renderObject(&gWinner, &gMeme2Pos, &gMeme2Scale);
    }
}

void resetGame() {
    p1Score = ZERO;
    p2Score = ZERO;
    player1Score = gScores[p1Score];
    player2Score = gScores[p2Score];

    gGameStatus = START;
    for (int num = 0; num < MAX_PUCKS; ++num) {
        gPuckPos[num] = {PUCK_INIT_POS.x, PUCK_INIT_POS.y + 15.0f * num};
        gPuckMovement[num] = { 0.0f, 0.0f };
        gPuckScale[num]   = PUCK_BASE_SIZE;

        gHalfPuck[num] = {gPuckScale[num].x / 2.0f, gPuckScale[num].y / 2.0f};
    }

    gPusher1Pos = PUSHER1_INIT_POS;
    gPusher1Movement = { 0.0f, 0.0f };
    gPusher1Scale    = PUSHER_BASE_SIZE;

    gPusher2Pos = PUSHER2_INIT_POS;
    gPusher2Movement = { 0.0f, 0.0f };
    gPusher2Scale    = PUSHER_BASE_SIZE;

    gHalfPusher = {gPusher1Scale.x / 2.0f, gPusher1Scale.y / 2.0f};
}

int main(void) {
    initialize();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
