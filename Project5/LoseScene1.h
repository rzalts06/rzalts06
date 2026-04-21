#include "Level3.h"
// Losing by going to cave without all the items

#ifndef CS3113_LOSESCENE1_H
#define CS3113_LOSESCENE1_H

class LoseScene1 : public Scene {
public:
    static constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
    float mCurrEnergy = 100.0f,
          mMaxEnergy = 100.0f;

    LoseScene1();
    LoseScene1(Vector2 origin, const char *bgHexCode);
    ~LoseScene1();

    float getCurrEnergy() const {return mCurrEnergy;}
    float getMaxEnergy() const {return mMaxEnergy;}

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif //CS3113_LOSESCENE1_H