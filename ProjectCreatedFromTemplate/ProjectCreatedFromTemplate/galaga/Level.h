#pragma once

#include <GameEntity.h>
#include <Timer.h>
#include <galaga/PlaySideBar.h>
#include <galaga/BackgroundStars.h>

using namespace SDLFramework;

namespace Galaga
{
    class Level : public GameEntity
    {
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
        void StartStage();

    public:
        Level(int stage, PlaySideBar* sideBar);
        ~Level();

        void Update() override;
        void Render() override;
    };

    void Level::StartStage()
    {
        mStageStarted = true;
    }

    Level::Level(int stage, PlaySideBar* sideBar)
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
    }

    void Level::Update()
    {
        if (mStageStarted)
        {
            return;
        }

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
    }
}