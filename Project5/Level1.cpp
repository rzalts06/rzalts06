#include "Level1.h"

Level1::Level1() : Scene { {0.0f}, nullptr   } {}
Level1::Level1(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

Level1::~Level1() { shutdown(); }

void Level1::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.effect = new Effects(mOrigin, float(1200 * 1.25f), float(900 * 1.25f));
    mGameState.effect->start(FADEIN);
    mGameState.effect->setEffectSpeed(0.40f);

    mGameState.musicShell = LoadSound("Project5/Audio/ahahah.wav");
    SetSoundVolume(mGameState.musicShell, 0.2f);

    mGameState.forkSound = LoadSound("Project5/Audio/dingleHopper.wav");
    SetSoundVolume(mGameState.forkSound, 0.5f);

    mGameState.itemSound = LoadSound("Project5/Audio/humanStuff.wav");
    SetSoundVolume(mGameState.itemSound, 0.5f);

    mGameState.ursalaSound = LoadSound("Project5/Audio/ursalaLaugh.wav");
    SetSoundVolume(mGameState.ursalaSound, 0.1f);

    mGameState.waves = new Map(80, 20,
    mLevelData,
    "Project5/Textures/ocean_sprite.png",
    150.0f,
    4, 1, mOrigin);

    std::map<Direction, std::vector<int>> animationAtlas = {
        {DOWN,  { 15, 16, 17, 18, 19}},
        {LEFT,  { 5, 6, 7, 8, 9 }},
        {RIGHT,  { 0, 1, 2, 3, 4 }},
        {UP,   { 10, 11, 12, 13, 14}}
    };

    std::map<Direction, std::vector<int>> UrsalaAnimationAtlas = {
        {RIGHT, {0, 1, 2, 3, 4, 5, 6, 7}}
    };

    mGameState.ariel = new Entity({500.0f, 0.0f},
        {150.0f, 150.0f},
        "Project5/Textures/ariel_sprite.png",
        ATLAS, {4, 5}, animationAtlas, PLAYER);

    mGameState.ariel->setSpeed(210.0f);
    mGameState.ariel->setAcceleration({0.0f, 450.0f});
    mGameState.ariel->setColliderDimensions({
        mGameState.ariel->getScale().x / 2.0f,
        mGameState.ariel->getScale().y / 2.0f
    });

    mGameState.voice = new Entity({1800.0f, 300.0f},
        {75.0f, 75.0f},
        "Project5/Textures/voice_shell.png", BLOCK);

    mGameState.fork = new Entity({900.0f, 75.0f},
        {75.0f, 75.0f},
        "Project5/Textures/fork.png", BLOCK);

    mGameState.flower = new Entity({3300.0f, 0.0f},
    {70.0f, 70.0f},
    "Project5/Textures/flower.png", BLOCK);

    mGameState.collidables[0] = mGameState.fork;
    mGameState.collidables[1] = mGameState.voice;
    mGameState.collidables[2] = mGameState.flower;

    mGameState.ursala = new Entity({1950.0f, 100.0f},
        {200.0f, 200.0f},
        "Project5/Textures/ursala_sprite.png",
        ATLAS, {3, 3}, UrsalaAnimationAtlas, NPC);

    mGameState.ursala->setAIType(ORBITER);
    mGameState.ursala->setAIState(ORBITING);
    mGameState.ursala->setDirection(RIGHT);
    mGameState.ursala->setSpeed(70);
    mGameState.ursala->setAcceleration({0.0f, 0.0f});

    mGameState.ursala->setColliderDimensions({
        mGameState.ursala->getScale().x / 2.0f,
        mGameState.ursala->getScale().y / 2.0f
    });

    mGameState.flounder = new Entity({mGameState.ariel->getPosition().x - 120.0f, mGameState.ariel->getPosition().y + 120.0f},
        {75.0f, 75.0f},
        "Project5/Textures/flounder.png", NPC);

    mGameState.flounder->setAIType(FOLLOWER);
    mGameState.flounder->setAIState(FOLLOWING);
    mGameState.flounder->setSpeed(205.0f);
    mGameState.flounder->setAcceleration({0.0f, 0.0f});

    mGameState.flounder->setColliderDimensions({
        mGameState.flounder->getScale().x / 2.0f,
        mGameState.flounder->getScale().y / 2.0f
    });

    mGameState.net = new Entity({3300.0f, 0.0f},
        {150.0f, 150.0f},
        "Project5/Textures/openNet.png", NPC);

    mGameState.net->setAIType(VERTICAL);
    mGameState.net->setAIState(UPANDDOWN);
    mGameState.net->setSpeed(100.0f);
    mGameState.net->setAcceleration({0.0f, 0.0f});

    mGameState.net->setColliderDimensions({
        mGameState.net->getScale().x / 2.0f,
        mGameState.net->getScale().y / 2.0f
    });

    mGameState.cave = new Entity({4200.0f, 400.0f},
        {200.0f, 200.0f},
        "Project5/Textures/cave_entrance.png", PLATFORM);

    mGameState.gCamera.offset   = mOrigin;
    mGameState.gCamera.target   = mGameState.ariel->getPosition();
    mGameState.gCamera.rotation = 0.0f;
    mGameState.gCamera.zoom     = 1.0f;
}

void Level1::update(float deltaTime) {
    if (mCollisionBuffer > 0.0f) {
        mCollisionBuffer -= deltaTime;
        if (mCollisionBuffer < 0.0f) mCollisionBuffer = 0.0f;
    }
    
    mGameState.waves->update(deltaTime);

    mGameState.ariel->update(deltaTime, nullptr, 0, mGameState.collidables,  3,
        nullptr, {}, 0.0f, 0.0f, 0.0f, 0.0f);

    float arielCurrSpeed = GetLength(mGameState.ariel->getMovement()) * mGameState.ariel->getSpeed();
    if (arielCurrSpeed > 0.0f) {
        mCurrEnergy -= 10.0f * deltaTime;
        if (mCurrEnergy < 0.0f) mCurrEnergy = 0.0f;
    }
    else {
        mCurrEnergy += 20.0f * deltaTime;
        if (mCurrEnergy > mMaxEnergy) mCurrEnergy = mMaxEnergy;
    }

    if (mCurrEnergy <= 0.0f) mGameState.ariel->setSpeed(0);
    else if (mCurrEnergy <= 50.0f) mGameState.ariel->setSpeed(105);
    else if (mCurrEnergy <= 100.0f) mGameState.ariel->setSpeed(160);
    else mGameState.ariel->setSpeed(210);

    mGameState.ursala->update(deltaTime, nullptr, 0, nullptr, 0,
        nullptr, mGameState.voice->getPosition(), 150.0f, 1.5f, 0.0f, 0.0f);

    mGameState.flounder->update(deltaTime, mGameState.ariel, 0, nullptr, 0,
        mGameState.ariel, {}, 0.0f, 0.0f, 0.0f, 0.0f);

    mGameState.net->update(deltaTime, nullptr, 0, nullptr,  0,
        nullptr, {}, 0.0f, 0.0f,
        mGameState.gCamera.target.y - SCREEN_HEIGHT / 2.0f + 50.0f,
        mGameState.gCamera.target.y + SCREEN_HEIGHT / 2.0f - 50.0f);

    if (mGameState.ariel->collideWith(mGameState.cave) &&
        (mGameState.isCollected[0] == true && mGameState.isCollected[1] == true && mGameState.isCollected[2] == true)) {
        mGameState.nextSceneID = 2;
    }

    else if (mGameState.ariel->collideWith(mGameState.cave) &&
        (mGameState.isCollected[0] == false || mGameState.isCollected[1] == false || mGameState.isCollected[2] == false)) {
        mGameState.nextSceneID = 4;
    }

    if (mCollisionBuffer <= 0.0f && mGameState.ariel->collideWith(mGameState.ursala)) {
        mCollisionBuffer = 5.0f;
        PlaySound(mGameState.ursalaSound);
    }

    for (int i = 0; i < NUMBER_OF_COLLECTABLES; i++) {
        if (mGameState.isCollected[i] == false && mGameState.collidables[i]->isActive() == false) {
            if (mGameState.collidables[i] == mGameState.fork) PlaySound(mGameState.forkSound);
            else if (mGameState.collidables[i] == mGameState.voice) PlaySound(mGameState.musicShell);
            else if (mGameState.collidables[i] == mGameState.flower) PlaySound(mGameState.itemSound);
        }
    }

    mGameState.gCamera.target = mGameState.ariel->getPosition();
    mGameState.effect->update(deltaTime, &mOrigin);
}

void Level1::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.waves->render();
    mGameState.fork->render();
    mGameState.ariel->render();
    mGameState.ursala->render();
    mGameState.voice->render();
    mGameState.flounder->render();
    mGameState.net->render();
    mGameState.flower->render();
    mGameState.cave->render();
}

void Level1::shutdown() {
    delete mGameState.waves;
    delete mGameState.ariel;
    delete mGameState.ursala;
    delete mGameState.flounder;
    delete mGameState.net;
    delete mGameState.cave;

    delete mGameState.fork;
    delete mGameState.voice;
    delete mGameState.flower;

    delete mGameState.effect;
    mGameState.effect = nullptr;

    UnloadSound(mGameState.musicShell);
    UnloadSound(mGameState.forkSound);
    UnloadSound(mGameState.itemSound);
    UnloadSound(mGameState.ursalaSound);
}
