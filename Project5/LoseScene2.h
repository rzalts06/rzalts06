#include "LoseScene1.h"
// Losing by going to cave without Eric

#ifndef CS3113_LOSESCENE2_H
#define CS3113_LOSESCENE2_H

class LoseScene2 : public Scene {
public:
    static constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
    float mCurrEnergy = 100.0f,
          mMaxEnergy = 100.0f;

    LoseScene2();
    LoseScene2(Vector2 origin, const char *bgHexCode);
    ~LoseScene2();

    float getCurrEnergy() const {return mCurrEnergy;}
    float getMaxEnergy() const {return mMaxEnergy;}

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif //CS3113_LOSESCENE2_H