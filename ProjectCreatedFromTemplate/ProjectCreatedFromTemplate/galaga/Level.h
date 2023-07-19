#pragma once

#include <GameEntity.h>
#include <Timer.h>
#include <galaga/PlaySideBar.h>
#include <galaga/BackgroundStars.h>
#include <galaga/Player.h>

using namespace SDLFramework;

namespace Galaga
{
    class Level : public GameEntity
    {
    public:
        enum LevelStates { Running, Finished, GameOver };

        Level(int stage, PlaySideBar* sideBar, Player* player);
        ~Level();

        void Update() override;
        void Render() override;

        LevelStates State();

    private:
        Timer* mTimer;
        PlaySideBar* mSideBar;
        BackgroundStars* mStars;

        int mStage;
        bool mStageStarted;

        float mLabelTimer;
        Texture* mStageLabel;
        Scoreboard* mStageNumber;
        float mStageLabelOnScreen;
        float mStageLabelOffScreen;
        Texture* mReadyLabel;
        float mReadyLabelOnScreen;
        float mReadyLabelOffScreen;
        Player* mPlayer;

        bool mPlayerHit;
        float mRespawnDelay;
        float mRespawnTimer;
        float mRespawnLabelOnScreen;

        Texture* mGameOverLabel;
        float mGameOverDelay;
        float mGameOverTimer;
        float mGameOverLabelOnScreen;

        void StartStage();

        LevelStates mCurrentState;
        void HandleStartLabels();
        void HandleCollisions();
        void HandlePlayerDeath();
    };

    void Level::StartStage()
    {
        mStageStarted = true;
    }

    Level::Level(int stage, PlaySideBar* sideBar, Player* player)
    {
        mTimer = Timer::Instance();
        mSideBar = sideBar;
        mSideBar->SetLevel(stage);
        mStars = BackgroundStars::Instance();

        mStage = stage;
        mStageStarted = false;
        mLabelTimer = 0.0f;

        mStageLabel = new Texture("STAGE", "emulogic.ttf", 32, { 75, 75, 200 });
        mStageLabel->Parent(this);
        mStageLabel->Position(Graphics::SCREEN_WIDTH * 0.35f,
            Graphics::SCREEN_HEIGHT * 0.5f);

        mStageNumber = new Scoreboard({ 75, 75, 200 });
        mStageNumber->Score(mStage);
        mStageNumber->Parent(this);
        mStageNumber->Position(Graphics::SCREEN_WIDTH * 0.5f,
            Graphics::SCREEN_HEIGHT * 0.5f);

        mStageLabelOnScreen = 0.0f;
        mStageLabelOffScreen = 1.5f;

        mReadyLabel = new Texture("READY", "emulogic.ttf", 32, { 150, 0, 0 });
        mReadyLabel->Parent(this);
        mReadyLabel->Position(Graphics::SCREEN_WIDTH * 0.4f,
            Graphics::SCREEN_HEIGHT * 0.5f);

        mReadyLabelOnScreen = mStageLabelOffScreen;
        mReadyLabelOffScreen = mReadyLabelOnScreen + 3.0f;

        mPlayer = player;

        mPlayerHit = false;
        mRespawnDelay = 3.0f;

        mRespawnTimer = 0.0f;
        mRespawnLabelOnScreen = 2.0f;
        mGameOverLabel = new Texture("GAME OVER", "emulogic.ttf", 32, { 150, 0, 0 });
        mGameOverLabel->Parent(this);
        mGameOverLabel->Position(Graphics::SCREEN_WIDTH * 0.4f,
            Graphics::SCREEN_HEIGHT * 0.5f); mGameOverDelay = 6.0f;
        mGameOverTimer = 0.0f;
        mGameOverLabelOnScreen = 1.0f;
        mCurrentState = Running;
    }

    Level::~Level()
    {
        mTimer = nullptr;
        mSideBar = nullptr;
        mStars = nullptr;

        delete mStageLabel;
        mStageLabel = nullptr;

        delete mStageNumber;
        mStageNumber = nullptr;

        delete mReadyLabel;
        mReadyLabel = nullptr;

        mPlayer = nullptr;

        delete mGameOverLabel;
        mGameOverLabel = nullptr;
    }

    void Level::Update()
    {
        if (mStageStarted)
        {
            HandleCollisions();

            if (mPlayerHit)
            {
                HandlePlayerDeath();
            }
            else
            {
                if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_N))
                {
                    mCurrentState = Finished;
                }
            }
            return;
        }

        HandleStartLabels();

        mLabelTimer += mTimer->DeltaTime();
        if (mLabelTimer < mStageLabelOffScreen)
        {
            return;
        }

        mStars->Scroll(true);
        if (mStage > 1)
        {
            StartStage();
        }
        else if (mLabelTimer >= mReadyLabelOffScreen)
        {
            StartStage();
            mPlayer->Active(true);
            mPlayer->Visible(true);
        }

    }

    void Level::Render()
    {
        if (!mStageStarted)
        {
            if (mLabelTimer > mStageLabelOnScreen && mLabelTimer < mStageLabelOffScreen)
            {
                mStageLabel->Render();
                mStageNumber->Render();
            }
            else if (mLabelTimer > mReadyLabelOnScreen && mLabelTimer < mReadyLabelOffScreen)
            {
                mReadyLabel->Render();
            }
        }
        else
        {
            if (mPlayerHit)
            {
                if (mRespawnTimer >= mRespawnLabelOnScreen)
                {
                    mReadyLabel->Render();
                }

                if (mGameOverTimer >= mGameOverLabelOnScreen)
                {
                    mGameOverLabel->Render();
                }
            }
        }
    }

    Level::LevelStates Level::State()
    {
        return mCurrentState;
    }

    void Level::HandleStartLabels()
    {
        mLabelTimer += mTimer->DeltaTime();
        if (mLabelTimer >= mStageLabelOffScreen)
        {
            mStars->Scroll(true);
            if (mStage > 1)
            {
                StartStage();
            }
            else
            {
                if (mLabelTimer >= mReadyLabelOffScreen)
                {
                    StartStage(); mPlayer->Active(true); mPlayer->Visible(true);
                }
            }
        }
    }

    void Level::HandleCollisions()
    {
        if (!mPlayerHit)
        {
            if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_X))
            {
                mPlayer->WasHit();
                mSideBar->SetShips(mPlayer->Lives());

                mPlayerHit = true;
                mRespawnTimer = 0.0f;

                mPlayer->Active(false);
                mStars->Scroll(false);
            }
        }
    }

    void Level::HandlePlayerDeath()
    {
        if (mPlayer->IsAnimating())
        {
            return;
        }

        if (mPlayer->Lives() > 0)
        {
            if (mRespawnTimer == 0.0f)
            {
                mPlayer->Visible(false);
            }
            mRespawnTimer += mTimer->DeltaTime();

            if (mRespawnTimer >= mRespawnDelay)
            {
                mPlayer->Active(true);
                mPlayer->Visible(true);
                mPlayerHit = false;
                mStars->Scroll(true);
            }
        }
        else
        {
            if (mGameOverTimer == 0.0f)
            {
                mPlayer->Visible(false);
            }

            mGameOverTimer += mTimer->DeltaTime();

            if (mGameOverTimer >= mGameOverDelay)
            {
                mCurrentState = GameOver;
            }
        }
    }
}