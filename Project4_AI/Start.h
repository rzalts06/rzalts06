#include "Scene.h"

#ifndef CS3113_START_H
#define CS3113_START_H

class Start : public Scene {
public:
    static constexpr float STAGE_DIMENSION = 120.0f;

    Start();
    Start(Vector2 origin, const char *bgHexCode);
    ~Start();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};
#endif //CS3113_START_H