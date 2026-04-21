#include "Entity.h"
#include "ShaderProgram.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    Entity* ariel = nullptr;
    Entity* ursala = nullptr;
    Entity* flounder = nullptr;
    Entity* net = nullptr;
    Entity* eric = nullptr;

    Entity* musicBox = nullptr;
    Entity* corkscrew = nullptr;
    Entity* flower = nullptr;
    Entity* fork = nullptr;
    Entity* pipe = nullptr;
    Entity* voice = nullptr;
    Entity* trident = nullptr;
    Entity* locket = nullptr;
    Entity* shell = nullptr;

    Entity* collidables[3] = {nullptr, nullptr, nullptr};
    bool isCollected[3] = {false, false, false};

    Entity* cave = nullptr;
    Map* waves = nullptr;

    Entity* homeScreen = nullptr;
    Entity* loseScreen1 = nullptr;
    Entity* loseScreen2 = nullptr;
    Entity* winScreen = nullptr;
    Entity* directions = nullptr;

    Music winMusic;
    Music loseMusic;
    Music homeScreenMusic;

    Sound musicShell;
    Sound forkSound;
    Sound itemSound;
    Sound locketSound;
    Sound musicBoxSound;
    Sound ericSound;
    Sound pipeSound;
    Sound tridentSound;
    Sound ursalaSound;

    Effects* effect = nullptr;

    Camera2D gCamera = { 0 };

    bool gGameOver = false;

    int nextSceneID;
};

class Scene 
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";
    static constexpr int SCREEN_WIDTH  = 1200,
                    SCREEN_HEIGHT = 900;
    
public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;
    
    GameState&   getState()          { return mGameState; }
    const GameState& getState()    const { return mGameState; }
    Vector2     getOrigin()          const { return mOrigin;    }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }
};

#endif