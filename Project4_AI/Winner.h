#include "Start.h"

#ifndef CS3113_WINNER_H
#define CS3113_WINNER_H

class Winner : public Scene {
public:
    static constexpr float STAGE_DIMENSION = 120.0f;

    Winner();
    Winner(Vector2 origin, const char *bgHexCode);
    ~Winner();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif //CS3113_WINNER_H