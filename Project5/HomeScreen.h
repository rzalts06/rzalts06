#include "Scene.h"

#ifndef CS3113_HOMESCREEN_H
#define CS3113_HOMESCREEN_H

class HomeScreen : public Scene {
public:
    static constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
    float mCurrEnergy = 100.0f,
          mMaxEnergy = 100.0f;

    HomeScreen();
    HomeScreen(Vector2 origin, const char *bgHexCode);
    ~HomeScreen();

    float getCurrEnergy() const {return mCurrEnergy;}
    float getMaxEnergy() const {return mMaxEnergy;}

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif //CS3113_HOMESCREEN_H