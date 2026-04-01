#include "level2.h"

Level2::Level2() : Scene { {0.0f}, nullptr   } {}
Level2::Level2(Vector2 mOrigin, const char *bgHexCode) : Scene { mOrigin, bgHexCode } {}

Level2::~Level2() { shutdown(); }

void Level2::initialise() {
    mGameState.nextSceneID = 0;
    

    mGameState.bg = LoadMusicStream("Project4_AI/Audio/LifesWhatYouMakeIt .wav");
    SetMusicVolume(mGameState.bg, 0.20f);
    PlayMusicStream(mGameState.bg);

    mGameState.sadOOO = LoadSound("Project4_AI/Audio/sadOOO.wav");
    SetSoundVolume(mGameState.sadOOO, 1.0f);
    mGameState.cameraShutters = LoadSound("Project4_AI/Audio/papSound.wav");
    SetSoundVolume(mGameState.cameraShutters, 0.5f);
    mGameState.oooTss = LoadSound("Project4_AI/Audio/ooTss.wav");
    SetSoundVolume(mGameState.oooTss, 0.5f);
    mGameState.sweetNiblets = LoadSound("Project4_AI/Audio/sweetNiblets.wav");
    SetSoundVolume(mGameState.sweetNiblets, 0.5f);
    mGameState.SGHMFight = LoadSound("Project4_AI/Audio/SGInsults.wav");
    SetSoundVolume(mGameState.SGHMFight, 0.5f);
    mGameState.kissCheer = LoadSound("Project4_AI/Audio/CrowdOOO.wav");
    SetSoundVolume(mGameState.kissCheer, 0.5f);
    
    mGameState.stages = new P4Map(11, 7,
         (unsigned int *)mLevelData,
         "Project4_AI/Textures/stageSet.png",
         STAGE_DIMENSION,
         3, 1, mOrigin);

    std::map<Direction, std::vector<int>> hannahAnimationAtlas = {
        {DOWN,  {  0,  1,  2,  3 }},
        {LEFT,  {  4, 5, 6, 7 }},
        {UP,    { 12, 13, 14, 15 }},
        {RIGHT, { 8, 9, 10, 11 }},
    };

    float sizeRatio = 0.75f;

    mGameState.hannah = new P4Entity(
        {110.0f, 95.0f},
        {102.0f * sizeRatio, 153.0f * sizeRatio},
        "Project4_AI/Textures/HMwalk.png",
        ATLAS,
        {4 , 4},
        hannahAnimationAtlas,
        PLAYER
    );

    mGameState.hannah->setJumpingPower(700.0f);
    mGameState.hannah->setColliderDimensions({
        mGameState.hannah->getScale().x / 3.0f,
        mGameState.hannah->getScale().y / 3.0f
    });
    mGameState.hannah->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

    mGameState.cheese = new P4Entity[NUMBER_OF_CHEESE];
    for (int i = 0; i < NUMBER_OF_CHEESE; i++) {
        mGameState.cheese[i].setTexture("Project4_AI/Textures/Cheese.png");
        mGameState.cheese[i].setEntityType(BLOCK);
        mGameState.cheese[i].setScale({CHEESE_DIMENSION, CHEESE_DIMENSION});
        mGameState.cheese[i].setColliderDimensions({CHEESE_DIMENSION, CHEESE_DIMENSION});

        cheeseWasActive[i] = true;
    }
    mGameState.cheese[0].setPosition({360.0f, 410.0f});
    mGameState.cheese[1].setPosition({610.0f, 160.0f});
    mGameState.cheese[2].setPosition({1200.0f, 755.0f});

    mGameState.paps = new P4Entity(
    {540.0f, 90.0f},
    {136.0f, 204.0f},
    "Project4_AI/Textures/Pap.png",
    NPC);

    mGameState.paps->setAIType(WANDERER);
    mGameState.paps->setAIState(WALKING);
    mGameState.paps->setDirection(RIGHT);
    mGameState.paps->setSpeed(100);
    mGameState.paps->setAcceleration({0.0f, 0.0f});

    mGameState.paps->setColliderDimensions({
        mGameState.paps->getScale().x / 3.0f,
        mGameState.paps->getScale().y / 5.0f
    });

    mGameState.selener = new P4Entity(
        {mGameState.hannah->getPosition().x - 500.0f, mGameState.hannah->getPosition().y},
        {136.0f, 204.0f},
        "Project4_AI/Textures/SG.png",
        NPC);

    mGameState.selener->setAIType(FOLLOWER);
    mGameState.selener->setAIState(FOLLOWING);
    mGameState.selener->setDirection(RIGHT);
    mGameState.selener->setSpeed(70);
    mGameState.selener->setAcceleration({0.0f, 0.0f});

    mGameState.selener->setColliderDimensions({
        mGameState.selener->getScale().x / 5.0f,
        mGameState.selener->getScale().y / 3.0f
    });

    mGameState.jakey = new P4Entity(
        {mGameState.cheese[2].getPosition().x + 30.0f, mGameState.cheese[2].getPosition().y + 30.0f},
        {136.0f, 204.0f},
        "Project4_AI/Textures/JR.png",
        NPC);

    mGameState.jakey->setAIType(ORBITER);
    mGameState.jakey->setAIState(ORBITING);
    mGameState.jakey->setAcceleration({0.0f, 0.0f});
    mGameState.jakey->setColliderDimensions({
        mGameState.jakey->getScale().x / 5.0f,
        mGameState.jakey->getScale().y / 3.0f
    });

    gWon = false;
}

void Level2::update(float deltaTime) {
    UpdateMusicStream(mGameState.bg);

    mGameState.hannah->update(
            deltaTime,
            nullptr,
            mGameState.stages,
            mGameState.cheese,
            NUMBER_OF_CHEESE, 0.0f, 0.0f, nullptr
        );

        for (int i = 0; i < NUMBER_OF_CHEESE; ++i) {
            if (cheeseWasActive[i] == true && mGameState.cheese[i].isActive() == false) {
                PlaySound(mGameState.sweetNiblets);
                cheeseWasActive[i] = false;
                ++cheeseCollected;
            }
        }

        mGameState.paps->update(deltaTime, nullptr, mGameState.stages,
             nullptr, 0, 540.0f, 1020.0f, nullptr);

        mGameState.selener->update(deltaTime, mGameState.hannah, mGameState.stages,
             nullptr, 0, 0.0f, 0.0f, nullptr);

        mGameState.jakey->update(deltaTime, nullptr, mGameState.stages,
             nullptr, 0, 0.0f, 0.0f, &mGameState.cheese[2]);

        if (cheeseWasActive[0] == false &&
            cheeseWasActive[1] == false &&
            cheeseWasActive[2] == false && !gWon ) {

            gWon = true;
            PlaySound(mGameState.oooTss);
            mGameState.nextSceneID = 3;
        }

        Vector2 currentPlayerPosition = {mGameState.hannah->getPosition().x, mOrigin.y};
}

void Level2::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.stages->render();
    mGameState.hannah->render();
    mGameState.paps->render();
    mGameState.selener->render();
    mGameState.jakey->render();

    for (int i = 0; i < NUMBER_OF_CHEESE; i++) mGameState.cheese[i].render();
}

void Level2::shutdown() {
    delete mGameState.hannah;
    delete mGameState.stages;
    delete mGameState.paps;
    delete mGameState.selener;
    delete mGameState.jakey;

    UnloadMusicStream(mGameState.bg);
    UnloadSound(mGameState.oooTss);
    UnloadSound(mGameState.sadOOO);
    UnloadSound(mGameState.cameraShutters);
    UnloadSound(mGameState.sweetNiblets);
    UnloadSound(mGameState.SGHMFight);
    UnloadSound(mGameState.kissCheer);
}
