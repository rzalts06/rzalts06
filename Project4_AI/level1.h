#include "Start.h"

#ifndef CS3113_LEVEL1_H
#define CS3113_LEVEL1_H

constexpr int LEVEL_WIDTH  = 11,
              LEVEL_HEIGHT = 7;

class Level1 : public Scene {
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 0, 2, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

public:
    static constexpr int NUMBER_OF_CHEESE    =  3;

    static constexpr float STAGE_DIMENSION = 120.0f,
                CHEESE_DIMENSION = 45.0f,
                ACCELERATION_OF_GRAVITY = 981.0f,
                FIXED_TIMESTEP = 1.0f / 60.0f,
                END_GAME_THRESHOLD = 800.0f;

    Level1();
    Level1(Vector2 origin, const char *bgHexCode);
    ~Level1();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif //CS3113_LEVEL1_H