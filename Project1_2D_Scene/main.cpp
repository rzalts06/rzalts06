#include "cs3113.h"
#include <math.h>


// Enums
enum Background {DEBUT, FEARLESS, SPEAKNOW, REDTV, SEAGULL, REP, LOVER, FOLKLORE, EVERMORE, MIDNIGHTS, TTPD, TLOAS};
enum Direction {LEFT, RIGHT};

// Global Constants
constexpr int   SCREEN_WIDTH  = 900,
                SCREEN_HEIGHT = 900,
                FPS           = 60,
                SIZE          = 200,
                FRAME_LIMIT   = 100;
constexpr float MAX_AMP       = 10.0f;

constexpr float RADIUS          = 250.0f, // radius of the orbit
                ORBIT_SPEED     = 0.02f,  // the speed at which the triangle will travel its orbit
                MAX_AMPLITUDE   = 20.0f,  // by how much the triangle will be expanding/contracting
                PULSE_SPEED     = 100.0f, // how fast the triangle is going to be "pulsing"
                PULSE_INCREMENT = 10.0f;  // the current value we're scaling by

constexpr Vector2 ORIGIN = {
    SCREEN_WIDTH / 2,
    SCREEN_HEIGHT / 2
};

constexpr Vector2 CORNER = {
    0.0f,
    0.0f
};

constexpr Vector2 BASE_SIZE   = { (float) SIZE, (float) SIZE };

constexpr char BG_DEBUT[] = "#A4C9A5",
               BG_FEARLESS[] = "#F0C080",
               BG_SPEAKNOW[] = "#C7A8CA",
               BG_REDTV[] = "#7A2E3A",
               BG_1989[] = "#B5E5F9",
               BG_REPUTATION[] = "#2B2B2B",
               BG_LOVER[] = "#F6B0CC",
               BG_FOLKLORE[] = "#CDC9C0",
               BG_EVERMORE[] = "#C5AB90",
               BG_MIDNIGHTS[] = "#242E47",
               BG_TTPD[] = "#FFFFFF",
               BG_TLOAS[] = "#E15921";

constexpr char TS_LOGO[] = "Textures/TS LOGO.png",
               TS_LOVER_RECORD[] = "Textures/TS Lover Record.png",
               TS_FULL_BODY[] = "Textures/TS 1989.png",
               SC_FULL_BODY[] = "Textures/SC SNS.png",
               SC_LOGO[] = "Textures/SC LOGO.png";

// Global Variables
AppStatus gAppStatus = RUNNING;
Direction gDirection = LEFT;

float gScaleFactor   = SIZE,
      gAngleOrbit    = 0.0f,
      gAngleRotation = 0.0f,
      gPreviousTicks = 0.0f,
      gPulseTime     = 0.0f;
Vector2 gPosition    = ORIGIN,
        gPositionSC  = { -53.0f, 5.0f };
Vector2 gRecordCenter = ORIGIN;
Vector2 gScale     = BASE_SIZE;

float gOrbitLocation = 0.0f;

int gCurrBg;

Texture2D gTSLOGO;
Texture2D gTSLoverRecord;
Texture2D gTS;
Texture2D gSC;

// Function Declarations
void initialize();
void processInput();
void update();
void render();
void shutdown();

void initialize()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple 2D Scene");

    gTSLOGO = LoadTexture(TS_LOGO);
    gTSLoverRecord = LoadTexture(TS_LOVER_RECORD);
    gTS = LoadTexture(TS_FULL_BODY);
    gSC = LoadTexture(SC_FULL_BODY);

    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gPulseTime += 1.0f * deltaTime;

    gScale = {
        SCREEN_WIDTH + MAX_AMP * cos(gPulseTime),
        SCREEN_HEIGHT / 7.0f + MAX_AMP * cos(gPulseTime)
    };

    gCurrBg = int(gPulseTime * FPS / 100) % 12;

    gAngleOrbit += ORBIT_SPEED;
    gPosition.x = gRecordCenter.x + RADIUS * cos(gAngleOrbit);
    gPosition.y = gRecordCenter.y + RADIUS * sin(gAngleOrbit);

    gAngleRotation += 1.0f * gDirection == LEFT ? -1 : 1;
}

void render() {
    BeginDrawing();

    Rectangle textureAreaLogoTS = {
        0.0f,
        0.0f,
        static_cast<float>(gTSLOGO.width),
        static_cast<float>(gTSLOGO.height)
    };

    Rectangle destinationAreaLogoTS = {
        (SCREEN_WIDTH - gScale.x) / 2.0f,
        0.0f,
        static_cast<float>(gScale.x),
        static_cast<float>(gScale.y)
    };

    Vector2 taylorLogoOrigin = {
        static_cast<float>(gScale.x) / 2.0f,
        static_cast<float>(gScale.y) / 2.0f
    };

    Rectangle textureAreaRecord = {
        0.0f,
        0.0f,
        static_cast<float>(gTSLoverRecord.width),
        static_cast<float>(gTSLoverRecord.height)
    };

    Rectangle destinationAreaRecord = {
        SCREEN_WIDTH / 2.0f,
        SCREEN_HEIGHT / 2.0f,
        static_cast<float>(gTSLoverRecord.width) / 2.0f,
        static_cast<float>(gTSLoverRecord.height) / 2.0f
    };

    Vector2 recordOrigin = {
        static_cast<float>(gTSLoverRecord.width) / 4.0f,
        static_cast<float>(gTSLoverRecord.height) / 4.0f
    };

    Rectangle textureAreaTS = {
        0.0f,
        0.0f,
        static_cast<float>(gTS.width),
        static_cast<float>(gTS.height)
    };

    Rectangle destinationAreaTS = {
        gPosition.x,
        gPosition.y,
        static_cast<float>(gTS.width) / 6.0f,
        static_cast<float>(gTS.height) / 6.0f
    };

    Vector2 taylorOrigin = {
        static_cast<float>(gTS.width) / 12.0f,
        static_cast<float>(gTS.height) / 12.0f
    };

    Rectangle textureAreaSC = {
        0.0f,
        0.0f,
        static_cast<float>(gSC.width),
        static_cast<float>(gSC.height)
    };

    Rectangle destinationAreaSC = {
        gPosition.x - (2.25f * gPositionSC.x),
        gPosition.y - (2.25f * gPositionSC.y),
        static_cast<float>(gSC.width) / 6.0f,
        static_cast<float>(gSC.height) / 6.0f
    };

    Vector2 scOrigin = {
        static_cast<float>(gSC.width) / 12.0f,
        static_cast<float>(gSC.height) / 12.0f
    };

    if (gCurrBg == DEBUT) {
        ClearBackground(ColorFromHex(BG_DEBUT));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == FEARLESS) {
        ClearBackground(ColorFromHex(BG_FEARLESS));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == SPEAKNOW) {
        ClearBackground(ColorFromHex(BG_SPEAKNOW));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == REDTV) {
        ClearBackground(ColorFromHex(BG_REDTV));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == SEAGULL) {
        ClearBackground(ColorFromHex(BG_1989));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == REP) {
        ClearBackground(ColorFromHex(BG_REPUTATION));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == LOVER) {
        ClearBackground(ColorFromHex(BG_LOVER));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == FOLKLORE) {
        ClearBackground(ColorFromHex(BG_FOLKLORE));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == EVERMORE) {
        ClearBackground(ColorFromHex(BG_EVERMORE));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == MIDNIGHTS) {
        ClearBackground(ColorFromHex(BG_MIDNIGHTS));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else if (gCurrBg == TTPD) {
        ClearBackground(ColorFromHex(BG_TTPD));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    else {
        ClearBackground(ColorFromHex(BG_TLOAS));

        DrawTexturePro(
        gTSLOGO,
        textureAreaLogoTS,
        destinationAreaLogoTS,
        CORNER,
        0.0f,
        WHITE);

        DrawTexturePro(
        gTSLoverRecord,
        textureAreaRecord,
        destinationAreaRecord,
        recordOrigin,
        gAngleRotation,
        WHITE);

        DrawTexturePro(
        gTS,
        textureAreaTS,
        destinationAreaTS,
        taylorOrigin,
        0.0f,
        WHITE);

        DrawTexturePro(
        gSC,
        textureAreaSC,
        destinationAreaSC,
        scOrigin,
        0.0f,
        WHITE);
    }

    EndDrawing();
}

void shutdown()
{
    CloseWindow();
}

int main(void)
{
    initialize();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
