#include "Winner.h"
#ifndef CS3113_LOSER_H
#define CS3113_LOSER_H

class Loser : public Scene {
public:
    static constexpr float STAGE_DIMENSION = 120.0f;

    Loser();
    Loser(Vector2 origin, const char *bgHexCode);
    ~Loser();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};


#endif //CS3113_LOSER_H