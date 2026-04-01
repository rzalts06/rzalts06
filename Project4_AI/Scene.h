#include "P4Entity.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    P4Entity* hannah;
    P4Entity* paps;
    P4Entity* jakey;
    P4Entity* selener;
    P4Entity* cheese;
    P4Entity* singleHannah;
    P4Entity* ATLogo;
    P4Entity* singleStage;
    P4Entity* directions;
    P4Entity* winner;
    P4Entity* loser;

    P4Map* stages;


    Camera2D camera;

    Music bg;
    Sound sadOOO;
    Sound cameraShutters;
    Sound oooTss;
    Sound sweetNiblets;
    Sound SGHMFight;
    Sound kissCheer;

    int nextSceneID;
};

class Scene
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";
    bool hittingPap = false,
    hittingSel = false,
    cheeseWasActive[3] = {true, true, true},
    gWon = false;
    int cheeseCollected = 0;

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState   getState()           const { return mGameState; }
    Vector2     getOrigin()          const { return mOrigin;    }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }
    int getCollectedCheese() const {return cheeseCollected; }

    void setCollectedCheese(int cheeses) {cheeseCollected = cheeses; }
};

#endif
