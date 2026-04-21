#include "Level3.h"

Level3::Level3() : Scene { {0.0f}, nullptr   } {}
Level3::Level3(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

Level3::~Level3() { shutdown(); }

void Level3::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.effect = new Effects(mOrigin, float(1200 * 1.25f), float(900 * 1.25f));
    mGameState.effect->start(FADEIN);
    mGameState.effect->setEffectSpeed(0.40f);

    mGameState.locketSound = LoadSound("Project5/Audio/locketSound.wav");
    SetSoundVolume(mGameState.locketSound, 0.7f);

    mGameState.musicBoxSound = LoadSound("Project5/Audio/musicBoxSound.wav");
    SetSoundVolume(mGameState.musicBoxSound, 0.5f);

    mGameState.ericSound = LoadSound("Project5/Audio/ohEric.wav");
    SetSoundVolume(mGameState.ericSound, 0.5f);

    mGameState.tridentSound = LoadSound("Project5/Audio/tridentSound.wav");
    SetSoundVolume(mGameState.tridentSound, 0.3f);

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

    mGameState.ariel = new Entity({0.0f, 0.0f},
        {150.0f, 150.0f},
        "Project5/Textures/ariel_sprite.png",
        ATLAS, {4, 5}, animationAtlas, PLAYER);

    mGameState.ariel->setSpeed(210.0f);
    mGameState.ariel->setAcceleration({0.0f, 450.0f});
    mGameState.ariel->setColliderDimensions({
        mGameState.ariel->getScale().x / 2.0f,
        mGameState.ariel->getScale().y / 2.0f
    });

    mGameState.musicBox = new Entity({200.0f, -100.0f},
        {75.0f, 75.0f},
        "Project5/Textures/musicBox.png", BLOCK);

    mGameState.trident = new Entity({2100.0f, 400.0f},
        {75.0f, 75.0f},
        "Project5/Textures/trident.png", BLOCK);

    mGameState.locket = new Entity({4500.0f, 800.0f},
    {70.0f, 70.0f},
    "Project5/Textures/locket.png", BLOCK);

    mGameState.collidables[0] = mGameState.musicBox;
    mGameState.collidables[1] = mGameState.trident;
    mGameState.collidables[2] = mGameState.locket;

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

    mGameState.net = new Entity({200.0f, 0.0f},
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

    mGameState.eric = new Entity({5100.0f, 0.0f},
        {150.0f, 150.0f},
        "Project5/Textures/PrinceEric.png", NPC);

    mGameState.eric->setAIType(VERTICAL);
    mGameState.eric->setAIState(DROWNING);
    mGameState.eric->setSpeed(50.0f);
    mGameState.eric->setAcceleration({0.0f, 900.0f});

    mGameState.eric->setColliderDimensions({
        mGameState.eric->getScale().x / 2.0f,
        mGameState.eric->getScale().y / 2.0f
    });

    mGameState.cave = new Entity({6000.0f, 500.0f},
        {200.0f, 200.0f},
        "Project5/Textures/cave_entrance.png", PLATFORM);

    mGameState.gCamera.offset   = mOrigin;
    mGameState.gCamera.target   = mGameState.ariel->getPosition();
    mGameState.gCamera.rotation = 0.0f;
    mGameState.gCamera.zoom     = 1.0f;
}

void Level3::update(float deltaTime) {
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
        nullptr, mGameState.trident->getPosition(), 150.0f, 1.5f, 0.0f, 0.0f);

    mGameState.flounder->update(deltaTime, mGameState.ariel, 0, nullptr, 0,
        mGameState.ariel, {}, 0.0f, 0.0f, 0.0f, 0.0f);

    mGameState.net->update(deltaTime, nullptr, 0, nullptr,  0,
        nullptr, {}, 0.0f, 0.0f,
        mGameState.gCamera.target.y - SCREEN_HEIGHT / 2.0f + 50.0f, mGameState.gCamera.target.y + SCREEN_HEIGHT / 2.0f - 50.0f);

    mGameState.eric->update(deltaTime, nullptr, 0, nullptr,  0,
        mGameState.ariel, {}, 0.0f, 0.0f,
        mGameState.gCamera.target.y - SCREEN_HEIGHT / 2.0f + 50.0f, mGameState.gCamera.target.y + SCREEN_HEIGHT / 2.0f - 50.0f);


    if (mGameState.ariel->collideWith(mGameState.net) || mGameState.ariel->collideWith(mGameState.ursala)) mGameState.gGameOver = true;

    if (mGameState.ariel->collideWith(mGameState.eric) &&
        (mGameState.isCollected[0] == true && mGameState.isCollected[1] == true && mGameState.isCollected[2] == true)) {
        mGameState.eric->setAIType(FOLLOWER);
        mGameState.eric->setAIState(FOLLOWING);
        mGameState.eric->setPosition({mGameState.ariel->getPosition().x + 120.0f, mGameState.ariel->getPosition().y + 10.0f});

        if (mCollisionBuffer <= 0.0f) {
            mCollisionBuffer = 10.0f;
            PlaySound(mGameState.ericSound);
        }
    }

    if (mGameState.ariel->collideWith(mGameState.cave) && mGameState.eric->getAIState() == FOLLOWING &&
        (mGameState.isCollected[0] == true && mGameState.isCollected[1] == true && mGameState.isCollected[2] == true)) {
        mGameState.nextSceneID = 6;
        mGameState.effect->start(FADEOUT);
        mGameState.effect->setEffectSpeed(0.40f);
    }

    else if (mGameState.ariel->collideWith(mGameState.cave) && mGameState.eric->getAIState() != FOLLOWING &&
        (mGameState.isCollected[0] == false || mGameState.isCollected[1] == false || mGameState.isCollected[2] == false)) {
        mGameState.nextSceneID = 5;
        mGameState.effect->start(FADEOUT);
        mGameState.effect->setEffectSpeed(0.40f);
    }

    if (mCollisionBuffer <= 0.0f && mGameState.ariel->collideWith(mGameState.ursala)) {
        mCollisionBuffer = 5.0f;
        if (mGameState.ariel->collideWith(mGameState.ursala)) PlaySound(mGameState.ursalaSound);
    }

    for (int i = 0; i < NUMBER_OF_COLLECTABLES; i++) {
        if (mGameState.isCollected[i] == false && mGameState.collidables[i]->isActive() == false) {
            if (mGameState.collidables[i] == mGameState.musicBox) PlaySound(mGameState.musicBoxSound);
            else if (mGameState.collidables[i] == mGameState.trident) PlaySound(mGameState.tridentSound);
            else if (mGameState.collidables[i] == mGameState.locket) PlaySound(mGameState.locketSound);
        }
    }

    mGameState.gCamera.target = mGameState.ariel->getPosition();
    mGameState.effect->update(deltaTime, &mOrigin);
}

void Level3::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.waves->render();
    mGameState.musicBox->render();
    mGameState.ariel->render();
    mGameState.ursala->render();
    mGameState.trident->render();
    mGameState.flounder->render();
    mGameState.net->render();
    mGameState.locket->render();
    mGameState.cave->render();
    mGameState.eric->render();
}

void Level3::shutdown() {
    delete mGameState.waves;
    delete mGameState.ariel;
    delete mGameState.ursala;
    delete mGameState.flounder;
    delete mGameState.net;
    delete mGameState.cave;
    delete mGameState.eric;

    delete mGameState.musicBox;
    delete mGameState.trident;
    delete mGameState.locket;

    delete mGameState.effect;
    mGameState.effect = nullptr;

    UnloadSound(mGameState.musicBoxSound);
    UnloadSound(mGameState.tridentSound);
    UnloadSound(mGameState.locketSound);
    UnloadSound(mGameState.ursalaSound);
    UnloadSound(mGameState.ericSound);
}
